

#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <iostream>
#include <random>

#include "../structure/Matrix.h"

class SimulatedAnnealing {
private:
    Matrix matrix;
    double initialTemperature;
    double a;  // Współczynnik α ze wzoru T(i+1) = α * T(i)
    int maxIterations;
    double timeLimit;
    int strategy;

    int initialCost;
    std::vector<int> currentPath;

    // do perturb function
    std::random_device rd;
    std::mt19937 GEN;


    int calculateDistance(const std::vector<int>& path); // oblicza koszt ścieżki
    std::vector<int> generateInitialSolution(); // Algorytm NN
    double calculateInitialTemperature();
    std::vector<int> generateNeighbor(const std::vector<int> &path, int strategy); // generuje podobne rozwiązania, strategy = sposób
    std::vector<int> generateNeighborInvert(const std::vector<int> &path); // strategy 1
    std::vector<int> generateNeighborSelect(const std::vector<int> &path); // strategy 2
    std::vector<int> generateNeighborInsert(const std::vector<int> &path); // strategy 3


public:
    SimulatedAnnealing(Matrix matrix,
                       int maxIterations,
                       double timeLimit,
                       double a,
                       int strategy):
    matrix(matrix),
    maxIterations(maxIterations),
    timeLimit(timeLimit),
    a(a),
    strategy(strategy)
    {
        // Wygenerowania rozwiązania początkowego metodą zachłanną NN
        currentPath = generateInitialSolution();
        initialTemperature = calculateInitialTemperature();
    };

    std::pair<std::vector<int>, int> solve(int it); // zwraca ścieżkę i koszt
    int getInitialCost();

    void saveResults(int it, int vertices, int bestCost, double bestTime, double totalTime, double blad);
};



#endif //SIMULATEDANNEALING_H
