#include "linkedList.h"
#include <stdbool.h>

// these are here for consistency, even though their functionality is limited.

// returns null as memory is infinite
void *intialiseMemoryInfinite();

// does nothing, no memory needs to be cleared
void clearProcessMemoryInfinite(void *, process_t *, int);

// prints what process is running
bool allocateMemoryInfinite(void *, process_t *, int);

// does nothing, as real memory is not used by this program
void cleanMemoryInfinite(void *);
