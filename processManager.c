
#include "processManager.h"
#include "genericMemoryManaging.h"
#include "linkedList.h"
#include "memoryManagerContiguous.h"
#include "roundRobin.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processing(list_t *processList, list_t *notArrivedList, memoryType mem, int quantum) {
    //operating variables
    void *memoryManagerData = intialiseMemory(mem);
    process_t *currentProcess = NULL;
    int time = 0;
    // statistics
    int totalProcesses = notArrivedList->size;
    int maxTurnAround = 0;
    float maxOverhead = 0;
    float averageTurnAround = 0;
    float averageOverhead = 0;

    while (processList->size != 0 || notArrivedList->size != 0) {
        // move stuff that hasnt arrived yet to processList if its ready.
        if (notArrivedList->size > 0) {
            moveArrivedProcesses(processList, notArrivedList, time);
        }

        // remove process, reappend it if not finished
        if (currentProcess != NULL) {
            removeHead(processList);
            if (currentProcess->remainingTime <= 0) {
                clearProcessMemory(mem, memoryManagerData, currentProcess, time);
                printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, currentProcess->pName, processList->size);
                //update statistics
                int turnAround = time - currentProcess->arrivalTime;
                float overhead = (float)turnAround / currentProcess->serviceTime;
                averageTurnAround += turnAround;
                averageOverhead += overhead;
                if (turnAround > maxTurnAround) maxTurnAround = turnAround;
                if (overhead > maxOverhead) maxOverhead = overhead;
                
                free(currentProcess->pName);
                free(currentProcess);
            } else {
                appendProcess(processList, currentProcess);
            }
        }

        process_t *newProcess = getNextCurrentProcess(processList);

        // don't worry about memory management if nothing is being run or we aren't derunning the process
        if (newProcess != NULL && newProcess != currentProcess) {
            // kickback process if we can't allocate its memory
            while (!allocateMemory(mem, memoryManagerData, newProcess, time)) {
                removeHead(processList);
                appendProcess(processList, newProcess);
                newProcess = getNextCurrentProcess(processList);
            }
        }
        currentProcess = newProcess;

        // Step time
        if (currentProcess != NULL) {
            currentProcess->remainingTime -= quantum;
        }
        time += quantum;
    }
    time -= quantum;
    // TODO: clean up function
    cleanMemory(mem, memoryManagerData);

    averageTurnAround /= totalProcesses;
    averageOverhead /= totalProcesses;
    printf("Turnaround time %.0f\n", averageTurnAround);
    printf("Time overhead %.2f %.2f\n", maxOverhead, averageOverhead);
    printf("Makespan %d\n", time);
}