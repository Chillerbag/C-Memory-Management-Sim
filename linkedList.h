#ifndef LIST_H
#define LIST_H

typedef struct {
    int arrival_time;
    char* p_name;
    int service_time;
    int memory_requirement;
    struct node_t* next;
} node_t;

typedef struct list {
    int size;
    node_t* head;
} list_t;

list_t* create_list();

void add_to_list(list_t* list, int arrival_time, const char* p_name, int service_time, int memory_requirement);

node_t* remove_from_list(list_t* list);

void free_list(list_t* list);

#endif