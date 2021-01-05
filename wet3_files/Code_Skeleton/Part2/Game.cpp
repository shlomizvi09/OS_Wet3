#include "Game.hpp"

static const char* colors[7] = {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
/*--------------------------------------------------------------------------------

--------------------------------------------------------------------------------*/
Game::Game(game_params p) : params(p) {}

void Game::run() {
    _init_game();  // Starts the threads and all other variables you need
    print_board("Initial Board");
    for (uint i = 0; i < m_gen_num; ++i) {
        auto gen_start = std::chrono::system_clock::now();
        _step(i);  // Iterates a single generation
        auto gen_end = std::chrono::system_clock::now();
        m_gen_hist.push_back((float)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
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
    m_gen_num = params.n_gen;
    m_thread_num = params.n_thread;
    interactive_on = params.interactive_on;
    print_on = params.print_on;
    vector<string> board_lines = utils::read_lines(this->params.filename);
    for (size_t i = 0; i < board_lines.size(); i++) {
        vector<string> line = utils::split(board_lines[i], ' ');
        for (size_t j = 0; i < line.size(); j++) {
            uint tmp_cell = std::stoi(line[j]);
            curr_board[i].push_back(tmp_cell);
            next_board[i].push_back(0);
        }
        board_width = line.size();
    }
    board_heigt = board_lines.size();

    for (uint i = 0; i < m_thread_num; i++) {
        Thread* temp_thread = new Thread_worker(i, jobs_queue);
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
}

void Game::_destroy_game() {
    // Destroys board and frees all threads and resources
    // Not implemented in the Game's destructor for testing purposes.
    // Testing of your implementation will presume all threads are joined here
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
