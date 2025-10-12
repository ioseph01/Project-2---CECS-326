#include "DiningServer.h"

int random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int DiningServer::Philosopher::m_next_id = 0;

DiningServer::Philosopher::Philosopher() : id {m_next_id}, 
    time {10}, state {State::Thinking}, rounds {0}
    { m_next_id++; }

void DiningServer::takeForks(int philosopherNumber) {
    takeLeftFork(philosopherNumber);
    takeRightFork(philosopherNumber);
}

void DiningServer::takeLeftFork(int philosopherNumber) {
    if (forks[philosopherNumber] == philosopherNumber || forks[philosopherNumber] == -1) {
        forks[philosopherNumber] = philosopherNumber;
    }
}

void DiningServer::takeRightFork(int philosopherNumber) {
    int right = (philosopherNumber + 1) % forks.size();
    if (forks[right] == -1 || forks[right] == philosopherNumber) {
        forks[right] = philosopherNumber;
    }
}

void DiningServer::returnForks(int philosopherNumber) {
    forks[philosopherNumber] = -1;
    forks[(philosopherNumber + 1) % forks.size()] = -1;
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