#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct for pageFrames which contain pNames and free pages, as well as the list used for memory. 
struct virtualPagedMemoryState {
    char **pageFrames; // array to hold page frames
    // the head of this list is the least recently used
    list_t *processesWithMemory;
    int freePages;
    int mem; // total size of memory 
};

// setup the virtual memory 
void *intialiseMemoryVirtual();

// clear memory allocated to a process
void clearProcessMemoryVirtual(void *, process_t *, int, int);

// allocate memory to a process
bool allocateMemoryVirtual(void *, process_t *, int);

// clean up all used memory
void freeStateVirtual(void *);
