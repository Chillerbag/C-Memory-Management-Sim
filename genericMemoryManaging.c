//https://stackoverflow.com/questions/6304794/oop-and-interfaces-in-c

#include "genericMemoryManaging.h"
#include "memoryManagerInfinite.h"
#include "memoryManagerContiguous.h"
#include "memoryManagerPaged.h"
#include "memoryManagerVirtual.h"
#include <stdio.h>
#include <string.h>

void *intialiseMemory(memoryType me) {
    switch(me) {
        case INFINITE: return intialiseMemoryInfinite();
        case CONTIGUOUS: return intialiseMemoryContiguous();
        case PAGED: return intialiseMemoryPaged();
        case VIRTUAL: return intialiseMemoryVirtual();
        default: return NULL;
    }
}

void clearProcessMemory(memoryType me, void *state, process_t *process, int time) {
    switch(me) {
        case INFINITE: clearProcessMemoryInfinite(state, process, time); break;
        case CONTIGUOUS: clearProcessMemoryContiguous(state, process, time); break;
        case PAGED: clearProcessMemoryPaged(state, process, time); break;
        case VIRTUAL: clearProcessMemoryVirtual(state, process, time, 4 * (process->memory_requirement)); break;
    }
}

bool allocateMemory(memoryType me, void *state, process_t *process, int time) {
    switch(me) {
        case INFINITE: return allocateMemoryInfinite(state, process, time);
        case CONTIGUOUS: return allocateMemoryContiguous(state, process, time);
        case PAGED: return allocateMemoryPaged(state, process, time);
        case VIRTUAL: return allocateMemoryVirtual(state, process, time);
        default: return true;
    }
}

void cleanMemory(memoryType me, void *state) {
    switch(me) {
        case INFINITE: return cleanMemoryInfinite(state);
        case CONTIGUOUS: return cleanMemoryContiguous(state);
        case PAGED: return cleanMemoryPaged(state);
        case VIRTUAL: return cleanMemoryVirtual(state);
    }
}

memoryType memTypeFromString(char * str) {
    if (!strcmp(str,"infinite")) return INFINITE;
    if (!strcmp(str,"first-fit")) return CONTIGUOUS;
    if (!strcmp(str,"paged")) return PAGED;
    if (!strcmp(str,"virtual")) return VIRTUAL;
    //TODO:throw error
    return INFINITE;
}