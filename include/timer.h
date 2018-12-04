#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
    public:
    Timer();

    long getElapsedMilliseconds() const;

    void reset();

    private:
    std::chrono::time_point<std::chrono::system_clock> startTime; 
};

#endif