#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>

// nothing needs to be initialised, so return null, for consistencies sake. 
void *intialiseMemoryInfinite() {
    return NULL;
}
// do nothing, memory is infinite, never needs to be cleared.
void clearProcessMemoryInfinite(void *state, process_t *process, int time) {
}
// print the running when a process is allocated to the infinite memory
bool allocateMemoryInfinite(void *state, process_t *process, int time) {
    printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, process->pName, process->remainingTime);
    return true;
}

// no memory needs to be cleaned, as this mallocs nothing
void cleanMemoryInfinite(void * state) {

}