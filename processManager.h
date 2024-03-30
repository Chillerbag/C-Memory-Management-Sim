
#include "genericMemoryManaging.h"
#include "linkedList.h"
#include "roundRobin.h"
#ifndef PM_H
#define PM_H

void processing(list_t *process_list, list_t *not_arrived_list, memoryType mem, int quantum);

#endif