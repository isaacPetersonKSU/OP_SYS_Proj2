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
    if(ready_queue == NULL)
    {
        printf("null queue pointer\n");
        return false;
    }
    if(result == NULL)
    {
        printf("results pointer null\n");
        return false;
    }
    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL) return false;
    return true;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //DO NOT IMPLEMENT! NOT PART OF ASSIGNMENT
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    if(ready_queue == NULL || result == NULL || quantum <= 0) return false;
    return true;   
}

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    if (input_file == NULL) return NULL;

    FILE *fp = fopen(input_file, "r");
    if (fp == NULL) return NULL;
    
    uint32_t nprocesses;
    fread(&nprocesses, sizeof(uint32_t), 1, fp);

    ProcessControlBlock_t *myPCB = (ProcessControlBlock_t *) malloc(nprocesses * sizeof(ProcessControlBlock_t));

    //printf("\n\t\tArival\tBurst T\tPriority\n");
    for(uint32_t i = 0; i < nprocesses; i++) {
        if(fread(&myPCB[i], sizeof(uint32_t), 3, fp) != 3)
        {
            free(myPCB);
            return NULL;
        }
        myPCB[i].started = false;
        //ProcessControlBlock_t iPCB = myPCB[i];
        //printf("myPCB[%d]\t%d\t%d\t%d\n", i, iPCB.remaining_burst_time, iPCB.priority, iPCB.arrival);
    }

    fclose(fp);
    dyn_array_t * result = dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), NULL);
    if(result == NULL) free(myPCB);
    return result;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL) return false;
    return true;   
}
