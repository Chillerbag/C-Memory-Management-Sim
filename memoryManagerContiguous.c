#include "linkedList.h"
#include "memoryManagerContiguous.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MEMORY_SIZE 2048

void* intialiseMemoryContiguous() {
    // array of p_names, so we use char ** as p_names are dynamically allocated
    char** memory = malloc(MEMORY_SIZE * sizeof(char*));

    // initialise as NULL
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = NULL;
    }

    return memory;
}

bool allocateMemoryContiguous(void* state, process_t* process, int time) {
    // typecast for use
    char** memory = (char**)state;
    int startIndex = -1; 
    int emptyCounter = 0;

    // iterate over whole array 
    for (int x = 0; x < MEMORY_SIZE; x++) {
    // check if we already exist in memory, if we do, its already allocated. Print and return early 
        if (memory[x] != NULL && strcmp(memory[x], process->p_name) == 0) {
            int memUse = getMemUse(memory);
            int address = getAddress(memory, process->p_name);
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, process->p_name, process->remainingTime, memUse, address);
            return true;
        }
    }
    // if we don't exist in memory:
    for (int i = 0; i < MEMORY_SIZE; i++) { 
        // we found an empty space in memory      
        if (memory[i] == NULL) {
            // placeholder to start finding where we start from 
            if (startIndex == -1) {
                startIndex = i;
            }
            // start counting to see if we can fit process
            emptyCounter++;
            // if we can fit
            if (emptyCounter == process->memory_requirement) {
            // Make the process occupy memory by setting the memory to the p_name from the start_index to the size of the process
                for (int j = startIndex; j <= process->memory_requirement + startIndex - 1; j++) {
                    memory[j] = strdup(process->p_name);
                }
                // print stats
                int memUse = getMemUse(memory);
                int address = getAddress(memory, process->p_name);
                printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, process->p_name, process->remainingTime, memUse, address);
                return true;
            }
        } else {
            // if we hit a spot where its not empty, reset start index and empty count
            startIndex = -1;
            emptyCounter = 0;
        }
    }
    return false;
}

void clearProcessMemoryContiguous(void* state, process_t* process, int time) {
    // typecast for use
    char** memory = (char**)state;

    // iterate over whole array to remove a process
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // check if what is in the char array is the same as the process we are trying to clear
        if (memory[i] != NULL && strcmp(memory[i], process->p_name) == 0) {
            // if it is, we clear
            free(memory[i]); 
            memory[i] = NULL;
        }
    }
}

// cleanMemoryContiguous - iterate over the memory array, freeing each char * until we get to the end, then free the array itself. 
void cleanMemoryContiguous(void * state) {
    char** memory = (char**)state;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        free(memory[i]);
    }
    free(memory);
}

// --------------------------- helper functions ---------------------------

// return how much memory is currently being used as a percentage 
int getMemUse(char** memory) {
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

// return the starting address of the process (index in the memory array)
int getAddress(char** memory, char* processName) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // iterate until we find the process we're looking for
        if (memory[i] != NULL && strcmp(memory[i], processName) == 0) {
            return i;
        }
    }
    return -1; 
}

