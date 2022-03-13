#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define TIMESTAMP_MSG "%d-%02d-%02d %02d:%02d:%02d\n"

#define FCFS_SUCCESS_MSG "\tFirst Come First Serve Analysis:\n"
#define SJF_SUCCESS_MSG "\tShortest Job First Analysis:\n"
#define RR_SUCCESS_MSG "\tRound Robin Analysis:\n"
#define SRTF_SUCCESS_MSG "\tShortest Remaining Time First Analysis:\n"

#define RESULTS_MSG "\tWaiting Time - %f, Turnaround Time - %f, Total Time - %lu\n"
#define FAIL_MSG "\tunable to perform algorithm\n"

enum algorithm { invalid, FCFS, SJF, RR, SRTF};

int metaDataOutput(char* filePath, enum algorithm al, ScheduleResult_t * result)
{
    FILE * metaData = fopen(filePath, "a");
    if(metaData == NULL)
    {
        printf("could not open file\n");
        return EXIT_FAILURE;
    } 

    time_t t = time(NULL);
    struct tm tm = * localtime(&t);
    fprintf(metaData, TIMESTAMP_MSG, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    switch(al)
    {
        case FCFS: 
            fprintf(metaData, FCFS_SUCCESS_MSG);
            break;    
        case SJF: 
            fprintf(metaData, FCFS_SUCCESS_MSG);
            break;
        case RR: 
            fprintf(metaData, FCFS_SUCCESS_MSG); 
            break;
        case SRTF: 
            fprintf(metaData, FCFS_SUCCESS_MSG);
            break;
        default:
            fprintf(metaData, FAIL_MSG);
            printf(FAIL_MSG);
            fclose(metaData);
            free(result);
            return 0;
            break;
    }
    fprintf(metaData, RESULTS_MSG, result->average_waiting_time, result->average_turnaround_time, result->total_run_time);
    fclose(metaData);
    free(result);
    printf("results written in file\n");
    return 1;
}

// Add and comment your analysis code in this function.
int main(int argc, char **argv) 
{
    if (argc < 3 || argc > 4) 
    {
        printf("%s <pcb file> <schedule algorithme> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    dyn_array_t * ready_queue = load_process_control_blocks(argv[1]);
    if (ready_queue == 0) return EXIT_FAILURE;
    ScheduleResult_t * result = calloc(1, sizeof(ScheduleResult_t));

    enum algorithm al = 0;
    if (!strcmp(argv[2], "FCFS") && first_come_first_serve(ready_queue, result)) al = FCFS;
    else if (!strcmp(argv[2], "SJF") && shortest_job_first(ready_queue, result)) al = SJF;
    else if (!strcmp(argv[2], "RR") && argc == 4 && round_robin(ready_queue, result, atoi(argv[3]))) al = RR;

    else if (!strcmp(argv[2], "SRTF") && shortest_remaining_time_first(ready_queue, result)) al = SRTF;

    return metaDataOutput("../readme.md", al, result);
}