#ifndef __THREAD_WORKER_H
#define __THREAD_WORKER_H
#include "Headers.hpp"
#include "PCQueue.hpp"
#include "Job.hpp"
#include "Thread.hpp"

class Thread_worker : public Thread {
   private:
    PCQueue<Job> *jobs_queue;
    pthread_mutex_t *m_lock;

   public:
    Thread_worker(uint thread_id, PCQueue<Job> *jobs_queue, pthread_mutex_t *m_lock) : Thread(thread_id), jobs_queue(jobs_queue), m_lock(m_lock) {}
    ~Thread_worker() {}

    void thread_workload() override {  
        while (true)
        {   
            Job tmp_job=jobs_queue->pop();
            if (tmp_job.phase == -1){
                pthread_mutex_lock(m_lock);
                (*tmp_job.working_threads)--;
                pthread_mutex_unlock(m_lock);
                return;
            }
            tmp_job.execute();
            pthread_mutex_lock(m_lock);
            (*tmp_job.working_threads)--;
            pthread_mutex_unlock(m_lock);
        }
        
    }
};

#endif
