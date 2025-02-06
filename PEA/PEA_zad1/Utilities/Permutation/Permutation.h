

#ifndef PERMUTATION_H
#define PERMUTATION_H
#include <set>
#include <vector>


class Permutation {
public:
    static std::set<std::vector<int>> permute(std::vector<int> input, int poz = 0);
    static std::set<std::vector<int>> permutation_withoutcycles(std::vector<int> input);
};


#endif //PERMUTATION_H
