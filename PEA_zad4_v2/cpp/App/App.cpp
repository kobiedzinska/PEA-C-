
#include "app.h"

#include <fstream>
#include <iostream>
#include "../utilities/Matrix/Matrix.h"
#include "../Utilities/Parser/Parser.h"
#include "../Genetic/genetic_tsp.h"

void App::runConfig() {
    std::string file_name;
    int population_size;
    double crossover_rate;
    double mutation_rate;
    double timeLimit;
    std::string crossover_method;
    std::string mutation_method;

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
        file >> population_size;
        file >> crossover_rate;
        file >> crossover_method;
        file >> mutation_rate;
        file >> mutation_method;
        file >> timeLimit;
    }
    file.close();

    std::cout << "Configurations loaded." << std::endl;
    std::cout<<"File name: " << file_name<<std::endl;
    std::cout<<"Population size: " << population_size<<std::endl;
    std::cout<<"Crossover rate: " << crossover_rate<<std::endl;
    std::cout<<"Crossover method: " << crossover_method<<std::endl;
    std::cout<<"Mutation rate: "<<mutation_rate<<std::endl;
    std::cout<<"Mutation method: "<<mutation_method<<std::endl;
    std::cout<<"Time limit: " << timeLimit<<" [s]"<<std::endl;
    std::cout << std::endl;

    // Odczytanie macierzy z pliku
    /*
    Matrix matrix;
    try {
        auto graph = Parser::loadGraphFromXML(file_name);
        matrix.fromGraph(graph);
    } catch(std::exception& e) {
        std::cout<<e.what()<<std::endl;
    }*/
    //matrix.printMatrix();

    // Wyłowanie algorytmu
    auto gs = new genetic_tsp(file_name, population_size, crossover_rate, mutation_rate, crossover_method, mutation_method);
    gs->setTimeLimit(timeLimit);
    std::cout << gs->run();

    //Zapisanie wyników
    //saveResults(file_name, bestPath, bestDistance, sa.getInitialCost());

}

void App::runTests() {
}
/*
void App::saveResults(std::string filename,std::vector<int> path, int cost, int cost2) {
    std::ofstream fileToSave;
    fileToSave.open("results.xls", std::ios::app); // Open the file in append mode

    if (fileToSave.is_open() && fileToSave.good()) {
        fileToSave << filename ;


        fileToSave << "\nInitialCost: " << cost2 << "\t";
        fileToSave << "\nCost: " << cost << "\t";
        fileToSave << "\nPath: ";
        for (size_t i = 0; i < path.size(); ++i) {
            fileToSave << path[i];
            if (i < path.size() - 1) {
                fileToSave << " -> ";
            }
        }

        // Close the file
        fileToSave.close();
        std::cout << "Results saved successfully to results.xls" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file " << filename << " for writing" << std::endl;
    }
}
*/