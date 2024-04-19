#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "genericMemoryManaging.h"
#include "processManager.h"
#include "roundRobin.h"

int main(int argc, char *argv[]) {
    // create the linked List of processes

    list_t *processList = createList();
    list_t *notArrivedList = createList(); // Separate list for processes that have not arrived

    // set up vars to be read from file
    unsigned int arrivalTime; 
    char pName[8]; 
    unsigned int serviceTime;
    int memoryRequirement;


    // Initialize variables to store the argument values
    char *filename = NULL;
    memoryType memoryStrategy= -1;
    int quantum = 0;

    // Iterate over the arguments
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-m") == 0) {
            // calls a function to convert this to a memoryType enum value. 
            memoryStrategy = memTypeFromString(argv[i + 1]);
        } else if (strcmp(argv[i], "-q") == 0) {
            quantum = atoi(argv[i + 1]);
        }
    }


    // reading the data into the linked list
    FILE *file = fopen(filename, "r");
    char process[1024];
    while (fgets(process, sizeof(process), file)) {
        sscanf(process, "%d %8s %d %d", &arrivalTime, pName, &serviceTime, &memoryRequirement);
        char *pNameCopy = strdup(pName);
        addToList(notArrivedList, arrivalTime, pNameCopy, serviceTime, memoryRequirement);
        free(pNameCopy);

    }
    fclose(file);

    // process all processes
    doProcessing(processList, notArrivedList, memoryStrategy, quantum);

    // clean up
    free(notArrivedList);
    free(processList);
    return 0;
}