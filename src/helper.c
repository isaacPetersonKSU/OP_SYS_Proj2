#include <stdio.h>
#include <stdint.h>
#include "processing_scheduling.h"

#ifndef __HELPER_C
#define __HELPER_C


void swappcb(ProcessControlBlock_t * pcb1, ProcessControlBlock_t * pcb2)
{
    ProcessControlBlock_t  temppcb = *pcb1;
    *pcb1 = *pcb2;
    *pcb2 = temppcb;
}
    
/*
void sortpcb_arrival(ProcessControlBlock_t * pcb, size_t n)
{
    for(size_t j = n-1; j > 0; j--) {
        for(size_t i = 0; i < j ; i++) {
            if(pcb[i].arrival > pcb[i+1].arrival) {
                swappcb(&pcb[i], &pcb[i+1]);
            }
        }
    }
}*/

///
/// Returns a pointer to the desired object in the array
/// Pointer may be invalidated if the container increases in size
/// \param dyn_array the dynamic array of PCBs to sort
/// \param attribute what to sort by 
///         0 for remaining_burst_time
///         1 for priority
///         2 for arrival
///         anything else will abort and return false
/// \return true if sucessfull, false for invalid perams
///
bool pcb_dynamic_sorter(dyn_array_t * dyn_array, char attribute)
{
    if(dyn_array == NULL || attribute > (char)2) return false;
    printf("\t\t-valid perams\n");

    uint32_t n = dyn_array_size(dyn_array);
    printf("\t\t-%u processes\n", n);

    char debuggingText[3][20] = {"remaining_burst_time", "priority", "arrival"};
    printf("\t\t-sorting by %s\n", debuggingText[(int)attribute]);
    
    for(size_t j = n-1; j > 0; j--) {
        for(size_t i = 0; i < j ; i++) {
            if(dyn_array_at(dyn_arr, i)->arrival)
                swappcb(&pcb[i], &pcb[i+1]);
            }
        }
    }
    printf("\t\t-sort successful\n");
    return true;
}


void sortpcb_burst(ProcessControlBlock_t * pcb, size_t n)
{
    for(size_t j = n-1; j > 0; j--) {
        for(size_t i = 0; i < j ; i++) {
            if(pcb[i].remaining_burst_time > pcb[i+1].remaining_burst_time) {
                swappcb(&pcb[i], &pcb[i+1]);
            }
        }
    }
}


void pcbinfo(ProcessControlBlock_t * pcb, size_t n)
{
    for(size_t i = 0; i < n; i++) {
        printf("Burst time: %u | Priority: %u | Arrival: %u\n", pcb[i].remaining_burst_time, pcb[i].priority, pcb[i].arrival);
    }
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
}


int ready_queue_pop(dyn_array_t * ready_queue, ProcessControlBlock_t * target)
{
    ProcessControlBlock_t * pcb = (ProcessControlBlock_t *) (ready_queue->array);
    
    memcpy(target, pcb, sizeof(ProcessControlBlock_t));
    ready_queue->size =  ready_queue->size - 1;
    
    /* Shift all the elements */
    for(size_t i = 0; i < ready_queue->size; i++)
    {
        *pcb = *(pcb+1);
        pcb++;
    }
    
    return 0;
}


int ready_queue_push(dyn_array_t * ready_queue, ProcessControlBlock_t * src)
{
    ProcessControlBlock_t *pcb = (ProcessControlBlock_t *) (ready_queue->array);
    
    /* Add the element to the tail of the queue */
    pcb += ready_queue->size;
    memcpy(pcb, src, sizeof(ProcessControlBlock_t));
   ready_queue->size =  ready_queue->size +1;
    
    return 0;
}


int ready_queue_peek(dyn_array_t * ready_queue, uint32_t i, ProcessControlBlock_t * target)
{
    ProcessControlBlock_t * pcb = (ProcessControlBlock_t *) (ready_queue->array);
    
    memcpy(target, pcb+i, sizeof(ProcessControlBlock_t));
    
    return 0;
}


int ready_queue_destroy(dyn_array_t * ready_queue, uint32_t i)
{
    ProcessControlBlock_t * pcb = (ProcessControlBlock_t *) (ready_queue->array);
    
    pcb = pcb+i;
    ready_queue->size =  ready_queue->size - 1;
    
    /* Shift all the elements */
    for(size_t j = (size_t)i; j < ready_queue->size; j++)
    {
        *pcb = *(pcb+1);
        pcb++;
    }
    
    return 0;
}

#endif