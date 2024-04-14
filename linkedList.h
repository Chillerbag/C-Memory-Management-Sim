#ifndef LIST_H
#define LIST_H

// struct for a single process
typedef struct process {
    int arrivalTime;
    char *pName;
    int serviceTime;
    int remainingTime;
    int memoryRequirement;
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
list_t *createList();

// create a node
void addToList(list_t *list, int arrivalTime, const char *pName, int serviceTime, int memoryRequirement);

// in node creation, call this to add the node to the list
void addProcessToList(list_t *list, process_t *process);

// remove head from the list (process we're dealing with, to call if process finishes)
node_t *removeHeadFromList(list_t *list);

// check if a provided pName is in a provided list
node_t *removeMatchFromList(list_t *list,char* name);

// cleanup used memory 
void freeList(list_t *list);

#endif