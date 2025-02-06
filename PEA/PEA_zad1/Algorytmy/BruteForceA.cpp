
#include "BruteForceA.h"
#include <vector>
#include <algorithm> // next_permutation
#include <iostream>
#include "../Utilities/Permutation/Permutation.h"

int BruteForceA::bruteForce(const Matrix& matrix) {
    int firstPoint=0;
    int size = matrix.getSize();
    std::vector<int> vertexes;

    for (int i = 0; i < size; i++){
        if(i != firstPoint){
            vertexes.push_back(i);
        }
    }

    int minimal = INT_MAX;                                                  //minimal path weight

    do {
        int current = 0;                                                    //current path weight
        int v = firstPoint;

        for (int i = 0; i < vertexes.size(); i++) {
            current += matrix.getWeight(v, vertexes[i]);
            v = vertexes[i];                                                //set the current vertex to the next one

        }
        current += matrix.getWeight(v, firstPoint);

        if(current < minimal){                                             //check if the current path is shorter than the minimal path
            minimal= current;
        }

    } while(                                                              //loop until all permutations are generated
            next_permutation(vertexes.begin(), vertexes.end())         //generate permutations of the vector
            );

    return minimal;
}

int BruteForceA::bruteForceWCyc(const Matrix &matrix) {
    int firstPoint=0;
    int size = matrix.getSize();
    std::vector<int> vertexes;

    for (int i = 0; i < size; i++){
        if(i != firstPoint){
            vertexes.push_back(i);
        }
    }

    int minimal = INT_MAX;                                                  //minimal path weight
    Permutation permutator;
    std::set<std::vector<int>> permutacje = permutator.permutation_withoutcycles(vertexes);

    for(const auto& permutacja : permutacje) {
        int current = 0;                                                    //current path weight
        int v = firstPoint;

        for (int i = 0; i < vertexes.size(); i++) {
            current += matrix.getWeight(v, vertexes[i]);
            v = vertexes[i];                                                //set the current vertex to the next one

        }
        current += matrix.getWeight(v, firstPoint);

        if(current < minimal){                                             //check if the current path is shorter than the minimal path
            minimal= current;
        }

    }

    return minimal;
}

