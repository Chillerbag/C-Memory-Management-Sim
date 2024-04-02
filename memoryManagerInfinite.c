#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>

void *intialiseMemoryInfinite() {
    return NULL;
}
void clearProcessMemoryInfinite(void *state, process_t *process) {
}
bool allocateMemoryInfinite(void *state, process_t *process) {
    return true;
}