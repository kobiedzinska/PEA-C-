

#include "app.h"

#include <fstream>
#include <iostream>

#include "../utilities/SA/SimulatedAnnealing.h"
#include "../utilities/parser/Parser.h"

void app::runConfig() {
    std::string file_name;
    double initTemp;
    int maxIterations;
    double timeLimit;
    double a;
    int strategy;

    std::cout<<"Running config app"<<std::endl;

    std::string filename = "config.txt";
    std::ifstream file;
    file.open(filename);
    if(!file.is_open()){
        std::cout << "File not found" << std::endl;
        file.close();
        return;
    }
    if(file.is_open()) {
        std::cout<<"File opened"<<std::endl;

        file >> file_name;
        file >> initTemp;
        file >> maxIterations;
        file >> timeLimit;
        file >> a;
        file >> strategy;

    }
    file.close();

    std::cout << "Configurations loaded." << std::endl;
    std::cout<<"File name: " << file_name<<std::endl;
    std::cout<<"Initialized Temperature: " << initTemp<<std::endl;
    std::cout<<"Max Iterations: " << maxIterations<<std::endl;
    std::cout<<"Time limit: " << timeLimit<<" [s]"<<std::endl;
    std::cout<<"Cooling factor: "<<a<<std::endl;
    std::cout << std::endl;

    // Odczytanie macierzy z pliku
    Matrix matrix;
    try {
        auto graph = Parser::loadGraphFromXML(file_name);
        matrix.fromGraph(graph);
    } catch(std::exception& e) {
        std::cout<<e.what()<<std::endl;
    }

    // Wyłowanie algorytmu
    SimulatedAnnealing sa (matrix, maxIterations, timeLimit, a, strategy);
    auto [bestPath, bestDistance] = sa.solve(1);
    std::cout << std::endl;
    std::cout << "Najlepsza znaleziona trasa:\n";
    for (int city : bestPath) {
        std::cout << city << " -> ";
    }
    std::cout << bestPath[0] << std::endl;
    std::cout << "Calkowita dlugosc trasy: " << bestDistance << std::endl;
    std::cout << "Blad wzgledny [%]: " << bestDistance-sa.getInitialCost() << std::endl;

    saveResults(file_name, bestPath, bestDistance, sa.getInitialCost());

}

void app::runTests() {
}

void app::saveResults(std::string filename,std::vector<int> path, int cost, int cost2) {
    std::ofstream fileToSave;
    fileToSave.open("results.xls", std::ios::app); // Open the file in append mode

    if (fileToSave.is_open() && fileToSave.good()) {
        fileToSave << filename ;

        fileToSave << "\nPath: ";
        for (size_t i = 0; i < path.size(); ++i) {
            fileToSave << path[i];
            if (i < path.size() - 1) {
                fileToSave << " -> ";
            }
        }
        fileToSave << "\nInitialCost: " << cost2 << "\n";
        fileToSave << "\nCost: " << cost << "\n";
        fileToSave << "----------------------\n";

        // Close the file
        fileToSave.close();
        std::cout << "Results saved successfully to results.xls" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file " << filename << " for writing" << std::endl;
    }
}
