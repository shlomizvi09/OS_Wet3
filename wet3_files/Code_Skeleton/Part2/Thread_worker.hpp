#ifndef __THREAD_WORKER_H
#define __THREAD_WORKER_H
#include "Headers.hpp"
#include "PCQueue.hpp"
#include "Job.hpp"
#include "Thread.hpp"

class Thread_worker : public Thread {
   private:
    PCQueue<Job> *jobs_queue;

   public:
    Thread_worker(uint thread_id, PCQueue<Job> *jobs_queue) : Thread(thread_id), jobs_queue(jobs_queue) {}
    ~Thread_worker() {}

    void thread_workload() override {  
        while (true)
        {   
            Job tmp_job=jobs_queue->pop();
            if (tmp_job.phase == -1){
                (*tmp_job.working_threads)--;
                return;
            }
            tmp_job.execute();
            (*tmp_job.working_threads)--;
        }
        
    }
};

#endif
