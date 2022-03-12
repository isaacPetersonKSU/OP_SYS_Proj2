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

/*
TEST(load_pcb_valid, validfile) {
    const char * pcbfile = "mypcbfile.bin";
    int n = 3;

    FILE * fp = fopen(pcbfile, "rw");
    // creating our own pcb file
    [
    fwrite(n, ...)

    fwrite(10, ...)
    fwrite(0, ...)
    fwrite(0, ...)

    fwrite(5, ...)
    fwrite(0, ...)
    fwrite(15, ...)

    fclose();
    ]

    [
        uint32_t array[] = {3, 10, 0, 0, 20, 0 , 1, 5, 0, 2};
        fwrite(array, sizeof(array), 1, fp);
    ]

    dyn_array_t * d = load_process_control_blocks(pcbfile);
    ASSERT_NE(d, NULL);

    // check d->capacity
    // traverse trhough d->array and see if it is properly loaded
    // check d->data_size

    // if not right, don't add points
}

*/

// One bad parameter for each algorithm and then one valid test case for each algorithm
TEST (process_scheduling, loadPCB)
{
    // read, if return value is 0, then there is no data. if there is not enough processes, then it should fail as well
    EXPECT_EQ(nullptr, load_process_control_blocks(NULL));
    EXPECT_EQ(nullptr, load_process_control_blocks("doesnotexist.txt"));
    
    dyn_array_t *pcb = load_process_control_blocks("../pcb.bin");
    EXPECT_NE(nullptr, pcb);
    
    // Add score?
    // test individual values
}

TEST (process_scheduling, FCFS_validate_input)
{
    dyn_array_t * arrayPtr = dyn_array_create(0,0,NULL);
    ScheduleResult_t result;
    EXPECT_FALSE(first_come_first_serve(nullptr, &result));
    EXPECT_FALSE(first_come_first_serve(arrayPtr, nullptr));
    EXPECT_TRUE(first_come_first_serve(arrayPtr, &result));
}

TEST (process_scheduling, SJF_validate_input)
{
    dyn_array_t * arrayPtr = dyn_array_create(0,0,NULL);
    ScheduleResult_t result;
    EXPECT_FALSE(shortest_job_first(nullptr, &result));
    EXPECT_FALSE(shortest_job_first(arrayPtr, nullptr));
    EXPECT_TRUE(shortest_job_first(arrayPtr, &result));
}

TEST (process_scheduling, PRI_validate_input)
{
    dyn_array_t * arrayPtr = dyn_array_create(0,0,NULL);
    ScheduleResult_t result;
    EXPECT_FALSE(priority(nullptr, &result));
    EXPECT_FALSE(priority(arrayPtr, nullptr));
    EXPECT_TRUE(priority(arrayPtr, &result));
}

TEST (process_scheduling, RR_validate_input)
{
    dyn_array_t * arrayPtr = dyn_array_create(0,0,NULL);
    ScheduleResult_t result;
    EXPECT_FALSE(round_robin(nullptr, &result, QUANTUM));
    EXPECT_FALSE(round_robin(arrayPtr, nullptr, QUANTUM));
    EXPECT_FALSE(round_robin(arrayPtr, &result, 0));
    EXPECT_TRUE(round_robin(arrayPtr, &result, QUANTUM));
}

TEST (process_scheduling, SRTF_validate_input)
{
    dyn_array_t * arrayPtr = dyn_array_create(0,0,NULL);
    ScheduleResult_t result;
    EXPECT_FALSE(shortest_remaining_time_first(nullptr, &result));
    EXPECT_FALSE(shortest_remaining_time_first(arrayPtr, nullptr));
    EXPECT_TRUE(shortest_remaining_time_first(arrayPtr, &result));
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}