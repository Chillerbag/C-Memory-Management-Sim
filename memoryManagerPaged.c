#include "memoryManagerPaged.h"
#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// these are constants defined in the spec
#define PAGE_COUNT 512
#define PAGE_SIZE 4

void *intialiseMemoryPaged() {
    struct pagedMemoryState *state = (struct pagedMemoryState *)malloc(sizeof(struct pagedMemoryState));
    state->processesWithMemory = createList();
    state->freePages = PAGE_COUNT;
    state->pageFrames = malloc(PAGE_COUNT * sizeof(char *));
    for (int i = 0; i < PAGE_COUNT; i++)
        state->pageFrames[i] = NULL;
    return state;
}

void clearProcessMemoryPaged(void *statev, process_t *process, int time) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    // clear pages
    int j = 0;
    int totalPages = (process->memoryRequirement + PAGE_SIZE - 1) / PAGE_SIZE;
    int *clearedFrames = malloc(totalPages * sizeof(int));

    for (int i = 0; i < PAGE_COUNT; i++) {
        if (state->pageFrames[i] != NULL && !strcmp(process->pName, state->pageFrames[i])) {
            state->pageFrames[i] = NULL;
            clearedFrames[j++] = i;
        }
    }
    char *array = stringOfIntArray(clearedFrames, totalPages);
    printf("%d,EVICTED,evicted-frames=%s\n", time, array);
    free(array);
    free(clearedFrames);

    // cleanup the rest of our state
    state->freePages += totalPages;
    node_t *toBeFreed = removeMatchFromList(state->processesWithMemory, process->pName);
    if (toBeFreed == NULL) return;
    free(toBeFreed->data->pName);
    free(toBeFreed->data);
    free(toBeFreed);
}

bool allocateMemoryPaged(void *statev, process_t *process, int time) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    int requiredPages = (process->memoryRequirement + PAGE_SIZE - 1) / PAGE_SIZE;
    // Is it already in memory? update it's recency if so.
    node_t *matchNode = removeMatchFromList(state->processesWithMemory, process->pName);
    if (NULL != matchNode) {
        addProcessToList(state->processesWithMemory, matchNode->data);
        free(matchNode);
        int allocatedPages = requiredPages;
        int *allocatedFrames = malloc(requiredPages * sizeof(int));
        for (int i = 0; allocatedPages > 0; i++) {
            if (state->pageFrames[i] != NULL && !strcmp(process->pName, state->pageFrames[i])) {
                allocatedFrames[requiredPages - allocatedPages--] = i;
            }
        }
        char *array = stringOfIntArray(allocatedFrames, requiredPages);
        printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->pName, process->remainingTime, 100 - (100 * state->freePages) / PAGE_COUNT, array);
        free(array);
        free(allocatedFrames);
        return true;
    }

    // if we dont have space, make some
    while (state->freePages < requiredPages) {
        clearProcessMemoryPaged(state, state->processesWithMemory->head->data, time);
    }

    // allocate the memory
    int toBeAllocated = requiredPages;
    int *allocatedFrames = malloc(requiredPages * sizeof(int));
    for (int i = 0; toBeAllocated > 0; i++) {
        if (state->pageFrames[i] == NULL) {
            state->pageFrames[i] = process->pName;
            allocatedFrames[requiredPages - toBeAllocated--] = i;
        }
    }
    state->freePages -= requiredPages;
    addToList(state->processesWithMemory, -1, process->pName, -1, process->memoryRequirement);
    char *array = stringOfIntArray(allocatedFrames, requiredPages);
    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->pName, process->remainingTime, 100 - (100 * state->freePages) / PAGE_COUNT, array);
    free(array);
    free(allocatedFrames);

    return true;
}

void freeStatePaged(void * state) {
    struct pagedMemoryState *memory = (struct pagedMemoryState *)state;
    for (int i = 0; i < PAGE_COUNT; i++) {
        free(memory->pageFrames[i]);
    }
    free(memory->pageFrames);
    
    freeList(memory->processesWithMemory);
    free(memory);
}

// helper function adapted from:https://stackoverflow.com/questions/30234363/how-can-i-convert-an-int-array-into-a-string-array
char *stringOfIntArray(int *array, int size) {
    if (size == 0)
        return "[]";

    char *str = (char *)malloc(sizeof(char) * 5 * size);
    int index = sprintf(&str[0], "[");
    for (int i = 0; i < size; i++)
        index += sprintf(&str[index], "%d,", array[i]);
    sprintf(&str[index - 1], "]");
    return str;
}
