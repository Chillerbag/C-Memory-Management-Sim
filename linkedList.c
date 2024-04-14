#include "linkedList.h"
#include <stdlib.h>
#include <string.h>

// creates a list by mallocing space and initing params. 
list_t *create_list() {
    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    new_list->size = 0;
    new_list->head = NULL;
    return new_list;
}

// create a node 
void add_to_list(list_t *list, int arrival_time, const char *p_name, int service_time, int memory_requirement) {
    process_t *new_data = (process_t *)malloc(sizeof(process_t));
    new_data->arrival_time = arrival_time;
    // dynamically allocated string, so use strdup instead of an ugly malloc
    new_data->p_name = strdup(p_name);
    new_data->service_time = service_time;
    new_data->remainingTime = service_time;
    new_data->memory_requirement = memory_requirement;
    // add it to a provided list
    add_process_to_list(list, new_data);
}

// add a provided node to a list
void add_process_to_list(list_t *list, process_t *process) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = process;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    }

    else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->size++;
}

// IMPORTANT: ONCE YOU USE THIS FUNCTION, YOU MUST FREE THE RETURNED NODE
// remove head from the list (process we're dealing with, to call if process finishes)
node_t *remove_head_from_list(list_t *list) {
    if (list->size == 0) {
        return NULL;
    }
    node_t *node_to_remove = list->head;
    list->head = node_to_remove->next;
    list->size--;
    return node_to_remove;
}
// IMPORTANT: ONCE YOU USE THIS FUNCTION, YOU MUST FREE THE RETURNED NODE
// check if a node is already in a list and remove it
node_t *remove_match_from_list(list_t *list, char *p_name) {
    if (list->size == 0)
        return NULL;

    node_t *current = list->head;
    node_t *match = current;
    if (!strcmp(list->head->data->p_name, p_name)) {
        list->head = list->head->next;
    } else {
        if (current->next == NULL) return NULL;
        while (!!strcmp(current->next->data->p_name, p_name)) {
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
void free_list(list_t *list) {
    node_t *current_node = list->head;
    while (current_node != NULL) {
        node_t *next_node = current_node->next;
        free(current_node->data->p_name);
        free(current_node->data);
        free(current_node);
        current_node = next_node;
    }
    free(list);
}