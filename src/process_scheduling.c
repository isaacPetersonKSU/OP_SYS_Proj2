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

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    if (input_file == NULL) return NULL; // File does not exist

    FILE *fp;
    fp = fopen(input_file, "r"); // Open the PCB file to be read

    if (fp == NULL) return NULL; // No data in file
    
    uint32_t nprocesses; // Number of processes
    ProcessControlBlock_t *myPCB; // Pointer to PCB struct
    fread(&nprocesses, sizeof(uint32_t), 1, fp); // Read the first 32 bits from PCB file to get number of processes
    myPCB = (ProcessControlBlock_t *) malloc(nprocesses * sizeof(ProcessControlBlock_t)); // Allocate memory for PCB struct based on number of processes

    for(uint32_t i = 0; i < nprocesses; i++) { // Read the 32 bits of each process and store them into the correct part of the PCB struct
        fread(&myPCB[i].remaining_burst_time, sizeof(uint32_t), 1, fp);
        fread(&myPCB[i].priority, sizeof(uint32_t), 1, fp);
        fread(&myPCB[i].arrival, sizeof(uint32_t), 1, fp);
    }
    fclose(fp);

    return dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), NULL); // Use the given dyn_array_import to create the array of processes to use later
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || result == NULL) return false; // Bad params

    ProcessControlBlock_t * PCB = ready_queue->array;
    size_t nprocesses = ready_queue->size;
    sortpcb_arrival(PCB, nprocesses);

    ProcessControlBlock_t next_process;
    
    size_t waiting = 0, summed_waiting = 0;
    size_t turnaround = 0, summed_turnaround = 0;

    int ticks = 0;

    for (int i = 0; i < nprocesses; i++) {

        if (ready_queue_pop(ready_queue, &next_process) < 0) return false;

        for (; next_process.remaining_burst_time != 0; ticks++) {
            virtual_cpu(&next_process);
        }

        waiting = ticks - next_process.arrival;
        summed_waiting += waiting;

        turnaround = waiting + next_process.remaining_burst_time;
        summed_turnaround += turnaround;
    }

    result->average_waiting_time = (float)summed_waiting / (float)nprocesses;
    result->average_turnaround_time = (float)summed_turnaround / (float)nprocesses;
    result->total_run_time = ticks;

    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || result == NULL) return false; // Bad params

    ProcessControlBlock_t * PCB = ready_queue->array;
    size_t nprocesses = ready_queue->size;
    sortpcb_burst(PCB, nprocesses);

    ProcessControlBlock_t next_process;
    
    size_t waiting = 0, summed_waiting = 0;
    size_t turnaround = 0, summed_turnaround = 0;

    int ticks = 0;

    for (int i = 0; nprocesses > 0;) {
        ready_queue_peek(ready_queue, 1, &next_process);

        if ((int)next_process.arrival > ticks) {
            i++;

            if (i == (int)ready_queue->size) {
                i = 0;
            }

            continue;
        }

        ready_queue_destroy(ready_queue, i);
        i = 0;

        for(; next_process.remaining_burst_time != 0; ticks++) {
            virtual_cpu(&next_process);
        }

        waiting = ticks - next_process.arrival;
        summed_waiting += waiting;

        turnaround = waiting + next_process.remaining_burst_time;
        summed_turnaround += turnaround;
    }

    result->average_waiting_time = (float)summed_waiting / (float)nprocesses;
    result->average_turnaround_time = (float)summed_turnaround / (float)nprocesses;
    result->total_run_time = ticks;

    return true;
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    if (ready_queue == NULL || result == NULL || quantum <= 0) return false; // Bad params

    ProcessControlBlock_t * PCB = ready_queue->array;
    size_t nprocesses = ready_queue->size;
    sortpcb_arrival(PCB, nprocesses);

    ProcessControlBlock_t next_process;
    
    size_t waiting = 0, summed_waiting = 0;
    size_t turnaround = 0, summed_turnaround = 0;

    int ticks = 0;

    while (nprocesses > 0) {

        if (ready_queue_pop(ready_queue, &next_process) < 0) return false;

        for (int i = quantum; i != 0; i--, ticks++) {

            if ((int)next_process->arrival < ticks) {
                virtual_cpu(&next_process);

                if (next_process->remaining_burst_time <= 0) {
                    ticks++;

                    waiting = ticks - next_process.remaining_burst_time;
                    summed_waiting += waiting;

                    turnaround = waiting + next_process.remaining_burst_time;
                    summed_turnaround += turnaround;

                    break;
                }
            }
        }

        if (next_process->remaining_burst_time > 0) ready_queue_push(ready_queue, &next_process);
    }

    result->average_waiting_time = (float)summed_waiting / (float)nprocesses;
    result->average_turnaround_time = (float)summed_turnaround / (float)nprocesses;
    result->total_run_time = ticks;

    return true;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
