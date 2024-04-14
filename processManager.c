
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
    //operating variables
    void *memoryManagerData = intialiseMemory(mem);
    process_t *currentProcess = NULL;
    int time = 0;
    // statistics
    int totalProcesses = not_arrived_list->size;
    int maxTurnAround = 0;
    float maxOverhead = 0;
    float averageTurnAround = 0;
    float averageOverhead = 0;

    while (process_list->size != 0 || not_arrived_list->size != 0) {
        // move stuff that hasnt arrived yet to process_list if its ready.
        if (not_arrived_list->size > 0) {
            moveArrivedProcesses(process_list, not_arrived_list, time);
        }

        // remove process, reappend it if not finished
        if (currentProcess != NULL) {
            removeHead(process_list);
            if (currentProcess->remainingTime <= 0) {
                clearProcessMemory(mem, memoryManagerData, currentProcess, time);
                printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, currentProcess->p_name, process_list->size);
                //update statistics
                int turnAround = time - currentProcess->arrival_time;
                float overhead = turnAround / currentProcess->service_time;
                averageTurnAround += turnAround;
                averageOverhead += overhead;
                if (turnAround > maxTurnAround) maxTurnAround = turnAround;
                if (overhead > maxOverhead) maxOverhead = overhead;
                
                free(currentProcess->p_name);
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
            currentProcess->remainingTime -= quantum;
        }
        time += quantum;
    }
    // TODO: clean up function
    cleanMemory(mem, memoryManagerData);

    averageTurnAround /= totalProcesses;
    averageOverhead /= totalProcesses;
    printf("Turnaround time %f\n", averageTurnAround);
    printf("Time overhead %.2f %.2f\n", maxOverhead, averageOverhead);
    printf("Makespan %d\n", time);
}