// https://stackoverflow.com/questions/6304794/oop-and-interfaces-in-c
#ifndef MEMORY_H
#define MEMORY_H
// weird fix. this was fucking up this file before without it
#include "linkedList.h"
#include <stdbool.h>

typedef enum memoryType {
    INFINITE,
    CONTIGUOUS,
    VIRTUAL,
    PAGED
} memoryType;

memoryType memTypeFromString(char *);

void *intialiseMemory(memoryType me);
void clearProcessMemory(memoryType me, void *state, process_t *process, int time);
bool allocateMemory(memoryType me, void *state, process_t *process, int time);
void cleanMemory(memoryType me, void *state);
#endif
