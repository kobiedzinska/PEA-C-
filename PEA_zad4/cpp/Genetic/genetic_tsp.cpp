//
// Created by obied on 12.01.2025.
//

#include "genetic_tsp.h"

#include <algorithm>
#include <chrono>
#include <fstream>

double genetic_tsp::calculateFitness(std::vector<int> &chromosome) const {
    double totalDistance = 0.0;
    for (int i = 0; i < numCities; i++) {
        int city1 = chromosome[i];
        int city2 = chromosome[(i + 1) % numCities];
        totalDistance += matrix.getWeight(city1, city2);
    }
    return totalDistance;
}

std::vector<int> genetic_tsp::crossoverOX(const std::vector<int> &parent1, const std::vector<int> &parent2) {
    std::uniform_int_distribution<> dis(0, numCities - 1);
    int start = dis(gen);
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

    // Tworzenie mapowania
    for (int i = start; i <= end; i++) {
        int value1 = parent1[i];
        int value2 = parent2[i];
        mapping[value1] = value2;
        mapping[value2] = value1;
        child[i] = parent1[i];
    }

    // Rozwiązywanie konfliktów
    for (int i = 0; i < numCities; i++) {
        if (i < start || i > end) {
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

void genetic_tsp::mutationInsert(std::vector<int> &chromosome) {
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

    double bestFitness = std::numeric_limits<double>::infinity();
    int bestIndex = -1;

    for (int i = 0; i < tournamentSize; i++) {
        int idx = dis(gen);
        double fitness = calculateFitness(population[idx]);
        if (fitness < bestFitness) {
            bestFitness = fitness;
            bestIndex = idx;
        }
    }

    return population[bestIndex];
}

void genetic_tsp::generatePopulation() {
    population.clear();
    for (int i = 0; i < population_size; i++) {
        std::vector<int> chromosome(numCities);
        for (int j = 0; j < numCities; j++) {
            chromosome[j] = j;
        }
        shuffle(chromosome.begin(), chromosome.end(), gen);
        population.push_back(chromosome);
    }
}

void genetic_tsp::run() {
    auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = startTime;

        std::vector<int> bestSolution;
        double bestDistance = std::numeric_limits<double>::infinity();

        while (std::chrono::duration_cast<std::chrono::seconds>(
                currentTime - startTime).count() < timeLimit) {

            std::vector<std::vector<int>> newPopulation;

            // Elityzm
            int eliteIndex = 0;
            double eliteFitness = calculateFitness(population[0]);
            for (int i = 1; i < population_size; i++) {
                double fitness = calculateFitness(population[i]);
                if (fitness < eliteFitness) {
                    eliteFitness = fitness;
                    eliteIndex = i;
                }
            }
            newPopulation.push_back(population[eliteIndex]);

            // Generowanie nowej populacji
            while (newPopulation.size() < population_size) {
                std::vector<int> parent1 = tournamentSelection();
                std::vector<int> parent2 = tournamentSelection();
                std::vector<int> child;

                // Krzyżowanie
                std::uniform_real_distribution<> dis(0.0, 1.0);
                if (dis(gen) < crossover_rate) {
                    if (crossover_method == "OX") {
                        child = crossoverOX(parent1, parent2);
                    } else { // PMX
                        child = crossoverPMX(parent1, parent2);
                    }
                } else {
                    child = parent1;
                }

                // Mutacja
                if (mutation_method == "swap") {
                    mutationSwap(child);
                } else { // inversion
                    mutationInsert(child);
                }

                newPopulation.push_back(child);
            }

            population = std::move(newPopulation);

            // Aktualizacja statystyk
            double currentBestDistance = calculateFitness(population[eliteIndex]);
            if (currentBestDistance < bestDistance) {
                bestDistance = currentBestDistance;
                bestSolution = population[eliteIndex];
            }

            currentTime = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                currentTime - startTime).count();
            double relativeError = std::abs(currentBestDistance - optimal_distance) /
                                 optimal_distance;

            results.push_back({elapsed, relativeError});
        }
}

void genetic_tsp::saveResults(const std::string &filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "filename: " << filename << "\n";
        file << "\npopulation_size: " << population_size << "\n";

        file << "Time,RelativeError\n";
        for (const auto& result : results) {
            file << result.first << "," << result.second << "\n";
        }
        file.close();
    }
}

double genetic_tsp::getBestDistance() {
    double bestDist = std::numeric_limits<double>::infinity();
    for (auto& chromosome : population) {
        double dist = calculateFitness(chromosome);
        bestDist = std::min(bestDist, dist);
    }
    return bestDist;
}

std::vector<std::pair<double, double>> genetic_tsp::getEvolutionData() const {
    return results;
}

