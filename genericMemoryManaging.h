// https://stackoverflow.com/questions/6304794/oop-and-interfaces-in-c
#ifndef MEMORY_H
#define MEMORY_H
#include "linkedList.h"
#include <stdbool.h>

// enum for each memory management strategy, set in main.c by calling memTypeFromString
typedef enum memoryType {
    INFINITE,
    CONTIGUOUS,
    VIRTUAL,
    PAGED
} memoryType;

// converts read string from command line to memoryType for clarity and use
memoryType memTypeFromString(char *);

// calls the function to initialise memory of the appropriate memory manager
void *intialiseMemory(memoryType me);

// calls the function to clear a process of the appropriate memory manager
void clearProcessMemory(memoryType me, void *state, process_t *process, int time);

// calls the function to allocate a process to memory of the appropriate memory manager
bool allocateMemory(memoryType me, void *state, process_t *process, int time);

// calls function to clean up after a process is done
void cleanMemory(memoryType me, void *state);
#endif
