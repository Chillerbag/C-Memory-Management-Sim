#include "linkedList.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// creates the memory array
void *intialiseMemoryContiguous();

// adds a process to memory or runs one if its already there
bool allocateMemoryContiguous(void *, process_t *, int);

// removes a process from memory
void clearProcessMemoryContiguous(void *, process_t *, int);

// cleans all the memory used by this file at end of program
void freeStateContiguous(void *);

// helper functions to get stats to print
int getAddress(char** memory, char *processName);
int getMemUse(char** memory);