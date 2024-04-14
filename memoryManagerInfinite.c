#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>

void *intialiseMemoryInfinite() {
    return NULL;
}
void clearProcessMemoryInfinite(void *state, process_t *process, int time) {
}
bool allocateMemoryInfinite(void *state, process_t *process, int time) {
    printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, process->p_name, process->service_time);
    return true;
}
void cleanMemoryInfinite(void * state) {

}