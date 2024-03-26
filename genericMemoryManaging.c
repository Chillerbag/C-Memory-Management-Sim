//https://stackoverflow.com/questions/6304794/oop-and-interfaces-in-c

#include "genericMemoryManaging.h"
#include "memoryManagerInfinite.h"
#include "memoryManagerContiguous.h"
#include "memoryManagerPaged.h"
#include "memoryManagerVirtual.h"
#include <stdio.h>

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
        case INFINITE: clearProcessMemoryInfinite(state, process);
        case CONTIGUOUS: clearProcessMemoryContiguous(state, process);
        case PAGED: clearProcessMemoryPaged(state, process);
        case VIRTUAL: clearProcessMemoryVirtual(state, process);
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