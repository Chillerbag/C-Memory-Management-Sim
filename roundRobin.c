#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

int roundRobin(list_t *process_list, int quantum) {
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
                // i think theres a memory leak here, might need to free curNode->p_name
                totalTime += 1;
                timeRunning = 0;

            }
        }
        else {
            totalTime += 1;

        }
    }
}