#ifndef __JOB_H
#define __JOB_H
//#include "Game.hpp"
#include "Thread.hpp"

class Job {
   public:
    int_mat *curr_board;
    int_mat *next_board;
    uint top_row;
    uint bottom_row;
    uint width;
    uint height;
    int phase;

    Job(int_mat *curr_board, int_mat *next_board,
        uint top_row, uint bottom_row, uint width, uint height, int phase);
    Job(const Job &old_job);
    ~Job() {}

    uint get_num_of_live_neighbors(int i, int j);
    int get_dominant_neighbor_color(int i, int j);
    int get_average_neighbor_color(int i, int j);
    void give_birth(int i, int j);
    void survive(int i, int j);
};

#endif