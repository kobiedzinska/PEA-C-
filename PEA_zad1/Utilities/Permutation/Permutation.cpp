

#include "Permutation.h"


std::set<std::vector<int>> Permutation::permute(std::vector<int> input, int poz) {
    std::set<std::vector<int>> perm;

    if (poz == input.size()) {
        perm.insert(input);
    } else {

        for (int i = poz; i < input.size(); i++) {
            std::swap(input[i], input[poz]);
            auto nowe_perm = permute(input, poz + 1);
            perm.insert(nowe_perm.begin(), nowe_perm.end());
            std::swap(input[i], input[poz]);
        }
    }

    return perm;
}

std::set<std::vector<int>> Permutation::permutation_withoutcycles(std::vector<int> input) {
    std::vector<int> temp = std::vector<int>(1, input[0]);              //we keep the first vertex
    input.erase(input.begin());                           //then we erase it from the vector which we're going to permute

    std::set<std::vector<int>> result = permute(input);
    std::set<std::vector<int>> finalResult;


    for (std::vector<int> permutacja : result) {
        permutacja.insert(permutacja.begin(), temp[0]);  // we add 0 to the beginning
        finalResult.insert(permutacja);                   //we save the changes to the other set
    }

    return finalResult;                                 //we return
}
