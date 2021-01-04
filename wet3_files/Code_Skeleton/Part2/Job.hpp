#ifndef __JOB_H
#define __JOB_H
#include "Game.hpp"
#include "Thread.hpp"

class Job {
   private:
    const vector<vector<uint>> &curr_board;
    vector<vector<uint>> next_board;
    uint top_line;
    uint bottom_line;
    uint width;
    uint height;

   public:
    Job(/* args */);
    ~Job();
};

Job::Job(/* args */) {
}

Job::~Job() {
}

#endif