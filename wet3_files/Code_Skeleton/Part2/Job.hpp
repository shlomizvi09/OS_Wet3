#ifndef __JOB_H
#define __JOB_H
#include "Thread.hpp"

class Job {
   private:
    int_mat *curr_board;
    int_mat *next_board;
    uint top_row;
    uint bottom_row;
    uint width;
    uint height;
    int phase;

   public:
    Job(int_mat *curr_board, int_mat *next_board,
        uint top_row, uint bottom_row, int phase);
    ~Job() {}
    void execute();
    uint get_num_of_live_neighbors(int i, int j);
    int get_dominant_neighbor_color(int i, int j);
    int get_average_neighbor_color(int i, int j);
    void give_birth(int i, int j);
    void survive(int i, int j);
    void kill(int i, int j);
    void phase_one_execute();
    void phase_two_execute();
};

#endif