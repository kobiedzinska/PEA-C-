
#ifndef PEA_ZAD1_STOPER_H
#define PEA_ZAD1_STOPER_H
#include <windows.h>

class Stoper {
public:
    LARGE_INTEGER stopTimer();

    LARGE_INTEGER startTimer();

    long long int diffTime(LARGE_INTEGER start, LARGE_INTEGER end);
};


#endif //PEA_ZAD1_STOPER_H
