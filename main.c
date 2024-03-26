#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "genericMemoryManaging.h"
#include "processManager.h"
#include "roundRobin.h"

int main(int argc, char *argv[]) {
    // create the linked List of processes
    list_t *process_list = create_list();

    // set up vars to be read from file
    int arrival_time; 
    char p_name[8]; 
    int service_time;
    int memory_requirement;


       // Initialize variables to store the argument values
    char *filename = NULL;
    char *memory_strategy = NULL;
    int quantum = 0;

    // Iterate over the arguments
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-m") == 0) {
            memory_strategy = argv[i + 1];
        } else if (strcmp(argv[i], "-q") == 0) {
            quantum = atoi(argv[i + 1]);
        }
    }


    // reading the data into the linked list
    FILE *file = fopen(filename, "r");
    char process[1024];
    while (fgets(process, sizeof(process), file)) {
        sscanf(process, "%d %8s %d %d", &arrival_time, p_name, &service_time, &memory_requirement);
        char *p_name_copy = strdup(p_name);
        add_to_list(process_list, arrival_time, p_name_copy, service_time, memory_requirement);

    }
    // processing(process_list,INFINITE);
    roundRobin(process_list, quantum);
}