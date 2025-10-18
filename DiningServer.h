#pragma once

#include <vector>
#include <chrono>


enum class State {Thinking, Hungry, Eating};

long random_long(long min, long max);

struct DiningServer {
    
        
    struct Philosopher {
        Philosopher();
        State state {};
        int id {}, rounds{};
        long time{};
            
    private:
        static int m_next_id;
    };

    /* Called by a philosopher when it wishes to eat */
    void takeForks(int philosopherNumber);
    void takeLeftFork(int philosopherNumber);
    void takeRightFork(int philosopherNumber);
    /* Called by a philosopher when it is finished eating */
    void returnForks(int philosopherNumber);
    bool bothAvailable(int philosopherNumber);
    std::vector<Philosopher> philosophers = std::vector<Philosopher>(5);
    std::vector<int> forks = std::vector<int>(5,-1);
    void print_forks();
};

