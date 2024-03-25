#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

int main(int argc, char *argv[]) {
    // create the linked List of processes
    list_t *process_list = create_list();

    // set up vars to be read from file
    int arrival_time; 
    char* p_name; 
    int service_time;
    int memory_requirement;


    // reading the data into the linked list
    FILE *file = fopen(argv[1], "r");
    char process[1024];
    while (fgets(process, sizeof(process), file)) {
        printf(process);
        sscanf(process, "%d, %s, %d, %d", arrival_time, p_name, service_time, memory_requirement);
        add_to_list(process_list, arrival_time, p_name, service_time, memory_requirement);

    }
}