#include "linkedList.h"
#include "memoryManagerContiguous.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MEMORY_SIZE 2048

void* intialiseMemoryContiguous() {
    char** memory = malloc(MEMORY_SIZE * sizeof(char*));

    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = NULL;
    }

    return memory;
}

void clearProcessMemoryContiguous(void* state, process_t* process) {
    char** memory = (char**)state;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] != NULL && strcmp(memory[i], process->p_name) == 0) {
            free(memory[i]); 
            memory[i] = NULL;
        }
    }
}

bool allocateMemoryContiguous(void* state, process_t* process) {
    char** memory = (char**)state;
    int startIndex = -1; 
    int emptyCounter = 0;
    //printf("now allocating: %s\n", process->p_name);

    for (int x = 0; x < MEMORY_SIZE; x++) {
    // check if we already exist in memory 
        if (memory[x] != NULL && strcmp(memory[x], process->p_name) == 0) {
            //printf("hit, %s\n", process->p_name);
            return true;
        }
    }

    for (int i = 0; i < MEMORY_SIZE; i++) {       
        // check if we are gonna overflow memory:
        //if ((i + process->memory_requirement) > (MEMORY_SIZE - 1)) {
        //    printf("process will overflow: %s\n", process->p_name);
        //    return false;
        //}

        if (memory[i] == NULL) {
            if (startIndex == -1) {
                startIndex = i;
            }
            emptyCounter++;
            if (emptyCounter == process->memory_requirement) {
            // Make the process occupy memory
                for (int j = startIndex; j <= process->memory_requirement + startIndex - 1; j++) {
                    //printf("%d\n", j);
                    memory[j] = strdup(process->p_name);
                    //printf("%s\n", memory[j]);
                }
                return true;
            }
        } else {
            startIndex = -1;
            emptyCounter = 0;
        }
    }
    return false;
}

int getMemUse(void* state) {
    int memCount = 0;
    int memUse = 0;
    char** memory = (char**)state;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] != NULL) {
            memCount += 1;
        }
    }
    memUse = (memCount * 100) / MEMORY_SIZE; 
    return memUse;
}

int getAddress(void* state, char* processName) {
    char** memory = (char**)state;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] != NULL && strcmp(memory[i], processName) == 0) {
            return i;
        }
    }
    return -1; 
}
