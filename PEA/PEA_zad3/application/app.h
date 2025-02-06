

#ifndef APP_H
#define APP_H
#include <string>
#include <vector>


class app {
public:
    static void runConfig();
    static void runTests();
    static void saveResults(std::string filename, std::vector<int> path, int cost, int cost2);

};



#endif //APP_H
