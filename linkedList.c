#include "linkedList.h"
#include <stdlib.h>
#include <string.h>

// creates a list by mallocing space and initing params. 
list_t *createList() {
    list_t *newList = (list_t *)malloc(sizeof(list_t));
    newList->size = 0;
    newList->head = NULL;
    return newList;
}

// create a node 
void addToList(list_t *list, int arrivalTime, const char *pName, int serviceTime, int memoryRequirement) {
    process_t *newData = (process_t *)malloc(sizeof(process_t));
    newData->arrivalTime = arrivalTime;
    // dynamically allocated string, so use strdup instead of an ugly malloc
    newData->pName = strdup(pName);
    newData->serviceTime = serviceTime;
    newData->remainingTime = serviceTime;
    newData->memoryRequirement = memoryRequirement;
    // add it to a provided list
    addProcessToList(list, newData);
}

// add a provided node to a list
void addProcessToList(list_t *list, process_t *process) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->data = process;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    }

    else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++;
}

// IMPORTANT: ONCE YOU USE THIS FUNCTION, YOU MUST FREE THE RETURNED NODE
// remove head from the list (process we're dealing with, to call if process finishes)
node_t *removeHeadFromList(list_t *list) {
    if (list->size == 0) {
        return NULL;
    }
    node_t *nodeToRemove = list->head;
    list->head = nodeToRemove->next;
    list->size--;
    return nodeToRemove;
}
// IMPORTANT: ONCE YOU USE THIS FUNCTION, YOU MUST FREE THE RETURNED NODE
// check if a node is already in a list and remove it
node_t *removeMatchFromList(list_t *list, char *pName) {
    if (list->size == 0)
        return NULL;

    node_t *current = list->head;
    node_t *match = current;
    if (!strcmp(list->head->data->pName, pName)) {
        list->head = list->head->next;
    } else {
        if (current->next == NULL) return NULL;
        while (!!strcmp(current->next->data->pName, pName)) {
            current = current->next;
            if (current->next == NULL) return NULL;
        }
        match = current->next;
        current->next = current->next->next;
    }
    list->size--;
    return match;
}


// cleanup process for a list type
void freeList(list_t *list) {
    node_t *currentNode = list->head;
    while (currentNode != NULL) {
        node_t *nextNode = currentNode->next;
        free(currentNode->data->pName);
        free(currentNode->data);
        free(currentNode);
        currentNode = nextNode;
    }
    free(list);
}