#include "linkedList.h"
#include "roundRobin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this file contains a set of functions for doing round robin processes in processManager.c

// Do round robin scheduling given the current process run time and a set quantum
process_t *getNextCurrentProcess(list_t *processList) {
    if (processList==NULL || processList->head==NULL) return NULL;
    return processList->head->data;
}

// Remove the head of the process list
process_t *removeHead(list_t *processList) {
    if (processList->head == NULL) return NULL;
    node_t *removedNode = removeHeadFromList(processList);
    process_t* removedProcess = removedNode->data;
    free(removedNode);
    return removedProcess;
}

// Append a new process to the process list
void appendProcess(list_t *processList, process_t *newProcess) {
    addProcessToList(processList, newProcess);
}

// Move processes from notArrivedList to processList based on the current time
void moveArrivedProcesses(list_t *processList, list_t *notArrivedList, int time) {
    while (notArrivedList->head != NULL && notArrivedList->head->data->arrivalTime <= time) {
        node_t *moveNode = removeHeadFromList(notArrivedList);
        appendProcess(processList, moveNode->data);
        free(moveNode);
    }
}