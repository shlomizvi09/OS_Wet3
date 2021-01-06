#ifndef _QUEUEL_H
#define _QUEUEL_H
#include "Headers.hpp"
#include "Semaphore.hpp"

// Single Producer - Multiple Consumer queue
template <class T>
class PCQueue {
   public:
    PCQueue();
    ~PCQueue();
    // Blocks while queue is empty. When queue holds items, allows for a single
    // thread to enter and remove an item from the front of the queue and return it.
    // Assumes multiple consumers.
    T pop();

    // Allows for producer to enter with *minimal delay* and push items to back of the queue.
    // Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.
    // Assumes single producer
    void push(const T& item);

   private:
    // Add your class memebers here
    Semaphore sem;
    std::queue<T> T_queue;
    unsigned int waiting_producers;
    pthread_mutex_t m_lock;
    pthread_cond_t push_cond;
};
// Recommendation: Use the implementation of the std::queue for this exercise

template <class T>
PCQueue<T>::PCQueue() : sem(0), waiting_producers(0) {
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&push_cond, NULL);
}

template <class T>
PCQueue<T>::~PCQueue() {
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&push_cond);
}

template <class T>
void PCQueue<T>::push(const T& item) {
    waiting_producers++;  // adding a producer to the other wating
    pthread_mutex_lock(&m_lock);
    T_queue.push(item);
    waiting_producers--;
    if (waiting_producers == 0) {
        pthread_cond_broadcast(&push_cond);
    }
	sem.up();             // increasing the semaphore after really adding element
    pthread_mutex_unlock(&m_lock);
}

template <class T>
T PCQueue<T>::pop() {
    sem.down();                      // blocking if queue is empty
    pthread_mutex_lock(&m_lock);     // block other consumers or producers
    while (waiting_producers > 0) {  // priorety to producers
        pthread_cond_wait(&push_cond, &m_lock);
    }
    T item = T_queue.front();
    T_queue.pop();
    pthread_mutex_unlock(&m_lock);
	return item;
}
#endif