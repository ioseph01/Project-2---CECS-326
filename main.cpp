/*
Turns out, this is actually Dijkstra's solution where philosophers either take 
both when available, or wait.
*/

#include <pthread.h>
#include <time.h>
#include "DiningServer.h"
#include <iomanip>
#include <iostream>

#define FORK_COUNT 5
#define LOWER 1000
#define UPPER 3000

int ROUNDS = -1;

pthread_mutex_t mutex;
pthread_cond_t cond_var;

void sleep_ms(long milliseconds) {
    struct timespec req;
    req.tv_sec = milliseconds / 1000;                    
    req.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&req, nullptr);
}

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


    pthread_mutex_lock(&mutex);
    std::cout << "Philosopher #" << id << " is now Thinking\n";
    pthread_mutex_unlock(&mutex);
    
    while ((philosopher.rounds < ROUNDS && 0 < ROUNDS) || ROUNDS < 0) {
        if (philosopher.state == State::Thinking) {
            sleep_ms(philosopher.time);
            philosopher.state = State::Hungry;

            pthread_mutex_lock(&mutex);
            std::cout << "Philosopher #" << id << " thought for " 
            << std::fixed << std::setprecision(2) << philosopher.time / 1000.0
            << " seconds and is now Hungry\n";
            pthread_mutex_unlock(&mutex);

        }
        else if (philosopher.state == State::Eating) {
            sleep_ms(philosopher.time);
            pthread_mutex_lock(&mutex);

            server->returnForks(id);
            std::cout << "Philosopher #" << id << " has ate for " 
            << std::fixed << std::setprecision(2) << philosopher.time / 1000.0
            << " seconds and is now Thinking\n";
            pthread_cond_broadcast(&cond_var);

            pthread_mutex_unlock(&mutex);

            philosopher.state = State::Thinking;
            ++philosopher.rounds;
            philosopher.time = random_long(LOWER, UPPER);

                
        }
       
        else if (philosopher.state == State::Hungry) {
            pthread_mutex_lock(&mutex);

            while (!server->bothAvailable(id)) {
                pthread_cond_wait(&cond_var, &mutex);
            }
            server->takeForks(id);
            std::cout << "Philosopher #" << id << " is now Eating\n";
            
            pthread_mutex_unlock(&mutex);
            philosopher.state = State::Eating;
            philosopher.time = random_long(LOWER, UPPER);
        }
        
    }

    return nullptr;
}



int main(int argc, char* argv[]) {
    if (argc > 1) {
        try {
            ROUNDS = std::stoi(argv[1]);
            std::cout << "Argument received; launching with " << ROUNDS << " rounds.\n";
        }
        catch (...) { std::cerr << "Error with argument; launching with infinite rounds.\n"; }
    }
    std::cout << "Starting Dining Philosophers with Dijkstra's Solution!\n";


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
