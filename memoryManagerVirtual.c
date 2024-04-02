#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>

void *intialiseMemoryVirtual() {
    return NULL;
}
void clearProcessMemoryVirtual(void *state, process_t *process) {
}
bool allocateMemoryVirtual(void *state, process_t *process) {
    return true;
}