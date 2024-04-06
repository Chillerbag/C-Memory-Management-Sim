#include "linkedList.h"
#include <stdbool.h>

void *intialiseMemoryVirtual();
void clearProcessMemoryVirtual(void *, process_t *);
bool allocateMemoryVirtual(void *, process_t *);