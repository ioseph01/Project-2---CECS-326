#include "DiningServer.h"

int random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int DiningServer::Philosopher::m_next_id = 0;

DiningServer::Philosopher::Philosopher() : id {m_next_id}, 
    time {random_int(1,5)}, state {State::Thinking}, right {false}, left {false},
    rounds {0}    { m_next_id++; }

/* Even: left-right; Odd: right-left */
void DiningServer::takeForks(int philosopherNumber) {
    takeLeftFork(philosopherNumber);
    takeRightFork(philosopherNumber);
}

void DiningServer::takeLeftFork(int philosopherNumber) {
    if (forks[philosopherNumber] == philosopherNumber || forks[philosopherNumber] == -1) {
        philosophers[philosopherNumber].left = true;
        forks[philosopherNumber] = philosopherNumber;
    }
}

void DiningServer::takeRightFork(int philosopherNumber) {
    int right = (philosopherNumber + 1) % forks.size();
    if (forks[right] == -1 || forks[right] == philosopherNumber) {
        philosophers[philosopherNumber].right = true;
        forks[right] = philosopherNumber;
    }
}

void DiningServer::returnForks(int philosopherNumber) {
    forks[philosopherNumber] = -1;
    forks[(philosopherNumber + 1) % forks.size()] = -1;
    philosophers[philosopherNumber].right = false;
    philosophers[philosopherNumber].left = false;
}

bool DiningServer::bothAvailable(int philosopherNumber) {
    int right = (philosopherNumber + 1) % forks.size();
    return (forks[right] == philosopherNumber || forks[right] == -1)
    &&
    (forks[philosopherNumber] == philosopherNumber || forks[philosopherNumber] == -1)
    ;
}

void DiningServer::print_forks() {
    std::cout << "[";
    for (const auto i : forks) {
        std::cout << i << ",";
    }
    std::cout << "]\n";
}