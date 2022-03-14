#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"
#include "helper.c"


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
    if(ready_queue == NULL || result == NULL) return false;
    uint32_t n = dyn_array_size(ready_queue);
    if(n == 0) return false;
    if(dyn_array_sort(ready_queue, compArrival) == false) return false;
    
    int ticks = 0;
    int i = 0;
    int * wait_times = calloc(n, sizeof(int));
    int * turnaround_times = calloc(n, sizeof(int));
    
    ProcessControlBlock_t * block = malloc(sizeof(ProcessControlBlock_t));
    while(dyn_array_extract_front(ready_queue, block))
    {
        wait_times[i] = ticks;
        while(block->remaining_burst_time > 0)
        {
            virtual_cpu(block);
            ticks++;
        }
        turnaround_times[i] = ticks - wait_times[i];
        i++;
    }
    
    while(--i > 0)
    {
        result->average_waiting_time += wait_times[i];
        result->average_turnaround_time += turnaround_times[i];
    }
    result->average_waiting_time /= n;
    result->average_turnaround_time /= n;
    result->total_run_time = ticks;
    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    printf("performing shortest job first\n");
    if(ready_queue == NULL || result == NULL) return false;
    printf("\tperams not null\n");
    uint32_t n = dyn_array_size(ready_queue);
    printf("\t%d processes\n", n);
    if(n == 0) return false;
    if(dyn_array_sort(ready_queue, compTRemaining) == false) return false;
    printf("\tprocesses sorted\n");
    
    int ticks = 0;
    int i = 0;
    int * wait_times = calloc(n, sizeof(int));
    int * turnaround_times = calloc(n, sizeof(int));

    ProcessControlBlock_t * block = malloc(sizeof(ProcessControlBlock_t));
    printf("\tresult data allocated");
    while(dyn_array_extract_front(ready_queue, block))
    {
        printf("starting next process\n");
        wait_times[i] = ticks;
        while(block->remaining_burst_time > 0)
        {
            virtual_cpu(block);
            ticks++;
            printf("\t\tVirtual CPU ticked\n");
        }
        turnaround_times[i] = ticks - wait_times[i];
        i++;
    }
    
    while(--i > 0)
    {
        result->average_waiting_time += wait_times[i];
        result->average_turnaround_time += turnaround_times[i];
    }
    result->average_waiting_time /= n;
    result->average_turnaround_time /= n;
    result->total_run_time = ticks;
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

    for(uint32_t i = 0; i < nprocesses; i++) {
        if(fread(&myPCB[i], sizeof(uint32_t), 3, fp) != 3) //fread returns number of elements copied on sucess.
        { 
            free(myPCB);
            return NULL;
        }
        myPCB[i].started = false;
    }

    fclose(fp);
    dyn_array_t * result = dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), free);
    if(result == NULL) free(myPCB);
    return result;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL) return false;
    return true;   
}
