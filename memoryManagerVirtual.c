#include "memoryManagerVirtual.h"
#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_COUNT 512
struct pagedMemoryState {
    char **pageFrames;
    // the head of this list is the least recently used
    list_t *processesWithMemory;
    int freePages;
    int mem;
};


void *intialiseMemoryVirtual() {
    struct pagedMemoryState *state = (struct pagedMemoryState *)malloc(sizeof(struct pagedMemoryState));
    state->processesWithMemory = create_list();

    // TODO:Remove magic number for p_name length
    state->pageFrames = malloc(PAGE_COUNT * sizeof(char *));
    for (int i = 0; i < PAGE_COUNT; i++)
        state->pageFrames[i] = NULL;
    // state->pageFrames[i] = malloc((8) * sizeof(char));
    state->freePages = PAGE_COUNT;
    return state;
}


void clearProcessMemoryVirtual(void *statev, process_t *process, int time) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    // clear pages
    int j = 0;
    int totalPages = (process->memory_requirement + 3) / 4;
    int *clearedFrames = malloc(totalPages * sizeof(int));

    for (int i = 0; i < PAGE_COUNT; i++) {
        if (state->pageFrames[i] != NULL && !strcmp(process->p_name, state->pageFrames[i])) {
            state->pageFrames[i] = NULL;
            clearedFrames[j++] = i;
        }
    }
    char *array = stringOfIntArray(clearedFrames, totalPages);
    printf("%d,EVICTED,evicted-frames=%s\n", time, array);
    free(array);
    free(clearedFrames);

    // cleanup state's processesWithMemory list
    state->freePages += totalPages;
    node_t *toBeFreed = remove_match_from_list(state->processesWithMemory, process->p_name);
    if (toBeFreed == NULL)
        return;

    free(toBeFreed->data->p_name);
    free(toBeFreed->data);
    free(toBeFreed);

}


bool allocateMemoryVirtual(void *statev, process_t *process, int time) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    int minRequiredPages;
    // Is it already in memory?
    node_t *matchNode = remove_match_from_list(state->processesWithMemory, process->p_name);
    int requiredPages = (process->memory_requirement + 3) / 4;
    if (requiredPages < 4) {
        int minRequiredPages = requiredPages;
    }
    else {
        int minRequiredPages = 4;
    }

    if (NULL != matchNode) {
        add_process_to_list(state->processesWithMemory, matchNode->data);
        free(matchNode);
        int toBeAllocated = minRequiredPages;
        int *allocatedFrames = malloc(requiredPages * sizeof(int));
        for (int i = 0; toBeAllocated > 0; i++) {
            if (state->pageFrames[i] != NULL && !strcmp(process->p_name, state->pageFrames[i])) {
                allocatedFrames[minRequiredPages - toBeAllocated--] = i;
            }
        }
        char *array = stringOfIntArray(allocatedFrames, minRequiredPages);
        printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->p_name, process->service_time, 100 - (100 * state->freePages) / PAGE_COUNT, array);
        free(array);
        free(allocatedFrames);
        return true;
    }
    // if we dont have space, make some
    while (state->freePages < minRequiredPages) {
        node_t *clearedProcessNode = remove_head_from_list(state->processesWithMemory);
        clearProcessMemoryPaged(state, clearedProcessNode->data, time);
        free(clearedProcessNode->data->p_name);
        free(clearedProcessNode->data);
        free(clearedProcessNode);
    }

    // allocate the memory
    int toBeAllocated;
    if (state->freePages < requiredPages) {
        toBeAllocated = state->freePages;
    } else {
        toBeAllocated = requiredPages;
    }
    int *allocatedFrames = malloc(requiredPages * sizeof(int));
    for (int i = 0; toBeAllocated > 0; i++) {
        if (state->pageFrames[i] == NULL) {
            state->pageFrames[i] = process->p_name;
            allocatedFrames[requiredPages - toBeAllocated--] = i;
        }
    }
    state->freePages -= requiredPages;
    add_to_list(state->processesWithMemory, -1, process->p_name, -1, process->memory_requirement);
    char *array = stringOfIntArray(allocatedFrames, requiredPages);
    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->p_name, process->service_time, 100 - (100 * state->freePages) / PAGE_COUNT, array);
    free(array);
    free(allocatedFrames);

    return true;
}