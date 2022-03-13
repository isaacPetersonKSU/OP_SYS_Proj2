#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"
#include "../src/helper.c"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}

#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

dyn_array_t * EmptyValidArrayPtr;
dyn_array_t * PopulatedArrayPtr;

ProcessControlBlock_t low;
ProcessControlBlock_t high;

ScheduleResult_t result;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            EmptyValidArrayPtr = dyn_array_create(0, 1, NULL);
            PopulatedArrayPtr = load_process_control_blocks("../pcb.bin");

            low.remaining_burst_time = 0;
            high.remaining_burst_time = 1;
            low.priority = 0;
            high.priority = 1;
            low.arrival = 0;
            high.arrival = 1;

            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            free(EmptyValidArrayPtr);
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};


/*
    // Reads the PCB burst time values from the binary file into ProcessControlBlock_t remaining_burst_time field
    // for N number of PCB burst time stored in the file.
    // \param input_file the file containing the PCB burst times
    // \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
    dyn_array_t *load_process_control_blocks(const char *input_file);
*/
TEST(loadPCB, nullString)
{
    EXPECT_EQ(nullptr, load_process_control_blocks(NULL));
}
TEST(loadPCB, noFile)
{
    EXPECT_EQ(nullptr, load_process_control_blocks("doesnotexist.txt"));
}
TEST(loadPCB, emptyFile)
{
    ASSERT_EQ(nullptr, load_process_control_blocks("../empty.bin"));
}
TEST (loadPCB, validPerams)
{
    EXPECT_NE(nullptr, load_process_control_blocks("../pcb.bin"));
}
TEST (loadPCB, validateOutpu)
{
    dyn_array_t * queue = load_process_control_blocks("../pcb.bin");
    printf("\t\tBurst T\tPriority\tArival\n");
    for(uint32_t i = 0; i < queue->size; i++)
    {
        ProcessControlBlock_t block = *(ProcessControlBlock_t*)dyn_array_at(queue, i);
        printf("myPCB[%d]\t%d\t%d\t\t%d\n", i, block.remaining_burst_time, block.priority, block.arrival);

        EXPECT_FALSE(block.started);
    }
}

/*
    // Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for first come first served stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
    bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result);
*/
TEST (FCFS, nullArrayPtr)
{
    EXPECT_FALSE(first_come_first_serve(nullptr, &result));
}
TEST (FCFS, nullResultPtr)
{
    EXPECT_FALSE(first_come_first_serve(EmptyValidArrayPtr, nullptr));
}
TEST (FCFS, emptyInputArray)
{
    EXPECT_FALSE(first_come_first_serve(EmptyValidArrayPtr, &result));
}
TEST(FCFS, outputValidation)
{
    EXPECT_TRUE(first_come_first_serve(PopulatedArrayPtr, &result));
}



/*
    // Runs the Shortest Job First Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for shortest job first stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
    bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result);
*/
TEST (SJF, nullArrayPtr)
{
    EXPECT_FALSE(shortest_job_first(nullptr, &result));
}
TEST (SJF, nullResultPtr)
{
    EXPECT_FALSE(shortest_job_first(EmptyValidArrayPtr, nullptr));
}
TEST(SJF, validPerams)
{
    EXPECT_TRUE(shortest_job_first(EmptyValidArrayPtr, &result));
}

/*
    // Runs the Round Robin Process Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for round robin stat tracking \ref ScheduleResult_t
    // \param the quantum
    // \return true if function ran successful else false for an error
    bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum);
*/
TEST(RoundRobin, nullArrayPtr)
{
    EXPECT_FALSE(round_robin(nullptr, &result, QUANTUM));
}
TEST(RoundRobin, nullResultsPtr)
{
    EXPECT_FALSE(round_robin(EmptyValidArrayPtr, nullptr, QUANTUM));
}
TEST (RoundRobin, zeroQuantum)
{
    EXPECT_FALSE(round_robin(EmptyValidArrayPtr, &result, 0));
}
TEST (RoundRobin, validPerams)
{
    EXPECT_TRUE(round_robin(EmptyValidArrayPtr, &result, QUANTUM));
}


/*
    // Runs the Shortest Remaining Time First Process Scheduling algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for shortest job first stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
    bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result);
*/
TEST (SRTF, nullArrayPtr)
{
    EXPECT_FALSE(shortest_remaining_time_first(nullptr, &result));
}
TEST (SRTF, nullResultsPtr)
{
    EXPECT_FALSE(shortest_remaining_time_first(EmptyValidArrayPtr, nullptr));
}
TEST (SRTF, validPerams)
{
    EXPECT_TRUE(shortest_remaining_time_first(EmptyValidArrayPtr, &result));
}

/*
    tests for funtions that compare PCB structs
*/
TEST(comparisonFunctions, RemainingTime)
{
    EXPECT_EQ(-1, compTRemaining(&low, &high));
    EXPECT_EQ(1, compTRemaining(&high, &low));
    EXPECT_EQ(0, compTRemaining(&low, &low));
    EXPECT_EQ(0, compTRemaining(&high, &high));
}
TEST(comparisonFunctions, Priority)
{
    EXPECT_EQ(-1, compPriority(&low, &high));
    EXPECT_EQ(1, compPriority(&high, &low));
    EXPECT_EQ(0, compPriority(&low, &low));
    EXPECT_EQ(0, compPriority(&high, &high));
}
TEST(comparisonFunctions, Arrival)
{
    EXPECT_EQ(-1, compArrival(&low, &high));
    EXPECT_EQ(1, compArrival(&high, &low));
    EXPECT_EQ(0, compArrival(&low, &low));
    EXPECT_EQ(0, compArrival(&high, &high));
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
