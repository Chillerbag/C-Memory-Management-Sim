#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
/*
int roundRobin(list_t *process_list, list_t *not_arrived_list, int quantum) {
    int totalTime = 0;
    int timeRunning = 0;
    node_t *curNode = process_list->head;

    while (curNode != NULL) {
        if (curNode->arrival_time >= totalTime) {
            if (curNode->service_time == 0) {
                node_t *removedNode = remove_from_list(curNode);
                curNode = curNode->next;
                free(removedNode);
                printf("COMPLETED ");
                continue;
            }
            if (timeRunning != quantum) {
                printf("RUNNING ");
                totalTime += 1;
                timeRunning += 1;
                curNode->service_time -= 1;
            }
            else {
                printf("moving to back of queue: READY ");
                // add to tail of list
                add_to_list(process_list, curNode->arrival_time, curNode->p_name, curNode->service_time, curNode->memory_requirement);
                //remove from head
                node_t *removedNode = remove_from_list(curNode);
                curNode = curNode->next;
                free(removedNode);
                totalTime += 1;
                timeRunning = 0;

            }
        }
        else {
            totalTime += 1;

        }
        while (not_arrived_list->head != NULL && not_arrived_list->head->arrival_time <= totalTime) {
            node_t *moveNode = remove_from_list(not_arrived_list->head);
            add_to_list(process_list, moveNode->arrival_time, moveNode->p_name, moveNode->service_time, moveNode->memory_requirement);
            free(moveNode);
        }
    }
}
*/
// Function to get the next process to be executed
// redundant ? 
node_t* getNextCurrentProcess(list_t *process_list, int time) {
    node_t *curNode = process_list->head;
    while (curNode != NULL && curNode->arrival_time > time) {
        curNode = curNode->next;
    }
    return curNode;
}

// Function to remove the head of the process list
void removeHead(list_t *process_list) {
    if (process_list->head != NULL) {
        node_t *removedNode = remove_from_list(process_list);
        free(removedNode);
    }
}

// Function to append a new process to the process list
void appendProcess(list_t *process_list, node_t *newProcess) {
    add_to_list(process_list, newProcess->arrival_time, newProcess->p_name, newProcess->service_time, newProcess->memory_requirement);
    // free(newProcess);
}

// Function to move processes from not_arrived_list to process_list based on the current time
void moveArrivedProcesses(list_t *process_list, list_t *not_arrived_list, int time) {
    while (not_arrived_list->head != NULL && not_arrived_list->head->arrival_time <= time) {
        node_t *moveNode = remove_from_list(not_arrived_list);
        appendProcess(process_list, moveNode);
    }
}