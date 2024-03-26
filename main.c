#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "genericMemoryManaging.h"
#include "processManager.h"

int main(int argc, char *argv[]) {
    // create the linked List of processes
    list_t *process_list = create_list();

    // set up vars to be read from file
    int arrival_time; 
    char p_name[8]; 
    int service_time;
    int memory_requirement;


    // reading the data into the linked list
    FILE *file = fopen(argv[1], "r");
    char process[1024];
    while (fgets(process, sizeof(process), file)) {
        sscanf(process, "%d %8s %d %d", &arrival_time, p_name, &service_time, &memory_requirement);
        char *p_name_copy = strdup(p_name);
        add_to_list(process_list, arrival_time, p_name_copy, service_time, memory_requirement);

    }
    processing(process_list,INFINITE);
}