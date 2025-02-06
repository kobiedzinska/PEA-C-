
#include "Timer.h"

LARGE_INTEGER Timer::startTimer() {
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);
    return start;
}

LARGE_INTEGER Timer::stopTimer() {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    return end;
}

long long int Timer::diffTime(LARGE_INTEGER start, LARGE_INTEGER end) {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER difference;
    difference.QuadPart = end.QuadPart - start.QuadPart;


    difference.QuadPart *= 1000000; //mikrosekundy  | *1000 milisekud
    difference.QuadPart /= frequency.QuadPart;

    return difference.QuadPart;
}
