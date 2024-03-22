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


#endif