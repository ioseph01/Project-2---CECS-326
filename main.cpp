#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

#define ROUNDS 1
#define FORK_COUNT 5
/*
Asymmetric solution is to have the even numbered
philosophers pick up in left-right order, while
odd-numbered pick up in right-left order
*/

int forks[5] = {-1,-1,-1,-1,-1};

enum class Parity {Even, Odd};
enum class State {Hungry, Eating, Thinking};
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void print_forks() {
    for (int i = 0; i < 5; i++) {
        std::cout << forks[i] << ",";
    }
    std::cout << "\n";
}

typedef struct {
    int id;
    int eating_time, thinking_time, rounds;
    bool right, left;
    State state;
    Parity const parity() { return (id % 2 == 0 ? Parity::Even : Parity::Odd);}
} ThreadParam;

int random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void* main_thread(void* arg) {
    ThreadParam* param = (ThreadParam*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (param->rounds < ROUNDS) {

             if (param->state == State::Thinking) {
                // simulate thinking
                param->thinking_time--;
                
                pthread_mutex_unlock(&mutex);
                sleep(1);
                pthread_mutex_lock(&mutex);
                if (param->thinking_time <= 0) {
                    param->state = State::Hungry;
                    std::cout << param->id << "# thread is hungry\n";

                }
            } else if (param->state == State::Hungry) {
                std::cout << param->id << "# is Checking forks: ";
                print_forks();
                if (param->right && param->left) {
                    param->eating_time = random_int(0,3);
                    param->state = State::Eating;
                    break;
                }
                else if (param->parity() == Parity::Even) {
                    if (not param->left) {
                        if (forks[param->id] == -1 || forks[param->id] == param->id) {
                            forks[param->id] = param->id;
                            param->left = true;
                        }
                    }
                    else if (not param->right) {
                        int idx = (param->id + 1) % FORK_COUNT;
                        if (forks[idx] == -1 || forks[idx] == param->id) {
                            forks[idx] = param->id;
                            param->right = true;
                        }
                    }
                }
                else {
                    
                    if (not param->right) {
                        int idx = (param->id + 1) % FORK_COUNT;
                        if (forks[idx] == -1) {
                            forks[idx] = param->id;
                            param->right = true;
                        }
                    }
                    if (not param->left) {
                        if (forks[param->id] == -1) {
                            forks[param->id] = param->id;
                            param->left = true;
                        }
                    }
                }
                if (param->right and param->left) { 
                    param->eating_time = random_int(0,3);
                    param->state = State::Eating;
                    break;
                 }

            } else if (param->state == State::Eating || (forks[param->id] == param->id && forks[(param->id + 1) % FORK_COUNT] == param->id)) { break; }
        }

        if (param->rounds >= ROUNDS) {
            pthread_mutex_unlock(&mutex);
            pthread_cond_broadcast(&cond_var);
            break;
        }
        // EAT up
        printf("%d# thread eating..\n", param->id);
        pthread_cond_broadcast(&cond_var);
        --param->eating_time;
        if (param->eating_time <= 0) {
            param->state = State::Thinking;
            param->thinking_time = random_int(2,5);
            param->right = param->left = false;
            param->rounds += 1;
            forks[param->id] = forks[(param->id + 1) % FORK_COUNT] = -1;
            std::cout << param->id << "# thread is done eating.\n";
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);

    }
    return NULL;
}

int main() {
    srand(time(NULL));
    std::cout << "Starting Program!\n";
    pthread_t t0, t1, t2, t3, t4;
    ThreadParam p0{0, 0, random_int(0,5),0,false,false,State::Thinking};
    ThreadParam p1{1, 0, random_int(0,5),0,false,false,State::Thinking};
    ThreadParam p2{2, 0, random_int(0,5),0,false,false,State::Thinking};
    ThreadParam p3{3, 0, random_int(0,5),0,false,false,State::Thinking};
    ThreadParam p4{4, 0, random_int(0,5),0,false,false,State::Thinking};

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    pthread_create(&t0, NULL, main_thread, &p0);
    pthread_create(&t1, NULL, main_thread, &p1);
    pthread_create(&t2, NULL, main_thread, &p2);
    pthread_create(&t3, NULL, main_thread, &p3);
    pthread_create(&t4, NULL, main_thread, &p4);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}
