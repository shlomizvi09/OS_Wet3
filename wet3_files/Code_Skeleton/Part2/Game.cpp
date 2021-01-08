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
    cout << "starting init" << endl;
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
    // Create threads
    // Create game fields
    // Start the threads
    // Testing of your implementation will presume all threads are started here
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
            cout << "pushed element " << j << " to line " << i << endl;
        }
        cout << "attemp to push vecor to matrix" << endl;
        curr_board.push_back(tmp_row_curr);
        next_board.push_back(tmp_row_next);
        tmp_row_curr.clear();
        tmp_row_next.clear();
    }
    board_heigt = board_lines.size();
    cout << "finished board initialize" << endl;
    for (uint i = 0; i < m_thread_num; i++) {
        Thread* temp_thread = new Thread_worker(i, &jobs_queue);
        m_threadpool.push_back(temp_thread);
    }

    for (size_t i = 0; i < m_thread_num; i++) {
        m_threadpool[i]->start();
    }
}

void Game::_step(uint curr_gen) {
    // Push jobs to queue
    // Wait for the workers to finish calculating
    // Swap pointers between current and next field
    int working_threads = m_thread_num;
    // int num_of_jobs_base = ceil((double)board_heigt / m_thread_num);
    // for (size_t i = 0; i < board_heigt; i += num_of_jobs_base) {
    //     uint bottom = std::min((int)board_heigt, (int)(i + num_of_jobs_base)) - 1;
    //     uint top = i;
    //     Job new_job = Job(&curr_board, &next_board, top, bottom, board_width, board_heigt, 1, &working_threads);
    //     jobs_queue.push(new_job);
    // }

    int quanta = floor((double)board_heigt / m_thread_num);
    int approx = quanta * m_thread_num;
    uint bottom = quanta;
    uint top = 0;
    for (size_t i = 1; i < m_thread_num; i++) {
        Job new_job = Job(&curr_board, &next_board, top, bottom - 1, board_width, board_heigt, 1, &working_threads);
        jobs_queue.push(new_job);
        bottom += quanta;
        top += quanta;
    }
    Job new_job = Job(&curr_board, &next_board, top, board_heigt - 1, board_width, board_heigt, 1, &working_threads);
    jobs_queue.push(new_job);

    while (working_threads != 0) {
        sleep(0);
    }
    working_threads = m_thread_num;
    // for (size_t i = 0; i < board_heigt; i += num_of_jobs_base) {
    //     uint bottom = std::min((int)board_heigt, (int)(i + num_of_jobs_base)) - 1;
    //     uint top = i;
    //     Job new_job = Job(&next_board, &curr_board, top, bottom, board_width, board_heigt, 2, &working_threads);
    //     jobs_queue.push(new_job);
    // }
    bottom = quanta;
    top = 0;
    for (size_t i = 1; i < m_thread_num; i++) {
        Job new_job = Job(&next_board, &curr_board, top, bottom - 1, board_width, board_heigt, 2, &working_threads);
        jobs_queue.push(new_job);
        bottom += quanta;
        top += quanta;
    }
    new_job = Job(&next_board, &curr_board, top, board_heigt - 1, board_width, board_heigt, 2, &working_threads);
    jobs_queue.push(new_job);

    while (working_threads != 0) {
        sleep(0);
    }

    // Phase 1 //

    // TODO: assign each job to a thread

    /*
    1. create a jobs_queue with phase = 1
    2. create threads and give each thread a job
    3. call job->execute
    4. wait for all threads to finish
    5. clear the queue
    6. repeat step 1 with phase = 2 or change all jobs->phase in jobs_queue to 2
    */
}

void Game::_destroy_game() {
    // Destroys board and frees all threads and resources
    // Not implemented in the Game's destructor for testing purposes.
    // Testing of your implementation will presume all threads are joined here
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

        cout << u8"╔" << string(u8"═") * this->board_heigt << u8"╗" << endl;
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
