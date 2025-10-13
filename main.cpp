#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "DiningServer.h"
#include <iostream>

#define ROUNDS 10
#define FORK_COUNT 5
#define LOWER 3
#define UPPER 5
/*
Turns out, this is actually Dijkstra's solution where philosophers either take 
both when available, or wait.
*/

pthread_mutex_t mutex;
pthread_cond_t cond_var;

// Philosopher thread wrapper
struct ThreadParam {
    DiningServer* server;
    int id;
};

// The main philosopher decision tree
void* philosopherProcess(void* arg) {
    ThreadParam* param = (ThreadParam*) arg;
    DiningServer* server = param->server;
    int id = param->id;
    DiningServer::Philosopher& philosopher = server->philosophers[id];

    while (philosopher.rounds < ROUNDS) {
        if (philosopher.state == State::Thinking) {
            sleep(1);
            --philosopher.time;
            if (philosopher.time <= 0) {
                philosopher.time = random_int(LOWER,UPPER);
                philosopher.state = State::Hungry;

                pthread_mutex_lock(&mutex);
                std::cout << "Philosopher " << id << " is now Hungry\n";
                pthread_mutex_unlock(&mutex);

            }
        }
        else if (philosopher.state == State::Eating) {
            sleep(1);
            --philosopher.time;
            if (philosopher.time <= 0) {
                pthread_mutex_lock(&mutex);

                server->returnForks(id);
                std::cout << "Philosopher " << id << " is now Thinking\n";
                pthread_cond_broadcast(&cond_var);

                pthread_mutex_unlock(&mutex);
                philosopher.state = State::Thinking;
                ++philosopher.rounds;
                philosopher.time = random_int(LOWER,UPPER);
            }
        }
       
        else if (philosopher.state == State::Hungry) {
            pthread_mutex_lock(&mutex);

            while (!server->bothAvailable(id)) {
                pthread_cond_wait(&cond_var, &mutex);
            }
            server->takeForks(id);
            std::cout << "Philosopher " << id << " is now Eating\n";
            
            pthread_mutex_unlock(&mutex);
            philosopher.state = State::Eating;
        }
        
    }

    return nullptr;
}



int main() {
    std::cout << "Starting Program!\n";
    DiningServer server;
    std::vector<ThreadParam> params;
    for (int i = 0; i < 5; ++i) {
        params.push_back({&server, i});
    }
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond_var, nullptr);

    std::vector<pthread_t> threads(5);
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], nullptr, philosopherProcess, &params[i]);
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}
