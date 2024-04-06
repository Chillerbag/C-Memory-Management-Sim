#include "linkedList.h"
#include "roundRobin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Do round robin scheduling given the current process run time and a set quantum
process_t *getNextCurrentProcess(list_t *process_list, int currentProcessTime, int quantum) {
    if (process_list==NULL || process_list->head==NULL) return NULL;
        
    process_t *currentProcess = process_list->head->data;
    // What process runs? then, we need to check if its hit the quantum limit. If so, kickback.
    if (currentProcessTime >= quantum) {
        appendProcess(process_list, currentProcess);
        removeHead(process_list);
        currentProcess = process_list->head->data;
    }
    return currentProcess;
}

// Remove the head of the process list
process_t *removeHead(list_t *process_list) {
    if (process_list->head == NULL) return NULL;
    node_t *removedNode = remove_head_from_list(process_list);
    process_t* removedProcess = removedNode->data;
    free(removedNode);
    return removedProcess;
}

// Append a new process to the process list
void appendProcess(list_t *process_list, process_t *newProcess) {
    add_process_to_list(process_list, newProcess);
    // free(newProcess);
}

// Move processes from not_arrived_list to process_list based on the current time
void moveArrivedProcesses(list_t *process_list, list_t *not_arrived_list, int time) {
    while (not_arrived_list->head != NULL && not_arrived_list->head->data->arrival_time <= time) {
        node_t *moveNode = remove_head_from_list(not_arrived_list);
        appendProcess(process_list, moveNode->data);
        free(moveNode);
    }
}