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

void clearProcessMemory(memoryType me, void *state, node_t *process) {
    switch(me) {
        case INFINITE: clearProcessMemoryInfinite(state, process); break;
        case CONTIGUOUS: clearProcessMemoryContiguous(state, process); break;
        case PAGED: clearProcessMemoryPaged(state, process); break;
        case VIRTUAL: clearProcessMemoryVirtual(state, process); break;
    }

}

bool allocateMemory(memoryType me, void *state, node_t *process) {
    switch(me) {
        case INFINITE: return allocateMemoryInfinite(state, process);
        case CONTIGUOUS: return allocateMemoryContiguous(state, process);
        case PAGED: return allocateMemoryPaged(state, process);
        case VIRTUAL: return allocateMemoryVirtual(state, process);
        default: return true;
    }
}
memoryType memTypeFromString(char * str) {
    if (strcmp(str,"infinite"))
    {
        return INFINITE;
    }
    if (strcmp(str,"first-fit"))
    {
        return CONTIGUOUS;
    }
    if (strcmp(str,"paged"))
    {
        return PAGED;
    }
    
    if (strcmp(str,"virtual"))
    {
        return VIRTUAL;
    }
    //TODO:throw error
    return INFINITE;
}