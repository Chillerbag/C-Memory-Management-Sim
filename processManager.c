
#include "processManager.h"
#include "genericMemoryManaging.h"
#include "memoryManagerContiguous.h"
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
    printf("asd\n");

    while (process_list->size != 0 || not_arrived_list->size != 0) {

        // move stuff that hasnt arrived yet to process_list if its ready.
        if (not_arrived_list->size > 0) {
            moveArrivedProcesses(process_list, not_arrived_list, time);
        }

        // remove process, reappend it if not finished
        if (currentProcess != NULL) {
            removeHead(process_list);
            if (currentProcess->service_time <= 0) {
                printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, currentProcess->p_name, process_list->size);
                clearProcessMemory(mem, memoryManagerData, currentProcess);
                free(currentProcess);
            } else {
                appendProcess(process_list, currentProcess);
            }

        }

        process_t *newProcess = NULL;
        while (newProcess == NULL) {
            // Determine the process that runs in this cycle
            newProcess = getNextCurrentProcess(process_list);            
            // don't worry about memory management if nothing or the same thing is being run
            if (newProcess == NULL || newProcess==currentProcess) break;
            // kickback process if we can't allocate its memory
            if (!allocateMemory(mem, memoryManagerData, newProcess)) {
                removeHead(process_list);
                appendProcess(process_list, newProcess);
                newProcess = NULL;
            }
        }
        // //TODO:Move this printing into allocate Memory calls? see paged example, we will need to pass time everywhere
        // if (mem == INFINITE) {
        //     printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, newProcess->p_name, newProcess->service_time);
        // }
        // else if (mem == CONTIGUOUS) {
        //     int memUse = getMemUse(memoryManagerData);
        //     int address = getAddress(memoryManagerData, newProcess->p_name);
        //     printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, newProcess->p_name, newProcess->service_time, memUse, address);
        // }            
        currentProcess = newProcess;
        
        // Step time
        if (currentProcess !=NULL) {
            currentProcess->service_time -= quantum;
            currentProcessTime += quantum;
        }
        time += quantum;
        
    }
}