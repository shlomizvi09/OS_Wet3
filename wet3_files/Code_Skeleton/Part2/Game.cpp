#include "Game.hpp"

#include <algorithm>
#include <cmath>

static const char* colors[7] = {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
Game::Game(game_params p) {
    m_gen_num = p.n_gen;
    m_thread_num = p.n_thread;
    interactive_on = p.interactive_on;
    print_on = p.print_on;
    params.filename = p.filename;
    params.interactive_on = p.interactive_on;
    params.n_gen = p.n_gen;
    params.n_thread = p.n_thread;
    params.print_on = p.print_on;
}

void Game::run() {
    _init_game();  // Starts the threads and all other variables you need
    print_board("Initial Board");
    for (uint i = 0; i < m_gen_num; ++i) {
        auto gen_start = std::chrono::system_clock::now();
        _step(i);  // Iterates a single generation
        auto gen_end = std::chrono::system_clock::now();
        m_gen_hist.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
        print_board(NULL);
    }  // generation loop
    print_board("Final Board");
    _destroy_game();
}

void Game::_init_game() {
    pthread_mutex_init(&m_lock, NULL);
    vector<string> board_lines = utils::read_lines(this->params.filename);
    vector<uint> tmp_row_curr;
    vector<uint> tmp_row_next;
    for (size_t i = 0; i < board_lines.size(); i++) {
        vector<string> line = utils::split(board_lines[i], ' ');
        board_width = line.size();
        for (size_t j = 0; j < board_width; j++) {
            uint tmp_cell = std::stoi(line[j]);
            tmp_row_curr.push_back(tmp_cell);
            tmp_row_next.push_back(0);
        }
        curr_board.push_back(tmp_row_curr);
        next_board.push_back(tmp_row_next);
        tmp_row_curr.clear();
        tmp_row_next.clear();
    }
    board_heigt = board_lines.size();
    m_thread_num = std::min(m_thread_num, board_heigt);
    for (uint i = 0; i < m_thread_num; i++) {
        Thread* temp_thread = new Thread_worker(i, &jobs_queue, &m_lock);
        m_threadpool.push_back(temp_thread);
    }

    for (size_t i = 0; i < m_thread_num; i++) {
        m_threadpool[i]->start();
    }
}

void Game::_step(uint curr_gen) {
    int working_threads = m_thread_num;
    int threads_to_divide = m_thread_num;
    int rows_to_divide = board_heigt;
    int start = 0;

    while (threads_to_divide > 0) {
        int tmp = floor(rows_to_divide / threads_to_divide);
        Job new_job = Job(&curr_board, &next_board, start, start + tmp - 1, board_width, board_heigt, 1, &working_threads);
        jobs_queue.push(new_job);
        start += tmp;
        rows_to_divide -= tmp;
        threads_to_divide -= 1;
    }

    while (working_threads != 0) {
        sleep(0);
    }
    working_threads = m_thread_num;
    threads_to_divide = m_thread_num;
    rows_to_divide = board_heigt;
    start = 0;

    while (threads_to_divide > 0) {
        int tmp = floor(rows_to_divide / threads_to_divide);
        Job new_job = Job(&next_board, &curr_board, start, start + tmp - 1, board_width, board_heigt, 2, &working_threads);
        start += tmp;
        jobs_queue.push(new_job);
        rows_to_divide -= tmp;
        threads_to_divide -= 1;
    }

    while (working_threads != 0) {
        sleep(0);
    }
}

void Game::_destroy_game() {
    int working_threads = m_thread_num;
    for (size_t i = 0; i < m_thread_num; i++) {
        jobs_queue.push(Job(nullptr, nullptr, 0, 0, 0, 0, -1, &working_threads));  //to stop thread execute
    }
    while (working_threads != 0) {
        sleep(0);
    }

    for (size_t i = 0; i < m_thread_num; i++) {
        m_threadpool[i]->join();
    }
    for (size_t i = 0; i < m_thread_num; i++) {
        delete m_threadpool[i];
    }
    pthread_mutex_destroy(&m_lock);
}

const vector<double> Game::gen_hist() const {
    return m_gen_hist;
}

const vector<double> Game::tile_hist() const {
    return m_tile_hist;
}

uint Game::thread_num() const {
    return m_thread_num;
}
/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {
    if (print_on) {
        // Clear the screen, to create a running animation
        if (interactive_on)
            system("clear");

        // Print small header if needed
        if (header != NULL)
            cout << "<------------" << header << "------------>" << endl;

        cout << u8"╔" << string(u8"═") * this->board_width << u8"╗" << endl;
        for (uint i = 0; i < this->board_heigt; ++i) {
            cout << u8"║";
            for (uint j = 0; j < this->board_width; ++j) {
                if (curr_board[i][j] > 0)
                    cout << colors[curr_board[i][j] % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
            }
            cout << u8"║" << endl;
        }
        cout << u8"╚" << string(u8"═") * this->board_width << u8"╝" << endl;

        // Display for GEN_SLEEP_USEC micro-seconds on screen
        if (interactive_on)
            usleep(GEN_SLEEP_USEC);
    }
}
