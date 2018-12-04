#include "timer.h"

Timer::Timer()
{
    startTime = std::chrono::system_clock::now();
}

long Timer::getElapsedMilliseconds() const
{
    auto curTime = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime).count();
}

void Timer::reset()
{
    startTime = std::chrono::system_clock::now();
}