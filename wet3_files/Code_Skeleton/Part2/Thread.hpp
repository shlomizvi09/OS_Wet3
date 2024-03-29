#ifndef __THREAD_H
#define __THREAD_H
#include "Headers.hpp"
#include "Job.hpp"

class Thread {
   public:
    Thread(uint thread_id) : my_thread_id(thread_id) {}
    virtual ~Thread() {}  // Does nothing

    /** Returns true if the thread was successfully started, false if there was an error starting the thread */
    bool start() {
        return pthread_create(&m_thread, nullptr, entry_func, (void *)this) == 0;
    }

    /** Will not return until the internal thread has exited. */
    void join() {
        pthread_join(m_thread, nullptr);
    }

    /** Returns the thread_id **/
    uint get_thread_id() {
		return my_thread_id;
    }

   protected:
    /** Implement this method in your subclass with the code you want your thread to run. */
    virtual void thread_workload() = 0;
    uint my_thread_id;  // A number from 0 -> Number of threads initialized, providing a simple numbering for you to use

   private:
    static void *entry_func(void *thread) {
        ((Thread *)thread)->thread_workload();
        return NULL;
    }
    pthread_t m_thread;
};

#endif
