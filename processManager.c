
#include "genericMemoryManaging.h"
#include "linkedList.h"
#include "processManager.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void processing(list_t *process_list, memoryType mem) {
    node_t *currentProcess = process_list->head;
    node_t *newProcess = NULL;
    int time = 0;
    int currentProcessTime = 0;
    // import process data file

    while (process_list->size != 0) {
        free(remove_from_list(process_list));
        // get new processes for this time from imported file
        // call RR(new process data pls update): for each process append(process)

        // Clear finished process
        if (currentProcessTime >= currentProcess->service_time) {
            // call RR(head finished): removeHead(currentProcess)
            // call MM: clearProcessMemory(mem,currentProcess)
            currentProcess = NULL;
        }

        bool newProcessDone = false;
        while (!newProcessDone) {

            // call RR(update scheduling): getNextCurrentProcess(currentProcess,time)
            node_t *returnValue = process_list->head;
            node_t *newProcess = returnValue;

            // call MM(allocateMemory for the new process): allocateMemory(mem,newProcess)
            bool isSuccessful = true;
            if (!isSuccessful /*in task 2 it can be not successful*/) {
                // call RR(kick process to back of queue): removeHead(newProcess); append(newProcess)
                continue;
            }
            currentProcess = newProcess;
            currentProcessTime = 0;
            newProcessDone = true;
        }
        time += 1;
        currentProcessTime += 1;
    }
}