#ifndef RR_H
#define RR_H
#include "linkedList.h"

// int roundRobin(list_t *process_list, list_t *not_arrived_list, int quantum);

process_t *getNextCurrentProcess(list_t *process_list, int currentProcessTime, int quantum);
process_t *removeHead(list_t *process_list);
void appendProcess(list_t *process_list, process_t *newProcess);
void moveArrivedProcesses(list_t *process_list, list_t *not_arrived_list, int time);
#endif