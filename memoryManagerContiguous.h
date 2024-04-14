#include "linkedList.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


void *intialiseMemoryContiguous();
void clearProcessMemoryContiguous(void *, process_t *, int);
bool allocateMemoryContiguous(void *, process_t *, int);
void cleanMemoryContiguous(void *);
int getAddress(char** memory, char *processName);
int getMemUse(char** memory);