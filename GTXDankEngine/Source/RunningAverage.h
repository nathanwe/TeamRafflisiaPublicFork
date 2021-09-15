#ifndef RUNNING_AVERAGE_H
#define RUNNING_AVERAGE_H

#include <cstdint>
#include <queue>

template <typename T, typename TDivisor = std::uint32_t>
class RunningAverage
{
public:
    RunningAverage(std::uint32_t numberTracked, T initialAverage)
        :
        average(initialAverage),
        lastTotal(average* (TDivisor)numberTracked),
        numberTracked(numberTracked)
    {
        while (numberTracked-- > 0)
            pastValues.push(average);
    }

    // Updates the running average, and returns it
    T Update(T value)
    {
        lastTotal -= pastValues.front();
        lastTotal += value;
        pastValues.pop();
        pastValues.push(value);
        average = lastTotal / (TDivisor)numberTracked;
        return average;
    }

    T Average() { return average; }

private:
    T average;
    T lastTotal;
    std::queue<T> pastValues;
    std::uint32_t numberTracked;
};


#endif
