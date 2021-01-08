#include "Job.hpp"

#include <algorithm>

Job::Job(int_mat *curr_board, int_mat *next_board,
         uint top_row, uint bottom_row, uint width, uint height, int phase, int *working_threads) : curr_board(curr_board),
                                                                                                    next_board(next_board),
                                                                                                    top_row(top_row),
                                                                                                    bottom_row(bottom_row),
                                                                                                    width(width),
                                                                                                    height(height),
                                                                                                    phase(phase),
                                                                                                    working_threads(working_threads) {
}

Job::Job(const Job &old_job) : top_row(old_job.top_row), bottom_row(old_job.bottom_row) {
    this->curr_board = old_job.curr_board;
    this->next_board = old_job.next_board;
    this->width = old_job.width;
    this->height = old_job.height;
    this->phase = old_job.phase;
    this->working_threads = old_job.working_threads;
}

uint Job::get_num_of_live_neighbors(int i, int j) {
    int res = 0;
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height - 1); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width - 1); column++) {
            if ((row != i || column != j) && (*curr_board)[row][column] != 0) {
                res++;
            }
        }
    }
    return res;
}

int Job::get_dominant_neighbor_color(int i, int j) {
    int colors_hist[7] = {0};
    int res = 0;
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height - 1); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width - 1); column++) {
            if ((row != i || column != j) && (*curr_board)[row][column] > 0) {
                int tmp_color = (*curr_board)[row][column];
                colors_hist[tmp_color] += tmp_color;
            }
        }
    }
    for (size_t i = 0; i < 7; i++) {
        if (colors_hist[i] > res) {
            res = i;
        }
    }
    return res;
}

int Job::get_average_neighbor_color(int i, int j) {
    int res = 0;
    int num_of_live_neighbors = get_num_of_live_neighbors(i, j) + 1;
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height - 1); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width - 1); column++) {
            res += (*curr_board)[row][column];
        }
    }
    return (int)(round((double)res / (double)num_of_live_neighbors));
}

void Job::give_birth(int i, int j) {
    (*next_board)[i][j] = get_dominant_neighbor_color(i, j);
}

void Job::survive(int i, int j) {
    (*next_board)[i][j] = (*curr_board)[i][j];
}

void Job::kill(int i, int j) {
    (*next_board)[i][j] = 0;
}

void Job::execute() {
    if (phase == 1) {
        phase_one_execute();
        return;
    } else if (phase == 2) {
        phase_two_execute();
        return;
    } else {
        return;
    }
}

void Job::phase_one_execute() {
    for (uint i = top_row; i <= bottom_row; i++) {
        for (uint j = 0; j < width; j++) {
            if (j == width - 1) {
            }
            uint tmp_cell = (*curr_board)[i][j];
            uint live_neighbors = get_num_of_live_neighbors(i, j);
            if (tmp_cell == 0) {
                if (live_neighbors == 3) {
                    give_birth(i, j);
                } else {
                    kill(i, j);
                }
            } else {
                if (live_neighbors == 2 || live_neighbors == 3) {
                    survive(i, j);
                } else {
                    kill(i, j);
                }
            }
        }
    }
}

void Job::phase_two_execute() {
    for (uint i = top_row; i <= bottom_row; i++) {
        for (uint j = 0; j < width; j++) {
            uint tmp_cell = (*curr_board)[i][j];
            if (tmp_cell == 0) {
                kill(i, j);
            } else {
                (*next_board)[i][j] = get_average_neighbor_color(i, j);
            }
        }
    }
}