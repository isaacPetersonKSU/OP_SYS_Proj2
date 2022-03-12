#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define SJF "SJF"
#define RR "RR"
#define SRTF "SRTF"

// Add and comment your analysis code in this function.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    dyn_array_t * ready_queue = load_process_control_blocks(argv[1]);

    ScheduleResult_t result = {0, 0, 0};

    if (!ready_queue) return -1;

    if (strcmp(argv[2], FCFS) == 0) {
        if (first_come_first_serve(ready_queue, &result)) printf("First Come First Serve Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu", result->average_waiting_time, result->average_turnaround_time, result->total_run_time);
        else printf("First Come First Serve has a return type: false");
    }
    else if (strcmp(argv[2], SJF) == 0) {
        if (shortest_job_first(ready_queue, &result)) printf("Shortest Job First Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu", result->average_waiting_time, result->average_turnaround_time, result->total_run_time);
        else printf("Shortest Job First has a return type: false");
    }
    else if (strcmp(argv[2], RR) == 0) {
        if (round_robin(ready_queue, &result, quantum)) printf("Round Robin Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu", result->average_waiting_time, result->average_turnaround_time, result->total_run_time);
        else printf("Round Robin has a return type: false");
    }
    else if (strcmp(argv[2], SRTF) == 0) {
        if (shortest_remaining_time_first(ready_queue, &result)) printf("Shortest Remaining Time First Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu", result->average_waiting_time, result->average_turnaround_time, result->total_run_time);
        else printf("Shortest Remaining Time First has a return type: false");
    }
    else printf("Incorrect input");

    return EXIT_SUCCESS;
}
