#include "memoryManagerVirtual.h"
#include "memoryManagerPaged.h"
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


void clearProcessMemoryVirtual(void *statev, process_t *process, int time, int memoryReq) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    int j = 0;
    int *clearedFrames = malloc(PAGE_COUNT * sizeof(int)); // Allocate maximum possible frames

    for (int i = 0; i < PAGE_COUNT; i++) {
        if (state->pageFrames[i] != NULL && !strcmp(process->p_name, state->pageFrames[i])) {
            state->pageFrames[i] = NULL;
            clearedFrames[j++] = i;
            state->freePages++; // attempt to fix issues around RUNNING print
        }    
    }
    j--;

    clearedFrames = realloc(clearedFrames, (memoryReq) * sizeof(int)); 
    char *array = stringOfIntArray(clearedFrames, j > memoryReq ? memoryReq : j);
    printf("%d,EVICTED,evicted-frames=%s\n", time, array);
    free(array);
    free(clearedFrames);

    node_t *toBeFreed = remove_match_from_list(state->processesWithMemory, process->p_name);
    if (toBeFreed == NULL)
        return;

    free(toBeFreed->data->p_name);
    free(toBeFreed->data);
    free(toBeFreed);
}


bool allocateMemoryVirtual(void *statev, process_t *process, int time) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    int minRequiredPages = 0;
    // Is it already in memory?
    node_t *matchNode = remove_match_from_list(state->processesWithMemory, process->p_name);
    int requiredPages = (process->memory_requirement + 3) / 4;
    minRequiredPages = requiredPages < 4 ? requiredPages : 4;

    if (NULL != matchNode) {
        add_process_to_list(state->processesWithMemory, matchNode->data);
        free(matchNode);
        int allocatedPages = requiredPages;
        int *allocatedFrames = malloc(requiredPages * sizeof(int));
        int allocatedIndex = 0;
        for (int i = 0; allocatedPages > 0 && i < PAGE_COUNT; i++) {
            if (state->pageFrames[i] != NULL && !strcmp(process->p_name, state->pageFrames[i])) {
                allocatedFrames[allocatedIndex++] = i;
                --allocatedPages; // attempt to index better to print properly
            }
        }
        char *array = stringOfIntArray(allocatedFrames, allocatedIndex);
        printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->p_name, process->service_time, 100 - (100 * state->freePages) / PAGE_COUNT, array);
        free(array);
        free(allocatedFrames);
        return true;
    }
    // if we dont have space, make some
    while (state->freePages < minRequiredPages) {
        node_t *clearedProcessNode = remove_head_from_list(state->processesWithMemory);
        clearProcessMemoryVirtual(state, clearedProcessNode->data, time, (4 - state->freePages));
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
    int *allocatedFrames = malloc(toBeAllocated * sizeof(int));
    int allocatedIndex = 0;
    for (int i = 0; toBeAllocated > 0 && i < PAGE_COUNT; i++) {
        if (state->pageFrames[i] == NULL) {
            state->pageFrames[i] = process->p_name;
             allocatedFrames[allocatedIndex++] = i; 
            --toBeAllocated; // attempt to index better
        }
    }
    add_to_list(state->processesWithMemory, -1, process->p_name, -1, process->memory_requirement);
    state->freePages -= (requiredPages - toBeAllocated);
    if (state->freePages < 0) {
        state->freePages = 0;
    }
    char *array = stringOfIntArray(allocatedFrames, allocatedIndex); // changed to allocatedIndex to try and print the correct length
    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->p_name, process->service_time, 100 - (100 * state->freePages) / PAGE_COUNT, array);
    free(array);
    free(allocatedFrames);

    return true;
}

