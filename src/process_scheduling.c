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
    /*
    //sort based on arrival time 

    */
    // find waiting time
    result->average_waiting_time = 0;
    ProcessControlBlock_t myPCB = ready_queue->array;
    for (int i = 1; i < (int)ready_queue->count; i++) {
        result->average_waiting_time += myPCB->remaining_burst_time;
        printf("Average waiting at %d processes: %d", i, result->average_waiting_time);
    }
    printf("Total waiting time: ", result->average_waiting_time);

    // find turn around time
    for (int i = 0; i < (int)ready_queue->count; i++)
    return false;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
/*
nextprocess processcontrolbock
uint32 n = readyqueue size_t
sort pbc by burst

ticks = 0;

for(i=0; size > 0)
    ready queue peek
    if (next process. arrival > ticks) {
        i ++;
        if(i == readyqueue->size) {
            i = 0;
        }
        continue;
    }

    ready queueu destroy (ready queue, i);
    i = 0;

    pcbinfo(&nextprocess, 1); //printing don't have to worry about it

    for(; nextprocess.remaining_burst_time !=0; ticks++) {
        virutal_cpu(&nextprocess)
    }

    turn around time = ticks - nextprocess.arrival;
    summed turnaround time += turnaround time;

*/
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
    // Error check to start
    ProcessControlBlock_t * pcb = ready_queue->array;
    size_t = nprocesses = ready_queue->nprocesses;
    sortpcb_arrival(pcb, nprocesses);

    ProcessControlBlock_t nextprocess;

    size_t turnaround = 0;
    size_t waiting = 0;
    int ticks = 0;

    while (ready_queue->nprocesses > 0) {
        ready_queue_pop(ready_queue, &nextprocess);

        for (int i = quantum; i != 0; i--, ticks++) {

            if (ready_queue->arrival < ticks) {
                virtual_cpu(&nextprocess);

                if (nextprocess->remaining_burst_time <= 0) {
                    ticks++;
                    // Calculate turn around time and waiting time for the current process
                    break;
                }
            }
        }

        if (nextprocess->remaining_burst_time > 0) ready_queue_push(ready_queue, &nextprocess);
    }

    // Calculate all averages
    return true;
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
    return dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), nprocesses, NULL);
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
/*
    short by burst time
    if a process comes in with a shorter remaining, then instantly start doing that one
    after executing one tick, then check if need to swap process
*/
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
