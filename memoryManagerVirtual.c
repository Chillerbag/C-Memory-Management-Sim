#include "memoryManagerVirtual.h"
#include "memoryManagerPaged.h"
#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// these are constants defined in the spec
#define PAGE_COUNT 512
#define PAGE_SIZE 4


// setup the virtual memory 
void *intialiseMemoryVirtual() {
    // allocate memory for pagedMemoryState struct
    struct pagedMemoryState *state = (struct pagedMemoryState *)malloc(sizeof(struct pagedMemoryState));
    // set up the list of processes currently in memory
    state->processesWithMemory = createList();
    // malloc as many pages as can exist in memory 
    state->pageFrames = malloc(PAGE_COUNT * sizeof(char *));
    for (int i = 0; i < PAGE_COUNT; i++)
        state->pageFrames[i] = NULL;
    state->freePages = PAGE_COUNT; // all pages are free at the start
    return state;
}

// clear memory allocated to a process
void clearProcessMemoryVirtual(void *statev, process_t *process, int time, int memoryReq) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    int j = 0;
    int *clearedFrames = malloc(PAGE_COUNT * sizeof(int)); // Allocate maximum possible frames

    // loop through page frames to clear memory allocated to a process
    for (int i = 0; i < PAGE_COUNT && j < memoryReq; i++) {
        if (state->pageFrames[i] != NULL && !strcmp(process->pName, state->pageFrames[i])) {
            state->pageFrames[i] = NULL;
            clearedFrames[j++] = i;
        }    
    }
    state->freePages += j; // update how many pages were freed

    clearedFrames = realloc(clearedFrames, (memoryReq) * sizeof(int)); // realloc to the correct size after clearing a bunch of memory so we dont print excess
    char *array = stringOfIntArray(clearedFrames, j); // convert to a string 
    printf("%d,EVICTED,evicted-frames=%s\n", time, array);

    // clean up
    free(array);
    free(clearedFrames);

    //update our record of the aount of memory stored if any
    node_t *current = state->processesWithMemory->head;
    while (strcmp(current->data->pName,process->pName)) {
        current = current->next;
    };
    if (current->data->memoryRequirement > PAGE_SIZE * j) {
        current->data->memoryRequirement -= PAGE_SIZE * j;
        return;
    }
    
    // clear the process from our list of allocated processes
    node_t *toBeFreed = removeMatchFromList(state->processesWithMemory, process->pName);
    if (toBeFreed == NULL)
        return;

    // clean up
    free(toBeFreed->data->pName);
    free(toBeFreed->data);
    free(toBeFreed);
}

// allocate memory to a process
bool allocateMemoryVirtual(void *statev, process_t *process, int time) {        
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    int minRequiredPages = 0;
    // Is it already in memory?
    node_t *matchNode = removeMatchFromList(state->processesWithMemory, process->pName);
    // find total amoutn of pages required for a process by dividing by page size 
    int requiredPages = (process->memoryRequirement + 3) / PAGE_SIZE;
    // if the required pages is greater than or equal to 4, then min required is 4. Else, it is requiredPages
    minRequiredPages = requiredPages < PAGE_SIZE ? requiredPages : PAGE_SIZE;

    // if its not already in memory:
    if (NULL != matchNode) {
        // add it to our list of processes we're dealing with
        addProcessToList(state->processesWithMemory, matchNode->data);
        // free the node used to match in memory
        free(matchNode);
        int allocatedPages = requiredPages;
        int *allocatedFrames = malloc(requiredPages * sizeof(int));
        int allocatedIndex = 0;
        // find allocated frames for the processes
        for (int i = 0; allocatedPages > 0 && i < PAGE_COUNT; i++) {
            // check if pageFrame is not null and if we're not already in memory
            if (state->pageFrames[i] != NULL && !strcmp(process->pName, state->pageFrames[i])) {
                // array of where we allocated to is incremented and set to i (where we allocated)
                allocatedFrames[allocatedIndex++] = i;
                --allocatedPages; // attempt to index better to print properly
            }
        }
        char *array = stringOfIntArray(allocatedFrames, allocatedIndex);
        printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->pName, process->remainingTime, 100 - (100 * state->freePages) / PAGE_COUNT, array);
        free(array);
        free(allocatedFrames);
        return true;
    }
    // if we coudnt allocate because of no space, make some
    while (state->freePages < minRequiredPages) {
        node_t *clearedProcessNode = state->processesWithMemory->head;
        // only clear as much memory as is required, either 4 or less than 4 if a process doesnt need >= 4 pages 
        clearProcessMemoryVirtual(state, clearedProcessNode->data, time, (4 - state->freePages));
    }

    // allocate the memory
    int toBeAllocated;
    // if theres less than desired, set amount to be allocated just to be as many as is left
    if (state->freePages < requiredPages) {
        toBeAllocated = state->freePages;
    } else {
        toBeAllocated = requiredPages;
    }

    int *allocatedFrames = malloc(toBeAllocated * sizeof(int));
    int allocatedIndex = 0;
    for (int i = 0; toBeAllocated > 0 && i < PAGE_COUNT; i++) {
        if (state->pageFrames[i] == NULL) {
            state->pageFrames[i] = process->pName;
             allocatedFrames[allocatedIndex++] = i; 
            --toBeAllocated; // attempt to index better
        }
    }
    // add process to list with allocated mem
    int allocatedMemory = process->memoryRequirement <= PAGE_SIZE*state->freePages ? process->memoryRequirement : PAGE_SIZE * (state->freePages > minRequiredPages ? state->freePages : minRequiredPages);
    addToList(state->processesWithMemory, -1, process->pName, -1, allocatedMemory);
    state->freePages -= (requiredPages - toBeAllocated);
    if (state->freePages < 0) {
        state->freePages = 0;
    }
    char *array = stringOfIntArray(allocatedFrames, allocatedIndex);
    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", time, process->pName, process->remainingTime, 100 - (100 * state->freePages) / PAGE_COUNT, array);
    free(array);
    free(allocatedFrames);

    return true;
}

// iterate over memory and free pageframes and also all processesWithMemory before freeing memory itself
void cleanMemoryVirtual(void * state) {
    struct pagedMemoryState *memory = (struct pagedMemoryState *)state;
    for (int i = 0; i < PAGE_COUNT; i++) {
        free(memory->pageFrames[i]);
    }
    free(memory->pageFrames);
    
    freeList(memory->processesWithMemory);
    free(memory);
}

