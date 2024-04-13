#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *intialiseMemoryVirtual();
void clearProcessMemoryVirtual(void *, process_t *, int, int);
bool allocateMemoryVirtual(void *, process_t *, int);
