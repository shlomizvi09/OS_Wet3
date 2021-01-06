#include "Semaphore.hpp"

Semaphore::Semaphore() : counter(0) {
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&condition, NULL);
}

Semaphore::Semaphore(unsigned int val) : counter(val) {
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&condition, NULL);
}

Semaphore::~Semaphore() {
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&condition);
}

void Semaphore::down() {
    pthread_mutex_lock(&m_lock);
    while (counter == 0) {
        std::cout<<"waiting"<<std::endl;
        pthread_cond_wait(&condition, &m_lock);
    }
    counter--;
    pthread_mutex_unlock(&m_lock);
}

void Semaphore::up() {
    pthread_mutex_lock(&m_lock);
    counter++;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&m_lock);
}