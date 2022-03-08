#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    UNUSED(quantum);
    return false;
}

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    if (input_file == NULL) return NULL;

    FILE *fp;
    fp = fopen(input_file, "r");
    if (fp == NULL) return NULL;
    
    uint32_t nprocesses;
    ProcessControlBlock_t *myPCB;
    fread(&nprocesses, sizeof(uint32_t), 1, fp);
    myPCB = (ProcessControlBlock_t *) malloc(nprocesses * sizeof(ProcessControlBlock_t));
    for(uint32_t i = 0; i < nprocesses; i++) {
        fread(&myPCB[i].remaining_burst_time, sizeof(uint32_t), 1, fp);
        fread(&myPCB[i].priority, sizeof(uint32_t), 1, fp);
        fread(&myPCB[i].arrival, sizeof(uint32_t), 1, fp);
        printf("myPCB at %d remaining bust time: %d\n", i, myPCB[i].remaining_burst_time);
        printf("myPCB at %d priority: %d\n", i, myPCB[i].priority);
        printf("myPCB at %d arrival: %d\n", i, myPCB[i].arrival);
    }
    return dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), NULL);
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
