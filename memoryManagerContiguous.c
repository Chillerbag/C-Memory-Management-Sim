#include "linkedList.h"
#include "memoryManagerContiguous.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MEMORY_SIZE 2048

struct memoryNode {
    char * p_name;
    int startIndex;
    int size;
    struct memoryNode *next;
};

struct continuousMemory {
    struct memoryNode *allocatedMemory;
    char memory[MEMORY_SIZE]; 
};

void *intialiseMemoryContiguous() {
    struct continuousMemory *memoryManager = malloc(sizeof(struct continuousMemory));
    memset(memoryManager->memory, 0, MEMORY_SIZE); // initialize memory array to 0
    return memoryManager;
}

// TODO debug. 
void clearProcessMemoryContiguous(void *state, process_t *process) {
    struct continuousMemory *memoryManager = (struct continuousMemory *)state;
    struct memoryNode *currentNode = memoryManager->allocatedMemory;
    struct memoryNode *prevNode = NULL;

    // find the process in the allocatedMemory list
    while (currentNode != NULL && strcmp(currentNode->p_name, process->p_name) != 0) {
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    // if the process is found, remove it from the list and clear the memory
    if (currentNode != NULL) {
        if (prevNode != NULL) {
            prevNode->next = currentNode->next;
        } else {
            memoryManager->allocatedMemory = currentNode->next;
        }

        // clear the memory
        memset(memoryManager->memory + currentNode->startIndex, 0, currentNode->size);
        free(currentNode);
    }
}

// TODO: this is def broken. Could be to do with p_name memory allocation, though I doubt it. 
bool allocateMemoryContiguous(void *state, process_t *process) {
    struct continuousMemory *memoryManager = (struct continuousMemory *)state;
    int freeCount = 0;
    int startIndex = -1;

    // find a block of memory that can fit the process
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memoryManager->memory[i] == 0) {
            if (freeCount == 0) {
                startIndex = i;
            }
            freeCount++;
            if (freeCount == process->memory_requirement) {
                break;
            }
        } else {
            freeCount = 0;
        }
    }

    // if a suitable block is found, allocate it and update the allocatedMemory list
    if (freeCount == process->memory_requirement) {
        struct memoryNode *newNode = malloc(sizeof(struct memoryNode));
        newNode->startIndex = startIndex;
        newNode->size = process->memory_requirement;
        newNode->next = memoryManager->allocatedMemory;
        memoryManager->allocatedMemory = newNode;

        // mark the memory blocks as occupied
        memset(memoryManager->memory + startIndex, 1, process->memory_requirement);

        return true;
    }

    return false;
}


// TODO: fix these. they cause segfaults for some reason. 
int getMemUse(void * state) {
    struct continuousMemory *memoryManager = (struct continuousMemory *)state;
    struct memoryNode *currentNode = memoryManager->allocatedMemory;
    int total = 0;

    while (currentNode != NULL) {
        total += currentNode->size; 
        currentNode = currentNode->next;
    }
    return (total / MEMORY_SIZE) * 100;
}

int getAddress(void *state, char *processName) {
    struct continuousMemory *memoryManager = (struct continuousMemory *)state;
    struct memoryNode *currentNode = memoryManager->allocatedMemory;

    while (currentNode != NULL) {
        if (strcmp(currentNode->p_name, processName) == 0) {
            return currentNode->startIndex;
        }
        currentNode = currentNode->next;
    }

    return -1;
}