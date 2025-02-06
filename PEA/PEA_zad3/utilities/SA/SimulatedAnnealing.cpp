

#include "SimulatedAnnealing.h"

#include <chrono>
#include <fstream>

int SimulatedAnnealing::calculateDistance(const std::vector<int> &path) {
    int totalDistance = 0;
    if (path.size() < 2) return totalDistance; // Jeśli za mało wierzchołków, brak podróży

    // Oblicz koszty kolejnych krawędzi
    for (int i = 0; i < path.size() - 1; i++) {
        totalDistance += matrix.getWeight(path[i], path[i + 1]);
    }
    totalDistance += matrix.getWeight(path.back(), path.front()); // dodaje koszt powrotu

    return totalDistance;
}


std::vector<int> SimulatedAnnealing::generateInitialSolution() {
    int n = matrix.getSize();
    std::vector<int> path;
    std::vector<bool> visited(n, false);

    // Rozpocznij od losowego miasta
    std::uniform_int_distribution<> dis(0, n - 1);
    int currentCity = dis(GEN);
    path.push_back(currentCity);
    visited[currentCity] = true;

    // Dla każdego pozostałego miasta sprawdź ścieżkę
    while (path.size() < n) {
        double minimum = std::numeric_limits<double>::max();
        int nearestCity = -1;

        // Znajdź najbliższe nieodwiedzone miasto
        for (int nextCity = 0; nextCity < n; nextCity++) {
            if (!visited[nextCity]) {
                int distance = matrix.getWeight(currentCity, nextCity);
                if (distance < minimum) {
                    minimum = distance;
                    nearestCity = nextCity;
                }
            }
        }

        // Dodaj najbliższe miasto do ścieżki
        path.push_back(nearestCity);
        visited[nearestCity] = true;
        currentCity = nearestCity;
    }

    return path;
}

double SimulatedAnnealing::calculateInitialTemperature() {
    int n = (matrix.getSize() <= 48) ? 50 :
            (matrix.getSize() <= 171) ? 100 : 200;

    double deltaSum = 0;
    std::vector<int> currentSol = currentPath;
    int currentCost = calculateDistance(currentSol);

    for(int i = 0; i < n; i++) {
        std::vector<int> neighborSol = generateNeighbor(currentSol, strategy);
        int neighborCost = calculateDistance(neighborSol);
        deltaSum += std::abs(neighborCost - currentCost);
    }

    double avgDelta = deltaSum / n;
    double p0 = 0.8; // prawdopodobieństwo początkowe

    return -avgDelta / std::log(p0);
}


// Metoda 0: Invert
std::vector<int> SimulatedAnnealing::generateNeighborInvert(const std::vector<int> &path) {
    std::vector<int> alterPath = path;
    int size = path.size();

    // Wybierz dwa różne punkty
    std::uniform_int_distribution<> gen1(0, size - 2);
    int i = gen1(GEN);
    std::uniform_int_distribution<> gen2(i + 1, size - 1);
    int j = gen2(GEN);

    // Odwróć segment między i a j
    std::reverse(alterPath.begin() + i, alterPath.begin() + j + 1);

    return alterPath;
}
// Metoda 1: Select - zamiana dwóch elementów miejscami
std::vector<int> SimulatedAnnealing::generateNeighborSelect(const std::vector<int> &path) {
    std::vector<int> alterPath = path;
    int size = path.size();

    // Wybierz dwa różne punkty
    std::uniform_int_distribution<> gen(0, size - 1);
    int i = gen(GEN);
    int j;
    do {
        j = gen(GEN);
    } while (i == j);

    // Zamień miejscami elementy
    std::swap(alterPath[i], alterPath[j]);

    return alterPath;
}
// Metoda 2: Insert - wstawienie elementu w nowe miejsce
std::vector<int> SimulatedAnnealing::generateNeighborInsert(const std::vector<int> &path) {
    std::vector<int> alterPath = path;
    int size = path.size();

    // Wybierz element do przeniesienia i nową pozycję
    std::uniform_int_distribution<> gen(0, size - 1);
    int from = gen(GEN);
    int to;
    do {
        to = gen(GEN);
    } while (from == to);

    // Przenieś element
    if (from < to) {
        std::rotate(alterPath.begin() + from,
                   alterPath.begin() + from + 1,
                   alterPath.begin() + to + 1);
    } else {
        std::rotate(alterPath.begin() + to,
                   alterPath.begin() + from,
                   alterPath.begin() + from + 1);
    }

    return alterPath;
}

std::vector<int> SimulatedAnnealing::generateNeighbor(const std::vector<int> &path, int strategy) {
    switch(strategy) {
        case 0:
            return generateNeighborInvert(path);
        case 1:
            return generateNeighborSelect(path);
        case 2:
            return generateNeighborInsert(path);
        default:
            // Losowy wybór strategii
                std::uniform_int_distribution<> gen(0, 2);
        return generateNeighbor(path, gen(GEN));
    }
}

struct SolutionInfo {
    std::vector<int> path;
    int distance;
    double timeToFind;
    double totalTime;
};

std::pair<std::vector<int>, int> SimulatedAnnealing::solve(int it) {

    int vertices = matrix.getSize();
    int timeLimit;
    if (vertices <= 48) timeLimit = 120;      // 2 minutes
    else if (vertices <= 171) timeLimit = 240; // 4 minutes
    else timeLimit = 360;                      // 6 minutes
    double totalTime;

    auto startTime = std::chrono::steady_clock::now();
    double bestSolutionTime = 0.0;  // Time when the best solution was found

    std::vector<int> bestPath = currentPath;
    double currentDistance = calculateDistance(currentPath);
    double bestDistance = currentDistance;
    double temperature = initialTemperature;

    initialCost = currentDistance;

    std::uniform_real_distribution<> r(0.0, 1.0);
    GEN.seed(std::random_device{}());
    //int iter=0;
    //for (int iter = 0; iter < maxIterations && temperature > 0.1; iter++) {
    while (true) {
        //iter++;
        auto currentTime = std::chrono::steady_clock::now();
        double elapsedSeconds = std::chrono::duration<double>(currentTime - startTime).count();

        if (elapsedSeconds >= timeLimit) break;

        if (temperature < 0.1) {
            //currentPath = generateInitialSolution();
            currentDistance = calculateDistance(currentPath);
            temperature = initialTemperature;
        }

        std::vector<int> neighborPath = generateNeighbor(currentPath, strategy);
        double neighborDistance = calculateDistance(neighborPath);
        double delta = neighborDistance - currentDistance;

        if (delta < 0 || r(GEN) < exp(-delta / temperature * 5)) {
            currentPath = neighborPath;
            currentDistance = neighborDistance;

            if (currentDistance < bestDistance) {
                bestDistance = currentDistance;
                bestPath = currentPath;
                bestSolutionTime = elapsedSeconds;  // Record when we found the best solution

                //std::cout << "Nowe najlepsze rozwiazanie znalezione po " << bestSolutionTime
                       //  << " sekundach. Koszt: " << bestDistance << std::endl;
            }
        }

        temperature = a * temperature;

        //if (iter % 1000 == 0) {
            //std::cout << "Iteracja " << iter << ": Temperatura = " << temperature
                     //<< ", Najlepsza odleglosc = " << bestDistance
                     //<< ", Czas = " << elapsedSeconds << "s" << std::endl;
       // }
    }

    auto endTime = std::chrono::steady_clock::now();
    totalTime = std::chrono::duration<double>(endTime - startTime).count();


    double blad_wzgledny = std::fabs(bestDistance - initialCost) / initialCost * 100;

    saveResults(it, vertices, bestDistance, bestSolutionTime, totalTime, blad_wzgledny);

    return {bestPath, bestDistance};
}

int SimulatedAnnealing::getInitialCost() {
    return initialCost;
}

void SimulatedAnnealing::saveResults(int it, int vertices, int bestCost, double bestTime, double totalTime, double blad) {
    std::ofstream outFile("wyniki_99.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << it << " "<<vertices << "," << bestCost << "," << initialCost <<"," << bestTime << "," << totalTime << "," <<blad << "\n";
        outFile.close();
    }
}




