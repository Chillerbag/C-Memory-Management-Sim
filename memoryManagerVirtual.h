#include "linkedList.h"
#include <stdbool.h>

void *intialiseMemoryVirtual();
void clearProcessMemoryVirtual(void *, process_t *, int);
bool allocateMemoryVirtual(void *, process_t *, int);