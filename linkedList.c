#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

list_t *create_list() {
    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    new_list->size = 0;
    new_list->head = NULL;
    return new_list;
}

void add_to_list(list_t *list, int arrival_time, const char *p_name, int service_time, int memory_requirement) {
    process_t *new_data = (process_t *)malloc(sizeof(process_t));
    new_data->arrival_time = arrival_time;
    new_data->p_name = strdup(p_name);
    new_data->service_time = service_time;
    new_data->memory_requirement = memory_requirement;
    add_process_to_list(list, new_data);
}

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
node_t *remove_from_list(list_t *list) {
    if (list->size == 0) {
        return NULL;
    }
    node_t *node_to_remove = list->head;
    list->head = node_to_remove->next;
    list->size--;
    return node_to_remove;
}

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