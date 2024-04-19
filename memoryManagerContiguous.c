#include "memoryManagerContiguous.h"
#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 2048

void *intialiseMemoryContiguous() {
    // array of pNames, so we use char ** as pNames are dynamically allocated
    char **memory = malloc(MEMORY_SIZE * sizeof(char *));

    // initialise as NULL
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = NULL;
    }

    return memory;
}

bool allocateMemoryContiguous(void *state, process_t *process, int time) {
    // typecast for use
    char **memory = (char **)state;
    int emptyCounter = 0;

    // iterate over whole array
    for (int x = 0; x < MEMORY_SIZE; x++) {
        // check if we already exist in memory, if we do, its already allocated. Print and return early
        if (memory[x] != NULL && strcmp(memory[x], process->pName) == 0) {
            int memUse = getMemUse(memory);
            int address = x;
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, process->pName, process->remainingTime, memUse, address);
            return true;
        }
    }
    // if we don't exist in memory:
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // if we hit a spot where its not empty, progress reset empty count
        if (memory[i] != NULL) {
            emptyCounter = 0;
            continue;
        }
        // start counting to see if we can fit process
        emptyCounter++;
        // if we can fit
        if (emptyCounter == process->memoryRequirement) {
            int addressStart = i + 1 - process->memoryRequirement;
            // Make the process occupy memory by setting the memory to the pName from the start_index to the size of the process
            for (int j = i + 1 - process->memoryRequirement; j <= i; j++) {
                memory[j] = strdup(process->pName);
            }
            // print stats
            int memUse = getMemUse(memory);
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, process->pName, process->remainingTime, memUse, addressStart);
            return true;
        }
    }
    return false;
}

void clearProcessMemoryContiguous(void *state, process_t *process, int time) {
    // typecast for use
    char **memory = (char **)state;

    // iterate over whole array to remove a process
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // check if what is in the char array is the same as the process we are trying to clear
        if (memory[i] != NULL && strcmp(memory[i], process->pName) == 0) {
            // if it is, we clear
            free(memory[i]);
            memory[i] = NULL;
        }
    }
}

// freeStateContiguous - iterate over the memory array, freeing each char * until we get to the end, then free the array itself.
void freeStateContiguous(void *state) {
    char **memory = (char **)state;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        free(memory[i]);
    }
    free(memory);
}

// --------------------------- helper functions ---------------------------

// return how much memory is currently being used as a percentage
int getMemUse(char **memory) {
    int memCount = 0;
    int memUse = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] != NULL) {
            memCount += 1;
        }
    }
    memUse = (memCount * 100) / MEMORY_SIZE;
    // for rounding purposes
    if ((memCount * 100) % MEMORY_SIZE != 0) {
        memUse += 1;
    }
    return memUse;
}