
#include "processManager.h"
#include "genericMemoryManaging.h"
#include "linkedList.h"
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

        // if the process is done, remove.
        if (currentProcess != NULL && currentProcessTime == quantum) {
            if (currentProcess->service_time == 0) {
                removeHead(process_list);
                clearProcessMemory(mem, memoryManagerData, currentProcess);
                printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, currentProcess->p_name, process_list->size);
                free(currentProcess);
            } else {
                appendProcess(process_list, currentProcess);
                removeHead(process_list);
            }

            currentProcessTime = 0;
        }

        process_t *newProcess = NULL;
        while (newProcess == NULL) {
            // Determine the process that runs in this cycle
            newProcess = getNextCurrentProcess(process_list, currentProcessTime, quantum);            
            // don't worry about memory management if nothing or the same thing is being run
            if (newProcess == NULL || newProcess==currentProcess) break;
            if (!allocateMemory(mem, memoryManagerData, newProcess)) {
                appendProcess(process_list, currentProcess);
                removeHead(process_list);
                newProcess = NULL;
            }
            currentProcessTime = 0;
            //TODO:Move this printing into allocate Memory calls? see paged example, we will need to pass time everywhere
            if (mem == INFINITE) {
                printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, newProcess->p_name, newProcess->service_time);
            }
            else if (mem == CONTIGUOUS) {
                //int memUse = getMemUse(memoryManagerData);
                //int address = getAddress(memoryManagerData, newProcess->p_name);
                printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%\n", time, newProcess->p_name, newProcess->service_time);
            }            
        }
        currentProcess = newProcess;
        // Step time
        if (currentProcess !=NULL) {
            if (currentProcess->service_time != 0) {
                currentProcess->service_time -= 1;
            }
        }
        

        time += 1;
        currentProcessTime += 1;
    }
}