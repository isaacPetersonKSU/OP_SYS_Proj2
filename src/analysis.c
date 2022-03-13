#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    dyn_array_t* ready_queue = load_process_control_blocks(argv[1]);
    ScheduleResult_t result = {0,0,0};

    if (!ready_queue) return -1;

    if (strcmp(argv[2], FCFS) == 0){
        if (first_come_first_serve(ready_queue, &result)){
            // print results
        }
        else{
            return EXIT_FAILURE;
        }
    }
    if (strcmp(argv[2], P))

    // fprintf something

    return EXIT_SUCCESS;
}
