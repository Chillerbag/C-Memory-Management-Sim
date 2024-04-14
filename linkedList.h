#ifndef LIST_H
#define LIST_H

// struct for a single process
typedef struct process {
    int arrival_time;
    char *p_name;
    int service_time;
    int remainingTime;
    int memory_requirement;
} process_t;

// a node containing a process and a pointer to the next node in the LL
typedef struct node {
    process_t *data;
    struct node *next;
} node_t;

// a list that has a head node pointer and the size of the list
typedef struct list {
    int size;
    node_t *head;
} list_t;

// create a list_t
list_t *create_list();

// create a node
void add_to_list(list_t *list, int arrival_time, const char *p_name, int service_time, int memory_requirement);

// in node creation, call this to add the node to the list
void add_process_to_list(list_t *list, process_t *process);

// remove head from the list (process we're dealing with, to call if process finishes)
node_t *remove_head_from_list(list_t *list);

// check if a provided p_name is in a provided list
node_t *remove_match_from_list(list_t *list,char* name);

// cleanup used memory 
void free_list(list_t *list);

#endif