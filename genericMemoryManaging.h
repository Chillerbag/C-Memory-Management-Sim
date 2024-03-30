//https://stackoverflow.com/questions/6304794/oop-and-interfaces-in-c
#ifndef MEMORY_H
#define MEMORY_H
// weird fix. this was fucking up this file before without it
#include <stdbool.h>
#include "linkedList.h"

typedef enum memoryType {
    INFINITE,
    CONTIGUOUS,
    VIRTUAL,
    PAGED
} memoryType;

memoryType memTypeFromString(char *);

void *intialiseMemory(memoryType me);
void clearProcessMemory(memoryType me, void *state, node_t *process);
bool allocateMemory(memoryType me, void *state, node_t *process);
#endif

