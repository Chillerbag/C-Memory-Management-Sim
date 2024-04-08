
#include "processManager.h"
#include "genericMemoryManaging.h"
#include "linkedList.h"
#include "memoryManagerContiguous.h"
#include "roundRobin.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processing(list_t *process_list, list_t *not_arrived_list, memoryType mem, int quantum) {
    void *memoryManagerData = intialiseMemory(mem);
    process_t *currentProcess = NULL;
    int time = 0;
    int currentProcessTime = 0;

    while (process_list->size != 0 || not_arrived_list->size != 0) {

        // move stuff that hasnt arrived yet to process_list if its ready.
        if (not_arrived_list->size > 0) {
            moveArrivedProcesses(process_list, not_arrived_list, time);
        }

        // remove process, reappend it if not finished
        if (currentProcess != NULL) {
            removeHead(process_list);
            if (currentProcess->service_time <= 0) {
                clearProcessMemory(mem, memoryManagerData, currentProcess, time);
                printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, currentProcess->p_name, process_list->size);
                free(currentProcess);
            } else {
                appendProcess(process_list, currentProcess);
            }
        }

        process_t *newProcess = getNextCurrentProcess(process_list);

        // don't worry about memory management if nothing is being run or we aren't derunning the process
        if (newProcess != NULL && newProcess != currentProcess) {
            // kickback process if we can't allocate its memory
            while (!allocateMemory(mem, memoryManagerData, newProcess, time)) {
                removeHead(process_list);
                appendProcess(process_list, newProcess);
                newProcess = getNextCurrentProcess(process_list);
            }
        }
        currentProcess = newProcess;

        // Step time
        if (currentProcess != NULL) {
            currentProcess->service_time -= quantum;
            currentProcessTime += quantum;
        }
        time += quantum;
    }
}