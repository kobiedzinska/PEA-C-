
#ifndef PEA_ZAD1_APPLICATION_H
#define PEA_ZAD1_APPLICATION_H
#include <string>


class Application {
    public:
    static void run();

    static int bf(const std::string &fileName);
    static int bf(int size, bool symmetric);

    static void nn(const std::string &fileName);
    static void nn(int size, bool symmetric);

    static void rnd(const std::string& fileName);
    static void rnd(int size, bool symmetric);

    static void all(const std::string &fileName);
    static void all(int size, bool symmetric);
};


#endif //PEA_ZAD1_APPLICATION_H
