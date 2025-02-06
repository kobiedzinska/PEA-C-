

#ifndef APP_H
#define APP_H
#include <utility>
#include <string>
#include "../../Utilities/Macierz/Matrix.h"


class App {
    public:
        static void runConfig();
        static void saveResults(int cost, const std::vector<int>& path, const std::string &filename, const std::string &alg); //reading from file
        static void saveResults2(int cost, const std::vector<int>& path, int size, bool isSymmetric, const std::string &alg); //generating

        static void runTest();

};



#endif //APP_H
