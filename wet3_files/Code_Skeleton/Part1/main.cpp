#include <pthread.h>
#include <vector>
#include "PCQueue.hpp"
#include "Semaphore.hpp"

#define THREAD_NUM 20

PCQueue<int> int_pc_queue;

void* produce(void* args) {
    int tmp = rand() % 100;
    std::cout << "thread: " << *(int*)args << " adding " << tmp << " to queue" << std::endl;
    int_pc_queue.push(tmp);
    delete(args);
}

void* consume(void* args) {
    std::cout << "thread: " << *(int*)args<<" trying to get item from queue" << std::endl;
    int tmp = int_pc_queue.pop();
    std::cout << "thread: " << *(int*)args << " got " << tmp << " from queue" << std::endl;
    delete(args);
}

int main() {
    PCQueue<int> int_pc_queue;
    std::cout << "started main" << std::endl;
    pthread_t th[THREAD_NUM];
    for (auto i = 0; i < THREAD_NUM; i++) {
        int* a = new int();
        *a = i;
        if (i % 3 != 0) {
            if (pthread_create(&th[i], NULL, &produce,a ) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consume, a) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (auto i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    return 0;
}