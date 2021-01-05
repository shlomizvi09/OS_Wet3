#include "Job.hpp"

#include <algorithm>

Job::Job(const int_mat &curr_board, int_mat &next_board,
         uint top_line, uint bottom_line) : curr_board(curr_board),
                                            next_board(next_board),
                                            top_row(top_row),
                                            bottom_row(bottom_row) {
}

uint Job::get_num_of_live_neighbors(int i, int j) {
    int res = 0;
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width); column++) {
            if ((row != i || column != j) && curr_board[row][column] != 0) {
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
            if ((row != i || column != j) && curr_board[row][column] > 0) {
                colors_hist[curr_board[row][column]]++;
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
    int num_of_live_neighbors = get_num_of_live_neighbors(i, j);
    for (int row = std::max(0, i - 1); row <= std::min(i + 1, (int)height); row++) {
        for (int column = std::max(0, j - 1); column <= std::min(j + 1, (int)width); column++) {
            res += curr_board[row][column];
        }
    }
    return (res / num_of_live_neighbors);
}

void Job::give_birth(int i, int j) {
    next_board[i][j] = get_dominant_neighbor_color(i, j);
}

void Job::survive(int i, int j) {
    next_board[i][j] = get_average_neighbor_color(i, j);
}