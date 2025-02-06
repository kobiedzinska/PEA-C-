//
// Created by obied on 12.01.2025.
//

#ifndef GENETIC_TSP_H
#define GENETIC_TSP_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <random>

#include "../Utilities/Matrix/Matrix.h"
#include "../Utilities/Parser/Parser.h"


class genetic_tsp {
private:
    Matrix matrix;
    int population_size;
    std::vector<std::vector<int>> population;
    double crossover_rate;
    double mutation_rate;
    int optimal_distance;
    int numCities;
    std::string crossover_method;
    std::string mutation_method;
    double timeLimit;   // ewentualnie można podmienić na liczbę iteracji
    std::vector<int> bestSolution;  // jako pole klasy

    // Generator liczb losowych
    std::random_device rd;
    std::mt19937 gen;

public:
    // Wektor do przechowywania wyników (do WYKRESU)
    struct Result {
        double time;
        double error;
        std::vector<int> path;
    };
    std::vector<Result> results;

    genetic_tsp(const std::string& xmlFile, int popSize,
            double crossRate, double mutRate,
            std::string crossMethod, std::string mutMethod)
    : population_size(popSize), crossover_rate(crossRate),
      mutation_rate(mutRate),
      crossover_method(crossMethod), mutation_method(mutMethod),
      gen(rd()) {

    // Wczytaj graf z pliku i stwórz macierz
    try {
        Graph graph = Parser::loadGraphFromXML(xmlFile);
        matrix.fromGraph(graph);
        numCities = matrix.getSize();

        // Ustaw parametry na podstawie nazwy pliku
        if(xmlFile == "ftv47.xml") {
            optimal_distance = 1776;
            timeLimit = 60;
        } else if(xmlFile == "ftv170.xml") {
            optimal_distance = 2755;
            timeLimit = 180;
        } else if (xmlFile == "rbg403.xml") {
            optimal_distance = 2465;
            timeLimit = 240;
        } else {
            std::cerr << "Wyodrębniona nazwa pliku: " << xmlFile << std::endl;
            throw std::invalid_argument("xmlFile does not exist");
        }

        // Zainicjuj populację początkową
        population.clear();
        for (int i = 0; i < population_size; i++) {
            std::vector<int> chromosome(numCities);
            for (int j = 0; j < numCities; j++) {
                chromosome[j] = j;
            }
            std::shuffle(chromosome.begin(), chromosome.end(), gen);
            population.push_back(chromosome);
        }

    } catch (const std::exception& e) {
        std::cerr << "Błąd podczas inicjalizacji: " << e.what() << std::endl;
        std::cerr << "Nazwa pliku: " << xmlFile << std::endl;
        throw;
    }
}
    // obliczanie fitness
    int calculateFitness(const std::vector<int>& chromosome) const;

    // Metody krzyżowania Order Crossover oraz PMX (częściowe dopasowanie)
    std::vector<int> crossoverOX(const std::vector<int>& parent1,
                                 const std::vector<int>& parent2);
    std::vector<int> crossoverPMX(const std::vector<int>& parent1,
                                  const std::vector<int>& parent2);
    // Metody mutacji
    void mutationSwap(std::vector<int>& chromosome);
    void mutationInversion(std::vector<int>& chromosome);

    //Selekcja
    std::vector<int> tournamentSelection();
  

    int run();
    void saveResults(const std::string& filename) const;
    int getBestDistance();
    std::vector<Result> getEvolutionData() const;
    void setTimeLimit(double timeLimit);
    std::vector<int> getBestPath() const { return bestSolution; }

};



#endif //GENETIC_TSP_H
