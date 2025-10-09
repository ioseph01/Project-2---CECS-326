# Project-2---CECS-326


How it works so far:

Main DiningServer Server acts as a fork/chopsticks & philosopher manager.
Forks/chopsticks are stored in a vector and claimed by philosophers by turning into the philosopher id.

In main, a philosopher process defines the lifetime and decision tree of each philosopher.

Also in main, the number of rounds is currently 10: `#define ROUNDS 10`.

Compile with your C++ compiler of choice: `g++ main.cpp DiningServer.cpp -o main` and run with `./main`.
