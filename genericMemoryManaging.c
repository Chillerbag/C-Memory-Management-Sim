//https://stackoverflow.com/questions/6304794/oop-and-interfaces-in-c

#include "genericMemoryManaging.h"
#include "memoryManagerInfinite.h"
#include "memoryManagerContiguous.h"
#include "memoryManagerPaged.h"
#include "memoryManagerVirtual.h"
#include <stdio.h>
#include <string.h>

// this file is used to use the correct memory management method whilst still calling a generic method in processManager.c
// all of these functions are called in processManager.c or main.c and they return the result of the specific memory management practice
// that is being used



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
        case VIRTUAL: clearProcessMemoryVirtual(state, process, time, 4 * (process->memoryRequirement)); break;
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

void freeState(memoryType me, void *state) {
    switch(me) {
        case INFINITE: return freeStateInfinite(state);
        case CONTIGUOUS: return freeStateContiguous(state);
        case PAGED: return freeStatePaged(state);
        case VIRTUAL: return freeStateVirtual(state);
    }
}

// converts the commandline string to a enum for ease of use 
memoryType memTypeFromString(char * str) {
    if (!strcmp(str,"infinite")) return INFINITE;
    if (!strcmp(str,"first-fit")) return CONTIGUOUS;
    if (!strcmp(str,"paged")) return PAGED;
    if (!strcmp(str,"virtual")) return VIRTUAL;
    return INFINITE;
}