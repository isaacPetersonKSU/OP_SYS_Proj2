#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}

#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

dyn_array_t * arrayPtr;
ScheduleResult_t result;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            arrayPtr = dyn_array_create(0,0,NULL);

            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            free(arrayPtr);
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
    EXPECT_FALSE(first_come_first_serve(arrayPtr, nullptr));
}
TEST (FCFS, validPerams)
{
    EXPECT_TRUE(first_come_first_serve(arrayPtr, &result));
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
    EXPECT_FALSE(shortest_job_first(arrayPtr, nullptr));
}
TEST(SJF, validPerams)
{
    EXPECT_TRUE(shortest_job_first(arrayPtr, &result));
}


/*
    // Runs the Priority algorithm over the incoming ready_queue
    // \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
    // \param result used for shortest job first stat tracking \ref ScheduleResult_t
    // \return true if function ran successful else false for an error
    bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result);
*/
TEST (Priority, nullArrayPtr)
{
    EXPECT_FALSE(priority(nullptr, &result));
}
TEST (Priority, nullResultsPtr)
{
    EXPECT_FALSE(priority(arrayPtr, nullptr));
}
TEST(Priority, validPerams)
{
    EXPECT_TRUE(priority(arrayPtr, &result));
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
    EXPECT_FALSE(round_robin(arrayPtr, nullptr, QUANTUM));
}
TEST (RoundRobin, zeroQuantum)
{
    EXPECT_FALSE(round_robin(arrayPtr, &result, 0));
}
TEST (RoundRobin, validPerams)
{
    EXPECT_TRUE(round_robin(arrayPtr, &result, QUANTUM));
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
    EXPECT_FALSE(shortest_remaining_time_first(arrayPtr, nullptr));
}
TEST (SRTF, validPerams)
{
    EXPECT_TRUE(shortest_remaining_time_first(arrayPtr, &result));
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
