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
    list_t *not_arrived_list = create_list(); // Separate list for processes that have not arrived

    // set up vars to be read from file
    int arrival_time; 
    char p_name[8]; 
    int service_time;
    int memory_requirement;


    // Initialize variables to store the argument values
    char *filename = NULL;
    memoryType memory_strategy= -1;
    int quantum = 0;

    // Iterate over the arguments
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-m") == 0) {
            // calls a function to convert this to a memoryType enum value. 
            memory_strategy = memTypeFromString(argv[i + 1]);
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
        add_to_list(not_arrived_list, arrival_time, p_name_copy, service_time, memory_requirement);
        free(p_name_copy);

    }
    fclose(file);

    // process all processes
    processing(process_list, not_arrived_list, memory_strategy, quantum);

    // clean up
    free(not_arrived_list);
    free(process_list);
    return 0;
}