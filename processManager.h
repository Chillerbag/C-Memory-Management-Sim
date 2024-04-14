
#include "genericMemoryManaging.h"
#include "linkedList.h"
#include "roundRobin.h"
#ifndef PM_H
#define PM_H

void processing(list_t *processList, list_t *notArrivedList, memoryType mem, int quantum);

#endif