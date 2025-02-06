//
// Created by obied on 12.01.2025.
//

#include "genetic_tsp.h"

#include <algorithm>
#include <chrono>
#include <fstream>

int genetic_tsp::calculateFitness(const std::vector<int> &chromosome) const {
    int totalDistance = 0;
    for (int i = 0; i < numCities; i++) {
        int city1 = chromosome[i];
        int city2 = chromosome[(i + 1) % numCities];
        totalDistance += matrix.getWeight(city1, city2);
    }
    return totalDistance;
}

std::vector<int> genetic_tsp::crossoverOX(const std::vector<int> &parent1, const std::vector<int> &parent2) {
    std::uniform_int_distribution<> dis(0, numCities - 1);
    int start = dis(gen);       // losujemy miasta
    int end = dis(gen);
    if (start > end) std::swap(start, end);

    std::vector<int> child(numCities, -1);
    std::vector<bool> used(numCities, false);

    // Kopiowanie segmentu z pierwszego rodzica
    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
        used[parent1[i]] = true;
    }
    // Wypełnianie pozostałych pozycji
    int currentPos = (end + 1) % numCities;
    int parent2Pos = (end + 1) % numCities;

    while (currentPos != start) {
        while (used[parent2[parent2Pos]]) {
            parent2Pos = (parent2Pos + 1) % numCities;
        }
        child[currentPos] = parent2[parent2Pos];
        used[parent2[parent2Pos]] = true;
        currentPos = (currentPos + 1) % numCities;
    }
    return child;
}

std::vector<int> genetic_tsp::crossoverPMX(const std::vector<int> &parent1, const std::vector<int> &parent2) {
    std::uniform_int_distribution<> dis(0, numCities - 1);
    int start = dis(gen);
    int end = dis(gen);
    if (start > end) std::swap(start, end);

    std::vector<int> child(parent2);
    std::vector<int> mapping(numCities, -1);

    // tworzenie mapowania miast w środku
    for (int i = start; i <= end; i++) {
        int value1 = parent1[i];
        int value2 = parent2[i];
        mapping[value1] = value2;   // miasto1 -> miasto2
        mapping[value2] = value1;   // miasto2 -> miasto1
        child[i] = parent1[i];  // podmiana
    }
    // rozwiązywanie konfliktów przy mapowaniu
    for (int i = 0; i < numCities; i++) {
        if (i < start || i > end) { // tylko dla pozycji poza środkiem
            int current = child[i];
            while (mapping[current] != -1) {
                current = mapping[current];
            }
            child[i] = current;
        }
    }
    return child;
}

void genetic_tsp::mutationSwap(std::vector<int> &chromosome) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if (dis(gen) < mutation_rate) {
        std::uniform_int_distribution<> posDis(0, numCities - 1);
        int pos1 = posDis(gen);
        int pos2 = posDis(gen);
        std::swap(chromosome[pos1], chromosome[pos2]);
    }
}

void genetic_tsp::mutationInversion(std::vector<int> &chromosome) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if (dis(gen) < mutation_rate) {
        std::uniform_int_distribution<> posDis(0, numCities - 1);
        int start = posDis(gen);
        int end = posDis(gen);
        if (start > end) std::swap(start, end);

        std::reverse(chromosome.begin() + start,
                    chromosome.begin() + end + 1);
    }
}

std::vector<int> genetic_tsp::tournamentSelection() {
    const int tournamentSize = 3;
    std::uniform_int_distribution<> dis(0, population_size - 1);

    int bestFitness = std::numeric_limits<double>::infinity();
    int bestIndex = -1;

    for (int i = 0; i < tournamentSize; i++) {
        int idx = dis(gen);
        int fitness = calculateFitness(population[idx]);
        if (fitness < bestFitness) {
            bestFitness = fitness;
            bestIndex = idx;
        }
    }

    return population[bestIndex];
}


int genetic_tsp::run() {
    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = startTime;

    std::vector<int> bestSolution;
    int bestDistance = std::numeric_limits<int>::infinity();

    while (std::chrono::duration_cast<std::chrono::seconds>(
            currentTime - startTime).count() < timeLimit) {

        std::vector<std::vector<int>> newPopulation;

        // Sortujemy populację według fitness
        std::vector<std::pair<int, int>> populationFitness;
        for (int i = 0; i < population_size; i++) {
            populationFitness.push_back({calculateFitness(population[i]), i});
        }
        std::sort(populationFitness.begin(), populationFitness.end());

        // Zachowujemy tylko 10% najlepszych rozwiązań
        int eliteCount = population_size * 0.1;
        for (int i = 0; i < eliteCount; i++) {
            newPopulation.push_back(population[populationFitness[i].second]);
        }

        // Generujemy 40% całkowicie nowych losowych rozwiązań
        int randomCount = population_size * 0.4;
        for (int i = 0; i < randomCount; i++) {
            std::vector<int> newChromosome(numCities);
            std::iota(newChromosome.begin(), newChromosome.end(), 0);
            std::shuffle(newChromosome.begin(), newChromosome.end(), gen);
            newPopulation.push_back(newChromosome);
        }

        // Resztę populacji generujemy przez krzyżowanie
        while (newPopulation.size() < population_size) {
            std::vector<int> parent1 = tournamentSelection();
            std::vector<int> parent2 = tournamentSelection();
            std::vector<int> child;

            std::uniform_real_distribution<> dis(0.0, 1.0);
            if (dis(gen) < crossover_rate) {
                if (crossover_method == "OX") {
                    child = crossoverOX(parent1, parent2);
                } else {
                    child = crossoverPMX(parent1, parent2);
                }
            } else {
                child = parent1;
            }

            if (mutation_method == "swap") {
                mutationSwap(child);
            } else {
                mutationInversion(child);
            }

            newPopulation.push_back(child);
        }

        population = std::move(newPopulation);

        // Aktualizacja najlepszego rozwiązania
        int currentBestDistance = populationFitness[0].first;
        if (currentBestDistance < bestDistance) {
            bestDistance = currentBestDistance;
            bestSolution = population[populationFitness[0].second];
        }

        currentTime = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            currentTime - startTime).count();
        double relativeError = std::abs(static_cast<double>(currentBestDistance - optimal_distance)) / optimal_distance;

        results.push_back({elapsed, relativeError, bestSolution});
    }
    return bestDistance;
}

void genetic_tsp::saveResults(const std::string &filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "filename: " << filename << "\n";
        file << "population_size: " << population_size << "\n";
        file << "Time,RelativeError,Path\n";
        for (const auto& result : results) {
            file << result.time << "," << result.error << ",";
            for (size_t i = 0; i < result.path.size(); i++) {
                file << result.path[i];
                if (i < result.path.size() - 1) file << "-";
            }
            file << "\n";
        }
        file.close();
    }
}

int genetic_tsp::getBestDistance() {
    int bestDist = std::numeric_limits<int>::infinity();
    for (auto& chromosome : population) {
        int dist = calculateFitness(chromosome);
        bestDist = std::min(bestDist, dist);
    }
    return bestDist;
}

std::vector<genetic_tsp::Result> genetic_tsp::getEvolutionData() const {
    std::vector<genetic_tsp::Result> evolution_data;
    for (const auto& result : results) {
        evolution_data.push_back({result.time, result.error, result.path});
    }
    return evolution_data;
}

void genetic_tsp::setTimeLimit(double timeLimit) {
    this->timeLimit= timeLimit;
}

