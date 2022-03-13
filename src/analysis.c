#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define SJF "SJF"
#define RR "RR"
#define SRTF "SRTF"

#define FCFS_SUCCESS_MSG "First Come First Serve Analysis:\n\tWaiting Time - %f, Turnaround Time - %f, Total Time - %lu\n"
#define FCFC_FAIL_MSG "First Come First Serve has a return type: false\n"

#define SJF_SUCCESS_MSG "Shortest Job First Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu\n"
#define SJF_FAIL_MSG "Shortest Job First has a return type: false\n"

#define RR_SUCCESS_MSG "Round Robin Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu"
#define RR_FAIL_MSG "Round Robin has a return type: false"

#define SRTF_SUCCESS_MSG "Shortest Remaining Time First Analysis: Waiting Time - %f, Turnaround Time - %f, Total Time - %lu"
#define SRTF_FAIL_MSG "Shortest Remaining Time First has a return type: false"


// Add and comment your analysis code in this function.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithme> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
    //printf("-input validated\n");

    int quantum = atoi(argv[3]);
    //printf("-valid quantum input: %d\n", quantum);

    dyn_array_t * ready_queue = load_process_control_blocks(argv[1]);
    if (!ready_queue) return -1;
    //printf("-file loaded\n");

    ScheduleResult_t result = {0, 0, 0};
    //printf("-results struct created\n");


    if (strcmp(argv[2], FCFS) == 0) 
    {
        if (first_come_first_serve(ready_queue, &result)) 
        {  
            printf(FCFS_SUCCESS_MSG, result.average_waiting_time, result.average_turnaround_time, 
                result.total_run_time);
            //TODO: write results to readme.md
        }
        else printf(FCFC_FAIL_MSG);
    }
    else if (strcmp(argv[2], SJF) == 0) 
    {
        if (shortest_job_first(ready_queue, &result))
        {
            printf(SJF_SUCCESS_MSG, result.average_waiting_time, result.average_turnaround_time, 
                result.total_run_time);
            //TODO: write results to readme.md
        } 
        else printf(SJF_FAIL_MSG);

    }
    else if (strcmp(argv[2], RR) == 0) 
    {
        if (round_robin(ready_queue, &result, quantum))
        {
            printf(RR_SUCCESS_MSG, result.average_waiting_time, result.average_turnaround_time, 
                result.total_run_time);
            //TODO: write results to readme.md
        } 
        else printf(RR_FAIL_MSG);
    }
    else if (strcmp(argv[2], SRTF) == 0) 
    {
        if (shortest_remaining_time_first(ready_queue, &result))
        {
            printf(SRTF_SUCCESS_MSG, result.average_waiting_time, result.average_turnaround_time, 
                result.total_run_time);
            //TODO: write results to readme.md
        }         
        else printf(SRTF_FAIL_MSG);
    }
    else printf("Incorrect input: Not a scheduleing alg.");
    return EXIT_SUCCESS;
}