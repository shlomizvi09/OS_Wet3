#ifndef __JOB_H
#define __JOB_H
#include "Game.hpp"
#include "Thread.hpp"

class Job {
   private:
    const matrix &curr_board;
    matrix &next_board;
    uint top_row;
    uint bottom_row;
    uint width;
    uint height;

   public:
    Job(const matrix &curr_board, matrix &next_board,
        uint top_line, uint bottom_line);
    ~Job() {}

    uint get_num_of_live_neighbors(int i, int j);
    int get_dominant_neighbor_color(int i, int j);
    int get_average_neighbor_color(int i, int j);
    void give_birth(int i, int j);
    void survive(int i, int j);
};

Job::Job(/* args */) {
}

Job::~Job() {
}

#endif