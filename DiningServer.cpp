#include "DiningServer.h"
#include <iostream>
#include <random>


long random_long(long min, long max) {
    static std::random_device rd {};           
    static std::mt19937_64 engine {rd()};    
    std::uniform_int_distribution<long> result {min, max};
    return result(engine);
}

int DiningServer::Philosopher::m_next_id = 0;

DiningServer::Philosopher::Philosopher() : id {m_next_id}, 
    time {random_long(1000,3000)}, state {State::Thinking}, rounds {0} 
    { m_next_id++; }

void DiningServer::takeLeftFork(int philosopherNumber) {
    if (forks[philosopherNumber] == philosopherNumber || forks[philosopherNumber] == -1) {
        forks[philosopherNumber] = philosopherNumber;
        std::cout << "\tFork " << philosopherNumber << " is with Philosopher #" << philosopherNumber 
        << "\n";
    }
}

void DiningServer::takeRightFork(int philosopherNumber) {
    int right = (philosopherNumber + 1) % forks.size();
    if (forks[right] == -1 || forks[right] == philosopherNumber) {
        forks[right] = philosopherNumber;
        std::cout << "\tFork " << right << " is with Philosopher #" << philosopherNumber 
        << "\n";
    }
}

void DiningServer::returnForks(int philosopherNumber) {
    forks[philosopherNumber] = -1;
    forks[(philosopherNumber + 1) % forks.size()] = -1;
}

bool DiningServer::leftAvailable(int philosopherNumber) {
    return (forks[philosopherNumber] == philosopherNumber || forks[philosopherNumber] == -1);
}

bool DiningServer::rightAvailable(int philosopherNumber) {
    int right = (philosopherNumber + 1) % forks.size();
    return (forks[right] == philosopherNumber || forks[right] == -1);
}

void DiningServer::print_forks() {
    std::cout << "[";
    for (const auto i : forks) {
        std::cout << i << ",";
    }
    std::cout << "]\n";
}


