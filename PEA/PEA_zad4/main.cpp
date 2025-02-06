
#include <iostream>
#include <vector>

#include "cpp/Genetic/genetic_tsp.h"

void testCrossoverMutationMethods(const std::string& xmlFile, double maxTime) {
    int populationSize = 100;
    double crossoverRate = 0.8;
    double mutationRate = 0.01;

    std::vector<std::tuple<std::string, std::string, std::string>> combinations = {
        {"OX-swap", "OX", "swap"},
        {"OX-inversion", "OX", "inversion"},
        {"PMX-swap", "PMX", "swap"},
        {"PMX-inversion", "PMX", "inversion"}
    };

    for (const auto& [label, crossover, mutation] : combinations) {
        std::cout << "Testing: " << label << std::endl;
        genetic_tsp ga(xmlFile, populationSize, crossoverRate, mutationRate,
                       crossover, mutation);
        ga.run();
        ga.saveResults(label + "_results.csv");
    }
}

void testPopulationSizes(const std::string& xmlFile, double maxTime) {
    std::vector<int> populationSizes = {50, 100, 200};
    double crossoverRate = 0.8;
    double mutationRate = 0.01;

    std::vector<std::pair<std::string, std::string>> methods = {
        {"OX", "swap"}, {"OX", "inversion"},
        {"PMX", "swap"}, {"PMX", "inversion"}
    };

    for (int size : populationSizes) {
        for (const auto& [crossover, mutation] : methods) {
            std::string label = crossover + "-" + mutation + "-pop" +
                              std::to_string(size);
            std::cout << "Testing: " << label << std::endl;

            genetic_tsp ga(xmlFile, size, crossoverRate, mutationRate,
                          crossover, mutation);
            ga.run();
            ga.saveResults(label + "_results.csv");
        }
    }
}

int main() {
    std::vector<std::string> instances = {
        "ftv47.xml",
        "ftv170.xml",
        "rbg403.xml"
    };
    double maxTime = 60.0; // sekundy

    for (const std::string& instance : instances) {
        std::cout << "Testing instance: " << instance << std::endl;

        // a) Porównanie metod krzyżowania i mutacji
        testCrossoverMutationMethods(instance, maxTime);

        // b) Analiza wpływu wielkości populacji
        testPopulationSizes(instance, maxTime);
    }

    return 0;
}