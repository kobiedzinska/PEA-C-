
#include "Application.h"
#include <fstream>
#include <iostream>

#include "C:\Users\obied\CLionProjects\PEA_zad1\Utilities\Macierz\Matrix.h"

#include "../Algorytmy/BruteForceA.h"
#include "../Algorytmy/NNA.h"
#include "../Algorytmy/RandomA.h"


void Application::run() {
    //read config file
    std::string filename = "C:/Users/obied/CLionProjects/PEA_zad1/Aplikacja/config.txt";
    std::ifstream file;
    file.open(filename);
    if(!file.is_open()){
        std::cout << "File not found" << std::endl;
        return;
    }

    std::string mode;
    file >> mode;
    if(mode == "R") {
        std::cout <<"I am reading from file..." << std::endl;
        std::string filename1;
        file >> filename1;
        std::cout <<"filename: " << filename1 + ".txt" << std::endl;
        std::string alg;
        file >> alg;
        int a=-1;
        if(alg == "BF") {
            a=1;
        }
        if(alg == "NN") {
            a=2;
        }
        if(alg == "Rnd") {
            a=3;
        }
        if(alg == "All") {
            a=4;
        }
        switch(a) {
            case 1:
                bf(filename1);
            break;
            case 2:
                nn(filename1);
            break;
            case 3:
                rnd(filename1);
            break;
            case 4:
                all(filename1);
                break;
            default:
                std::cout << "Invalid Parameters. Change config file :(."<<std::endl;
        }

    }else if(mode == "G") {
        std::cout <<"I am generating..." << std::endl;
        int size=-1;
        file >> size;
        bool isSymmetric;
        file >> isSymmetric;
        std::string alg;
        file >> alg;
        int a=-1;
        if(alg == "BF") {
           a=1;
        }
        if(alg == "NN") {
            a=2;
        }
        if(alg == "Rnd") {
            a=3;
        }
        if(alg == "All") {
            a=4;
        }
        switch(a) {
            case 1:
                bf(size, isSymmetric);
                break;
            case 2:
                nn(size, isSymmetric);
                break;
            case 3:
                rnd(size, isSymmetric);
                break;
            case 4:
                all(size, isSymmetric);
                break;
            default:
                std::cout << "Invalid Parameters. Change config file :(."<<std::endl;

        }
    }else {
        std::cout << "Wrong parameter: mode" << std::endl;
    }
    std::cout <<"Closing file..."<<std::endl;
    file.close();
}

//function when reading from file
int Application::bf(const std::string &fileName) {
    std::cout << "Brute Force Algorithm:" << std::endl;
    Matrix matrix;
    matrix.readFromFile(fileName);
    const int cost = BruteForceA::bruteForce(matrix);
    std::cout << "Minimal cost: " << cost << std::endl;
    return cost;
}

void Application::nn(const std::string &fileName) {
    std::cout << "Nearest Neighbour Algorithm:" << std::endl;
    Matrix matrix;
    matrix.readFromFile(fileName);
    std::cout << "Symmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlg(matrix) << std::endl;
    std::cout << "Asymmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlgAS(matrix) << std::endl;
}

void Application::rnd(const std::string& fileName) {
    std::cout << "Random Algorithm:" << std::endl;
    Matrix matrix;
    matrix.readFromFile(fileName);
    int const cost = RandomA::randomPO(matrix,BruteForceA::bruteForce(matrix));
    std::cout << "Minimal cost: " << cost << std::endl;
}

void Application::all(const std::string &fileName) {
    Matrix matrix;
    matrix.readFromFile(fileName);
    std::cout << std::endl;
    matrix.printMatrix();
    std::cout << std::endl;

    std::cout << "Brute Force Algorithm:" << std::endl;
    int cost = BruteForceA::bruteForce(matrix);
    std::cout << "Minimal cost: " << cost << std::endl;

    std::cout << "Nearest Neighbour Algorithm:" << std::endl;
    std::cout << "Symmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlg(matrix) << std::endl;
    std::cout << "Asymmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlgAS(matrix) << std::endl;

    std::cout << "Random Algorithm:" << std::endl;
    cost = RandomA::randomPO(matrix,BruteForceA::bruteForce(matrix));
    std::cout << "Minimal cost: " << cost << std::endl;

    std::cout << std::endl;
    std::cout << "That's all" << std::endl;
}

//function when generating
int Application::bf(const int size, const bool symmetric) {
    std::cout << "Brute Force Algorithm:" << std::endl;
    Matrix matrix;
    matrix.generate(size);
    if(symmetric) {
        matrix.fillRandomSymmetric();
    }
    else {
        matrix.fillRandom();
    }
    matrix.printMatrix();
    int const cost = BruteForceA::bruteForce(matrix);
    std::cout << "Minimal cost: " << cost << std::endl;
    return cost;
}

void Application::nn(const int size, const bool symmetric) {
    std::cout << "Nearest Neighbour Algorithm:" << std::endl;
    Matrix matrix;
    matrix.generate(size);
    int  cost = 0 ;
    if(symmetric) {
        matrix.fillRandomSymmetric();
        matrix.printMatrix();
    }
    else {
        matrix.fillRandom();
        matrix.printMatrix();
    }
    std::cout << "Symmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlg(matrix) << std::endl;
    std::cout << "Asymmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlgAS(matrix) << std::endl;
}

void Application::rnd(const int size, const bool symmetric) {
    std::cout << "Random Algorithm:" << std::endl;
    Matrix matrix;
    matrix.generate(size);
    if(symmetric) {
        matrix.fillRandomSymmetric();
    }
    else {
        matrix.fillRandom();
    }
    matrix.printMatrix();
    int const costBF = BruteForceA::bruteForce(matrix);
    const int cost = RandomA::randomPO(matrix,costBF);
    std::cout << "Minimal cost: " << cost << std::endl;
}

void Application::all(const int size, const bool symmetric) {
    std::cout << std::endl;
    Matrix matrix;
    matrix.generate(size);
    if(symmetric) {
        matrix.fillRandomSymmetric();
    }
    else {
        matrix.fillRandom();
    }
    matrix.printMatrix();
    std::cout << std::endl;

    std::cout << "Brute Force Algorithm:" << std::endl;
    const int costBF = BruteForceA::bruteForce(matrix);
    std::cout << "Minimal cost: " << costBF << std::endl;

    std::cout << "Nearest Neighbour Algorithm:" << std::endl;
    std::cout << "Symmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlg(matrix) << std::endl;
    std::cout << "Asymmetric"<<std::endl;
    std::cout << "Minimal cost: " << NNA::NNAlgAS(matrix) << std::endl;

    std::cout << "Random Algorithm:" << std::endl;
    std:: cout << "Minimal cost: "<< RandomA::randomPO(matrix,costBF);

    std::cout << std::endl;
    std::cout << "That's all" << std::endl;


}
