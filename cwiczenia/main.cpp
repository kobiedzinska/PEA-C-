#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <limits>

class SimulatedAnnealing {
private:
    std::vector<std::pair<double, double>> cities;
    std::vector<int> currentPath;
    std::random_device rd;
    std::mt19937 gen;

    double initialTemperature;
    double alpha;
    int maxIterations;
    double timeLimit;

    // Funkcja obliczająca odległość między dwoma miastami
    double distanceBetweenCities(int city1, int city2) {
        double dx = cities[city1].first - cities[city2].first;
        double dy = cities[city1].second - cities[city2].second;
        return sqrt(dx * dx + dy * dy);
    }

    double calculateDistance(const std::vector<int>& path) {
        double totalDistance = 0.0;
        for (size_t i = 0; i < path.size(); i++) {
            totalDistance += distanceBetweenCities(path[i], path[(i + 1) % path.size()]);
        }
        return totalDistance;
    }

    // Generowanie rozwiązania początkowego metodą zachłanną (najbliższego sąsiada)
    std::vector<int> generateGreedyInitialSolution() {
        int n = cities.size();
        std::vector<int> path;
        std::vector<bool> visited(n, false);

        // Rozpocznij od losowego miasta
        std::uniform_int_distribution<> dis(0, n - 1);
        int currentCity = dis(gen);
        path.push_back(currentCity);
        visited[currentCity] = true;

        // Dla każdego pozostałego miasta
        while (path.size() < n) {
            double minDistance = std::numeric_limits<double>::max();
            int nearestCity = -1;

            // Znajdź najbliższe nieodwiedzone miasto
            for (int nextCity = 0; nextCity < n; nextCity++) {
                if (!visited[nextCity]) {
                    double distance = distanceBetweenCities(currentCity, nextCity);
                    if (distance < minDistance) {
                        minDistance = distance;
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

    std::vector<int> generateNeighbor(const std::vector<int>& path) {
        std::vector<int> newPath = path;
        std::uniform_int_distribution<> dis(0, path.size() - 1);

        int i = dis(gen);
        int j = dis(gen);
        std::swap(newPath[i], newPath[j]);

        return newPath;
    }

public:
    SimulatedAnnealing(const std::vector<std::pair<double, double>>& cityList,
                       double initTemp = 1000.0,
                       double alphaCoefficient = 0.995,
                       int maxIter = 10000,
                       double timeLimitSeconds = 60.0)
        : cities(cityList),
          gen(rd()),
          initialTemperature(initTemp),
          alpha(alphaCoefficient),
          maxIterations(maxIter),
          timeLimit(timeLimitSeconds) {

        // Generuj rozwiązanie początkowe metodą zachłanną
        currentPath = generateGreedyInitialSolution();
        double initialDistance = calculateDistance(currentPath);
        std::cout << "Odleglosc rozwiazania zachlannego: " << initialDistance << std::endl;
    }

    std::pair<std::vector<int>, double> solve() {
        auto startTime = std::chrono::steady_clock::now();

        std::vector<int> bestPath = currentPath;
        double currentDistance = calculateDistance(currentPath);
        double bestDistance = currentDistance;
        double temperature = initialTemperature;

        std::uniform_real_distribution<> probability(0.0, 1.0);

        for (int iter = 0; iter < maxIterations && temperature > 0.1; iter++) {
            auto currentTime = std::chrono::steady_clock::now();
            double elapsedSeconds = std::chrono::duration<double>(currentTime - startTime).count();

            if (elapsedSeconds >= timeLimit) {
                std::cout << "Osiagnieto limit czasu (" << timeLimit << " sekund)\n";
                break;
            }

            std::vector<int> neighborPath = generateNeighbor(currentPath);
            double neighborDistance = calculateDistance(neighborPath);

            double deltaE = neighborDistance - currentDistance;

            if (deltaE < 0 || probability(gen) < exp(-deltaE / temperature)) {
                currentPath = neighborPath;
                currentDistance = neighborDistance;

                if (currentDistance < bestDistance) {
                    bestDistance = currentDistance;
                    bestPath = currentPath;
                }
            }

            temperature = alpha * temperature;

            if (iter % 1000 == 0) {
                std::cout << "Iteracja " << iter << ": Temperatura = " << temperature
                         << ", Najlepsza odleglosc = " << bestDistance << std::endl;
            }
        }

        auto endTime = std::chrono::steady_clock::now();
        double totalTime = std::chrono::duration<double>(endTime - startTime).count();
        std::cout << "Calkowity czas wykonania: " << totalTime << " sekund\n";

        return {bestPath, bestDistance};
    }
};

int main() {
    std::vector<std::pair<double, double>> cities = {
        {0, 0}, {2, 4}, {3, 1}, {5, 3}, {1, 5},
        {4, 6}, {5, 2}, {2, 3}, {6, 5}, {4, 1}
    };

    SimulatedAnnealing sa(cities, 1000.0, 0.995, 10000, 60.0);
    auto [bestPath, bestDistance] = sa.solve();

    std::cout << "Najlepsza znaleziona trasa:\n";
    for (int city : bestPath) {
        std::cout << city << " -> ";
    }
    std::cout << bestPath[0] << std::endl;
    std::cout << "Calkowita dlugosc trasy: " << bestDistance << std::endl;

    return 0;
}