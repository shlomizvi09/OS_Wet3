#include "Job.hpp"

#include <algorithm>

Job::Job(int_mat *curr_board, int_mat *next_board,
         uint top_row, uint bottom_row, uint width, uint height, int phase) : curr_board(curr_board),
                                                                              next_board(next_board),
                                                                              top_row(top_row),
                                                                              bottom_row(bottom_row),
                                                                              width(width),
                                                                              height(height),
                                                                              phase(phase) {
}

Job::Job(const Job &old_job) {
    this->curr_board = old_job.curr_board;
    this->next_board = old_job.next_board;
    this->top_row = old_job.top_row;
    this->bottom_row = old_job.bottom_row;
    this->width = old_job.width;
    this->height = old_job.height;
    this->phase = old_job.phase;
}

uint Job::get_num_of_live_neighbors(int i, int j) {
    int res = 0;
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width); column++) {
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
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width); column++) {
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
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width); column++) {
            res += (*curr_board)[row][column];
        }
    }
    return (int)(round((double)res / (double)num_of_live_neighbors));
}

void Job::give_birth(int i, int j) {
    (*next_board)[i][j] = get_dominant_neighbor_color(i, j);
}

void Job::survive(int i, int j) {
    (*next_board)[i][j] = get_average_neighbor_color(i, j);
}