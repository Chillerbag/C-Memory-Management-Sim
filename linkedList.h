#ifndef LIST_H
#define LIST_H

typedef struct process {
    int arrival_time;
    char *p_name;
    int service_time;
    int memory_requirement;
} process_t;

typedef struct node {
    process_t *data;
    struct node *next;
} node_t;

typedef struct list {
    int size;
    node_t *head;
} list_t;

list_t *create_list();

void add_to_list(list_t *list, int arrival_time, const char *p_name, int service_time, int memory_requirement);
void add_process_to_list(list_t *list, process_t *process);

node_t *remove_head_from_list(list_t *list);
node_t *remove_match_from_list(list_t *list,char* name);

void free_list(list_t *list);

#endif