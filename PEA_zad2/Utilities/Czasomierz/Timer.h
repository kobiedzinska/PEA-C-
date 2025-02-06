

#ifndef TIMER_H
#define TIMER_H
#include <windows.h>


class Timer {
public:
    LARGE_INTEGER stopTimer();

    LARGE_INTEGER startTimer();

    long long int diffTime(LARGE_INTEGER start, LARGE_INTEGER end);
};



#endif //TIMER_H
