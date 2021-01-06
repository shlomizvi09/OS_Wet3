#ifndef __JOB_H
#define __JOB_H
//#include "Game.hpp"
#include "Thread.hpp"

class Job {
   private:
    const int_mat &curr_board;
    int_mat &next_board;
    uint top_row;
    uint bottom_row;
    uint width;
    uint height;
    int phase;

   public:
    Job(const int_mat &curr_board, int_mat &next_board,
        uint top_row, uint bottom_row) : curr_board(curr_board), next_board(next_board), top_row(top_row), bottom_row(bottom_row), width(width), height(height) {}
    ~Job() {}

    uint get_num_of_live_neighbors(int i, int j);
    int get_dominant_neighbor_color(int i, int j);
    int get_average_neighbor_color(int i, int j);
    void give_birth(int i, int j);
    void survive(int i, int j);
};

#endif