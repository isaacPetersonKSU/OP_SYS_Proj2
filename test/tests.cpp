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
    EXPECT_NE(nullptr, load_process_control_blocks("../pcb.bin"));
    EXPECT_EQ(nullptr, load_process_control_blocks(NULL));
    EXPECT_EQ(nullptr, load_process_control_blocks("doesnotexist.txt"));
}

TEST (proces_scheduling, FCFS_validate_input){
    dyn_array_t * arrayPtr = dyn_array_create(0,0,NULL);
    ScheduleResult_t result;
    EXPECT_EQ(false, first_come_first_serve(NULL, &result));
    EXPECT_EQ(false, first_come_first_serve(arrayPtr, NULL));
    EXPECT_EQ(true, first_come_first_serve(arrayPtr, &result));
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}