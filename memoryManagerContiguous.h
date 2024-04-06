#include "linkedList.h"
#include <stdbool.h>

void *intialiseMemoryContiguous();
void clearProcessMemoryContiguous(void *, process_t *);
bool allocateMemoryContiguous(void *, process_t *);
int getAddress(void *state, char *processName);
int getMemUse(void * state);