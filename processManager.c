
#include "genericMemoryManaging.h"
#include "linkedList.h"
#include "processManager.h"
#include "roundRobin.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void processing(list_t *process_list, list_t *not_arrived_list, memoryType mem, int quantum) {
    void * memoryManagerData = intialiseMemory(mem);
    node_t *currentProcess = process_list->head;
    node_t *newProcess = NULL;
    int time = 0;
    int currentProcessTime = 0;
    bool currentProcessRunning = false;

    while (process_list->size != 0 || not_arrived_list->size != 0) {
        //TODO: check what type of memoryManager we are using before we decide what we print here
        printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, currentProcess->p_name, currentProcess->service_time);

        // move stuff that hasnt arrived yet to process_list if its ready.
        if (not_arrived_list->size > 0) {
            moveArrivedProcesses(process_list, not_arrived_list, time);
        }
        if (currentProcessRunning == false) {
            while (!allocateMemory(mem,memoryManagerData,newProcess)) {
                appendProcess(process_list, currentProcess);
                removeHead(process_list);
                currentProcess = process_list->head;
                
            }
            currentProcessRunning = true;
        }
        // at start of loop, decrement how much time we have to service it. 
        currentProcess->service_time -= 1;
   
        // if the process is done, remove. 
        if (currentProcess->service_time == 0) {
            clearProcessMemory(mem,memoryManagerData,currentProcess);
            removeHead(process_list);
            currentProcess = process_list->head;
            // this doesn't make sense to ever do. Otherwise, we are pointlessly declaring variables. 
            // removeHead will assign a new list head. 

            // currentProcess = NULL;


            // now, since we have removed a completed task, we say a task is not currently running
            currentProcessRunning = false;
            currentProcessTime = 0;
        }

        // then, we need to check if its hit the quantum limit. If so, kickback.
        if (currentProcessTime == quantum && currentProcess->service_time != 0) {
            appendProcess(process_list, currentProcess);
            removeHead(process_list);
            currentProcess = process_list->head;
            
        }

        /*
        // ? 
        bool newProcessDone = false;

        while (!newProcessDone) {
            // we get the next in the list. Why? 
            newProcess = getNextCurrentProcess(process_list,time);

            // if we cant allocate memory, kick to back. 
            if (!allocateMemory(mem,memoryManagerData,newProcess)) {
                removeHead(process_list);
                appendProcess(process_list, newProcess);
                continue;
            }

            // the current process is the new process
            currentProcess = newProcess;

            // reset this (useless) 
            currentProcessTime = 0;
            newProcessDone = true;
        }
        */
        time += 1;
        currentProcessTime += 1;
    }
}