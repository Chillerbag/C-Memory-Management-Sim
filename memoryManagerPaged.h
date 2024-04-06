#include "linkedList.h"
#include <stdbool.h>

void *intialiseMemoryPaged();
void clearProcessMemoryPaged(void *, process_t *);
bool allocateMemoryPaged(void *, process_t *);