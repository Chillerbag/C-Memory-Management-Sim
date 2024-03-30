#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

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