#include <stdio.h>
#include <stdlib.h>
#include <linkedList.h>

list_t* create_list() {
    list_t* new_list = (list_t*)malloc(sizeof(list_t));
    new_list->size = 0;
    new_list->head = NULL;
    return new_list;
}
 
void add_to_list(list_t* list, void* data) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    //new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}
 
void* remove_from_list(list_t* list) {
    if (list->size == 0) {
        return NULL;
    }
    node_t* node_to_remove = list->head;
    // void* data = node_to_remove->data;
    list->head = node_to_remove->next;
    free(node_to_remove);
    list->size--;
    return NULL; // return the data here. temp.
}
 
void free_list(list_t* list) {
    node_t* current_node = list->head;
    while (current_node != NULL) {
        node_t* next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(list);
}