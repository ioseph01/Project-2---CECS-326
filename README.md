# Project-2---CECS-326


How it works so far:

Main DiningServer Server acts as a fork/chopsticks & philosopher manager.
Forks/chopsticks are stored in a vector and claimed by philosophers by turning into the philosopher id.

In main, a philosopher process defines the lifetime and decision tree of each philosopher. Uses Dijkstra's solution where philosophers only get forks when both are available.

Also in main, the number of rounds defines how long the philosophers will simulate a Thinking-Hungry-Eating cycle - `#define ROUNDS -1` - with positive integers defining a finite number of rounds and -1 or negative integers defining and infinite number.

Compile with your C++ compiler of choice: `g++ main.cpp DiningServer.cpp -o main` and run with `./main`.

**Note:** There are optional arguments to run with to specify the number of simulated rounds with the default being infinite. Example: `./main 10` for 10 rounds.