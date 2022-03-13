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



int sort_arrival(const void* process1, const void* process2){
    return ((const ProcessControlBlock_t*)process1)->arrival - ((const ProcessControlBlock_t*)process2)->arrival;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    /*
    // Error check
    // Sort by arrival time
    ProcessControlBlock_t nextprocess;

    size_t nprocesses = ready_queue->nprocesses;

    int ticks = 0;

    while (ready_queue->nprocesses > 0) {
        ready_queue_pop(ready_queue, &nextprocess);

        for (int i = quantum; q != 0; i--, ticks++) {

            if (ready_queue->arrival < ticks) {
                virtual_cpu(&nextprocess);

                if(nextprocess->remaining_burst_time <=0) { // finished through burst time for the current process
                ticks++;
                // Calculate turn around time and waiting time for this process
                break;
                }
            }
        }
        if (nextprocess->remaining_burst_time > 0) ready_queue_push(ready_queue, &nextprocess);
    }
    
    */
    //add any more error checking
    if (ready_queue == NULL || result == NULL) return false;

    uint32_t nprocs = dyn_array_size(ready_queue);
    uint32_t wait_time[nprocs], turn_around[nprocs], burst_time[nprocs];
    unsigned long total_wait = 0, total_tat = 0, run_time = 0;

    int ticks = 0;
    
    uint32_t i;
    ProcessControlBlock_t *array = ready_queue->array;
    dyn_array_sort(ready_queue, sort_arrival); 
    // Calculate all averages
    for (i = 0; i < nprocs; i++){
        burst_time[i] = array[i].remaining_burst_time;
        run_time += burst_time[i];
    }

    wait_time[0] = 0;
    for (i = 1; i < nprocs; i++){
        wait_time[i] = wait_time[i-1] + burst_time[i-1];
        total_wait += wait_time[i];
    }

    for (i = 0; i < nprocs; i++){
        turn_around[i] = wait_time[i] + burst_time[i];
        total_tat += turn_around[i];
    }

    result->average_waiting_time = (float) total_wait / (float) nprocs;
    result->average_turnaround_time = (float) total_tat / (float) nprocs;
    result->total_run_time = run_time;

    virtual_cpu(array);

    return true;
}

int sort_burst_time(const void* process1, const void* process2){
    return ((const ProcessControlBlock_t*)process1)->remaining_burst_time - ((const ProcessControlBlock_t*)process1)->remaining_burst_time;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

int sort_burst_time(const void* process1, const void* process2){
    return ((const ProcessControlBlock_t*)process1)->priority - ((const ProcessControlBlock_t*)process1)->priority;
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
        myPCB[i].started = false;
    }
    fclose(fp);
    return dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), NULL);
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
