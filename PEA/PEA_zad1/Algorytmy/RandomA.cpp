
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include "RandomA.h"

int RandomA::randomPO(const Matrix &matrix, const int bruteForceCost) {
    int current = 0;
    int size = matrix.getSize();
    std::vector<int> vertexes(size);                  // tworzymy wektor na wylosowaną kolejność miast
    int counter = 0;
    int minimal = INT_MAX;
    do {

        for (int i = 0; i < size; ++i) {
            vertexes[i] = i;
        }

        shuffle(vertexes.begin(), vertexes.end(), std::mt19937(std::random_device()()));    // we shuffle the vector

        for (int i = 0; i < size - 1; i++) {
            int j = vertexes[i];
            int k = vertexes[i + 1];
            current += matrix.getWeight(j, k);
        }

        current += matrix.getWeight(vertexes[size - 1], vertexes[0]);            // we add the last edge to the first vertex

       if (0 < current < minimal) {
           minimal = current;
       }
        current = 0;
    counter++;

    }
    while (minimal > bruteForceCost*1.25 && counter < 1000);
    return minimal;
}
