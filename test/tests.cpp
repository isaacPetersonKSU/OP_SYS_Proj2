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

dyn_array_t* ready_queue = load_process_control_blocks("../pcb.bin");

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};

TEST (process_scheduling, loadPCB){
    EXPECT_EQ(nullptr, load_process_control_blocks(NULL));
    EXPECT_EQ(nullptr, load_process_control_blocks("doesnotexist.txt"));
    
    dyn_array_t *pcb = load_process_control_blocks("../pcb.bin");
    EXPECT_NE(nullptr, pcb);
    
    // test individual values
}

TEST (proces_scheduling, FCFS_validate_input){
    ScheduleResult_t result;
    first_come_first_serve(ready_queue, &result);
    EXPECT_EQ(50, result.total_run_time);
    EXPECT_EQ(17.5, result.average_waiting_time);
    EXPECT_EQ(30, result.average_turnaround_time);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}