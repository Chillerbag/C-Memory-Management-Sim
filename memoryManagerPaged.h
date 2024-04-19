#include "linkedList.h"
#include <stdbool.h>

void *intialiseMemoryPaged();
void clearProcessMemoryPaged(void *, process_t *, int);
bool allocateMemoryPaged(void *, process_t *, int);
char *stringOfIntArray(int* array,int size);
void freeStatePaged(void *);

struct pagedMemoryState {
    char **pageFrames;
    // the head of this list is the least recently used process
    list_t *processesWithMemory;
    int freePages;
};