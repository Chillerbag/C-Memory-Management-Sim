#ifndef RR_H
#define RR_H
#include "linkedList.h"

// get the next process to be allocated
process_t *getNextCurrentProcess(list_t *processList);

// remove a element when it has been deallocated
process_t *removeHead(list_t *processList);

// adding new processes / processes which used their quantum to the end of the LL 
void appendProcess(list_t *processList, process_t *newProcess);

// moving proceses which have reached arrival time to the processing list
void moveArrivedProcesses(list_t *processList, list_t *notArrivedList, int time);
#endif