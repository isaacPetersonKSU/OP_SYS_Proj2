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

// sort by arrival
int sort_arrival(const void* process1, const void* process2){
    return ((const ProcessControlBlock_t*)process1)->arrival - ((const ProcessControlBlock_t*)process2)->arrival;
}

// computes stats for nonpreemptive algorithms for schedule result
bool nonpreemptive_stats(const ProcessControlBlock_t* pcb, ScheduleResult_t* result, size_t nprocs){
    uint32_t wait_time[nprocs], turn_around[nprocs], burst_time[nprocs], arrivals[nprocs];
    unsigned long total_wait = 0, total_tat = 0, run_time = 0;
    uint32_t i;

    for (i = 0; i < nprocs; i++){
        burst_time[i] = pcb[i].remaining_burst_time;
        arrivals[i] = pcb[i].arrival;
        run_time += burst_time[i];
    }

    wait_time[0] = 0;
    for (i = 1; i < nprocs; i++){
        wait_time[i] = wait_time[i-1] + burst_time[i-1] - arrivals[i] + arrivals[i-1];
        total_wait += wait_time[i];
    }

    for (i = 0; i < nprocs; i++){
        turn_around[i] = wait_time[i] + burst_time[i];
        printf("wait time %d: %d\n", i, wait_time[i]);
        printf("turn around time %d: %d\n", i, turn_around[i]);
        total_tat += turn_around[i];
    }

    result->average_waiting_time = (float) total_wait / (float) nprocs;
    result->average_turnaround_time = (float) total_tat / (float) nprocs;
    result->total_run_time = run_time;

    return true;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //add any more error checking
    if (ready_queue == NULL || result == NULL) return false;

    dyn_array_sort(ready_queue, sort_arrival);
    
    uint32_t nprocs = dyn_array_size(ready_queue);
   // ProcessControlBlock_t *pcb = ready_queue->array;

    ProcessControlBlock_t curr_process;
    uint32_t tick = 0, total_wait = 0, total_turn_around = 0, wait = 0;
    uint32_t j, burst_time;
    while(!dyn_array_empty(ready_queue)){
        if (!dyn_array_extract_front(ready_queue, &curr_process)) return false;
        wait = tick - curr_process.arrival;
        total_wait += wait;
        burst_time = curr_process.remaining_burst_time;
        total_turn_around += wait + burst_time;
        for(j = 0; j < burst_time; j++){
            virtual_cpu(&curr_process);
            tick++;
        }
    }

    result->total_run_time = tick;
    result->average_waiting_time = (float) total_wait / (float) nprocs;
    result->average_turnaround_time = (float) total_turn_around / (float) nprocs;

    return true;
}

// sorts by burst time
int sort_burst_time(const void* process1, const void* process2){
    return ((const ProcessControlBlock_t*)process1)->remaining_burst_time - ((const ProcessControlBlock_t*)process2)->remaining_burst_time;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || result == NULL) return false;

    uint32_t nprocs = dyn_array_size(ready_queue);

    dyn_array_sort(ready_queue, sort_burst_time);

    ProcessControlBlock_t curr_process;
    uint32_t tick = 0, total_wait = 0, total_turn_around = 0, wait = 0;
    uint32_t j, burst_time;
    while(!dyn_array_empty(ready_queue)){
        if (!dyn_array_extract_front(ready_queue, &curr_process)) return false;

        // sorts by arrival and takes next in arrival
        if (tick > curr_process.arrival){
            dyn_array_push_front(ready_queue, &curr_process);
            dyn_array_sort(ready_queue, sort_arrival);
            if (!dyn_array_extract_front(ready_queue, &curr_process)) return false;
            dyn_array_sort(ready_queue, sort_burst_time);
        }

        wait = tick - curr_process.arrival;
        total_wait += wait;
        burst_time = curr_process.remaining_burst_time;
        total_turn_around += wait + burst_time;
        for(j = 0; j < burst_time; j++){
            virtual_cpu(&curr_process);
            tick++;
        }
    }

    result->total_run_time = tick;
    result->average_waiting_time = (float) total_wait / (float) nprocs;
    result->average_turnaround_time = (float) total_turn_around / (float) nprocs;

    return true;   
}

int sort_priority(const void* process1, const void* process2){
    return ((const ProcessControlBlock_t*)process1)->priority - ((const ProcessControlBlock_t*)process2)->priority;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || result == NULL) return false;

    dyn_array_sort(ready_queue, sort_burst_time);

    uint32_t nprocs = dyn_array_size(ready_queue);
    ProcessControlBlock_t *pcb = ready_queue->array;
    nonpreemptive_stats(pcb, result, nprocs);

    return false;   
}


bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    //add any more error checking
    if (ready_queue == NULL || result == NULL || quantum == 0) return false;

    dyn_array_sort(ready_queue, sort_arrival);
    
    uint32_t nprocs = dyn_array_size(ready_queue);
    //ProcessControlBlock_t *pcb = ready_queue->array;
    uint32_t i, burst_time, total_turn_around = 0, total_wait = 0, wait = 0, tick = 0;
    ProcessControlBlock_t curr_process;
    while(!dyn_array_empty(ready_queue)){
        if (!dyn_array_extract_front(ready_queue, &curr_process)) return false;
        
        wait = tick - curr_process.arrival;
        total_wait += wait;
        burst_time = curr_process.remaining_burst_time;
        total_turn_around += wait;
        if (burst_time > quantum){
            total_turn_around += quantum;
        }
        else{
            total_turn_around += burst_time;
        }

        for (i = 0; i < burst_time && i < quantum; i++){
            virtual_cpu(&curr_process);
            tick++;
        }

        if (curr_process.remaining_burst_time > 0){
            curr_process.arrival = tick;
            dyn_array_push_back(ready_queue, &curr_process);
        }
    }

    result->total_run_time = tick;
    result->average_waiting_time = (float) total_wait / (float) nprocs;
    result->average_turnaround_time = (float) total_turn_around / (float) nprocs;

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
        myPCB[i].started = false;
    }
    fclose(fp);
    return dyn_array_import(myPCB, nprocesses, sizeof(ProcessControlBlock_t), NULL);
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if (ready_queue == NULL || result == NULL) return false;

    uint32_t nprocs = dyn_array_size(ready_queue);

    dyn_array_sort(ready_queue, sort_burst_time);

    ProcessControlBlock_t curr_process;
    const ProcessControlBlock_t* next_process;
    uint32_t tick = 0, total_wait = 0, total_turn_around = 0, wait = 0;
    uint32_t i, burst_time;
    while(!dyn_array_empty(ready_queue)){
        if (!dyn_array_extract_front(ready_queue, &curr_process)) return false;

        // sorts by arrival and takes next in arrival
        if (tick > curr_process.arrival){
            dyn_array_push_front(ready_queue, &curr_process);
            dyn_array_sort(ready_queue, sort_arrival);
            if (!dyn_array_extract_front(ready_queue, &curr_process)) return false;
            dyn_array_sort(ready_queue, sort_burst_time);
        }

        wait = tick - curr_process.arrival;
        total_wait += wait;
        burst_time = curr_process.remaining_burst_time;
        total_turn_around += wait + burst_time;

        next_process = dyn_array_front(ready_queue);
        for(i = 0; i < burst_time; i++){
            virtual_cpu(&curr_process);
            tick++;
            if (!next_process) continue;
            
            if (next_process->remaining_burst_time < burst_time && next_process->remaining_burst_time < burst_time){
                curr_process.arrival = tick;
                dyn_array_push_front(ready_queue, &curr_process);
                dyn_array_sort(ready_queue, sort_burst_time);
                break;
            }
        }
    }

    result->total_run_time = tick;
    result->average_waiting_time = (float) total_wait / (float) nprocs;
    result->average_turnaround_time = (float) total_turn_around / (float) nprocs;

    return true;   
}
