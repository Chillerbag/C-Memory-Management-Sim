#include "memoryManagerPaged.h"
#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_COUNT 512
struct pagedMemoryState {
    char **pageFrames;
    // the head of this list is the least recently used
    list_t *processesWithMemory;
    int freePages;
    int mem;
};

void *intialiseMemoryPaged() {
    struct pagedMemoryState *state = (struct pagedMemoryState *)malloc(sizeof(struct pagedMemoryState));
    state->processesWithMemory = create_list();

    // TODO:Remove magic number for p_name length
    state->pageFrames = malloc(PAGE_COUNT * sizeof(char *));
    for (int i = 0; i < PAGE_COUNT; i++)
        state->pageFrames[i] = NULL;
    // state->pageFrames[i] = malloc((8) * sizeof(char));
    state->freePages = PAGE_COUNT;
    return state;
}
void clearProcessMemoryPaged(void *statev, process_t *process) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    // clear pages
    int j = 0;
    int *clearedFrames = malloc(process->memory_requirement * sizeof(int));

    for (int i = 0; i < PAGE_COUNT; i++) {
        if (state->pageFrames[i] != NULL && !strcmp(process->p_name, state->pageFrames[i])) {
            state->pageFrames[i] = NULL;
            clearedFrames[j++] = i;
        }
    }
    printf(",EVICTED,evicted-frames=%s\n", stringOfIntArray(clearedFrames, process->memory_requirement));
    
    // cleanup state's processesWithMemory list
    state->freePages += process->memory_requirement;
    node_t *toBeFreed = remove_match_from_list(state->processesWithMemory,process->p_name);
    if (toBeFreed==NULL) return;
    
    free(toBeFreed->data->p_name);
    free(toBeFreed->data);
    free(toBeFreed);

    
}

bool allocateMemoryPaged(void *statev, process_t *process) {
    struct pagedMemoryState *state = (struct pagedMemoryState *)statev;
    // Is it already in memory?
    node_t *matchNode = remove_match_from_list(state->processesWithMemory,process->p_name);
    if (NULL != matchNode) {
        add_process_to_list(state->processesWithMemory,matchNode->data);
        free(matchNode);
        return true;
    }
    // if we dont have space, make some
    while (state->freePages < process->memory_requirement) {
        node_t *clearedProcessNode = remove_head_from_list(state->processesWithMemory);
        clearProcessMemoryPaged(state, clearedProcessNode->data);
        free(clearedProcessNode->data->p_name);
        free(clearedProcessNode->data);
        free(clearedProcessNode);
    }

    // allocate the memory
    int toBeAllocated = process->memory_requirement;
    int *allocatedFrames = malloc(process->memory_requirement * sizeof(int));
    for (int i = 0; toBeAllocated > 0; i++) {
        if (state->pageFrames[i] == NULL) {
            state->pageFrames[i] = process->p_name;
            allocatedFrames[process->memory_requirement - toBeAllocated--] = i;
        }
    }
    state->freePages -= process->memory_requirement;
    add_to_list(state->processesWithMemory, -1, process->p_name, -1, process->memory_requirement);
    //TODO:Fix off by one or rounding error with the percent
    printf(",RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,mem-frames=%s\n", process->p_name, process->service_time, 100 - (100 * state->freePages) / PAGE_COUNT, stringOfIntArray(allocatedFrames, process->memory_requirement));

    return true;
}

// adapted from:https://stackoverflow.com/questions/30234363/how-can-i-convert-an-int-array-into-a-string-array
// TODO:Move to a utils?
char *stringOfIntArray(int *array, int size) {
    if (size == 0)
        return "[]";

    char *str = (char *)malloc(sizeof(char) * 5 * size);
    int index = sprintf(&str[0], "[");
    for (int i = 0; i < size; i++)
        index += sprintf(&str[index], "%d,", array[i]);
    sprintf(&str[index - 1], "]");
    return str;
}