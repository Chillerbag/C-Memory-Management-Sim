#include "linkedList.h"
#include <stdbool.h>

void *intialiseMemoryPaged();
void clearProcessMemoryPaged(void *, process_t *, int);
bool allocateMemoryPaged(void *, process_t *, int);
char *stringOfIntArray(int* array,int size);