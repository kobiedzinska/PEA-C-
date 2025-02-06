//
// Created by obied on 06.11.2024.
//

#include "App.h"

#include <fstream>
#include <iostream>
#include <string>
#include "../../AlgorytmyBB/BB.h"
#include "../../Utilities/Macierz/Matrix.h"
#include "../../Utilities/Czasomierz/Timer.h"


void App::runConfig() {
    std::cout<<"Running config app"<<std::endl;

    std::ofstream test;
    test.open("test.txt",std::ios::app);
    test << "test.txt";
    test.close();


    std::pair<Matrix, std::string> box = std::make_pair(Matrix(), "");
    std::string filename = "config.txt";
    std::ifstream file;
    file.open(filename);
    if(!file.is_open()){
        std::cout << "File not found" << std::endl;
    }
    std::string mode;
    file >> mode;

    if(mode == "R" || mode == "G") {

       if(mode == "R") {                                                        ////READING FROM FILE
        // nazwa pliku: (bez rozszerzenia)
        std::string readingFile;
        file >> readingFile;
        std::cout << "Reading from file:  " << readingFile << ".txt" << std::endl;
        std::string alg;
        file >> alg;
        if(alg == "BFS") {
            std::cout << "BFS" << std::endl;
            Matrix m;
            m.readFromFile(readingFile);

           auto[cost, path] = BB::BFS(m);
            std::cout << "Cost: " << cost << std::endl;
            for(int i : path) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
            saveResults(cost,path,filename, alg);

        }
        else if(alg == "DFS") {
            std::cout << "DFS" << std::endl;
            Matrix m;
            m.readFromFile(readingFile);

            auto[cost, path] = BB::DFS(m);
            std::cout << "Cost: " << cost << std::endl;
            for(int i : path) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
            saveResults(cost,path,filename, alg);

        }
        else if(alg == "BEST") {
            std::cout << "BEST" << std::endl;
            Matrix m;
            m.readFromFile(readingFile);

            auto[cost, path] = BB::bestFS(m);
            std::cout << "Cost: " << cost << std::endl;
            for(int i : path) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
            saveResults(cost,path,filename, alg);

        }
        else if(alg == "ALL") {
            std::cout << "ALL" << std::endl;
            Matrix m;
            m.readFromFile(readingFile);

            auto[cost, path] = BB::BFS(m);
            std::cout << "Cost BFS: " << cost << std::endl;
            for(int i : path) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            auto[cost1, path1] = BB::DFS(m);
            std::cout << "Cost DFS: " << cost1 << std::endl;
            for(int i : path1) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            auto[cost2, path2] = BB::bestFS(m);
            std::cout << "Cost bestFS: " << cost2 << std::endl;
            for(int i : path2) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

        }
        else {
            std::cout << "Reading from file >> Invalid algorithm." << std::endl;
        }
                                                                    ////END READING
        } else if(mode =="G") {                                                        ////GENERATING
            int size;
            file >> size;
            bool isSymmetric;
            file >> isSymmetric;
            std::string alg;
            file >> alg;
            if(alg == "BFS") {
                std::cout << "BFS" << std::endl;
                Matrix m;
                m.generate(size);
                if(isSymmetric) {
                    m.fillRandomSymmetric();
                }else {
                    m.fillRandom();
                }
                auto[cost, path] = BB::BFS(m);
                std::cout << "Cost: " << cost << std::endl;
                for(int i : path) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                saveResults2(cost, path,size,isSymmetric,alg);

            }
            else if(alg == "DFS") {
                std::cout << "DFS" << std::endl;
                Matrix m;
                m.generate(size);
                if(isSymmetric) {
                    m.fillRandomSymmetric();
                }else {
                    m.fillRandom();
                }
                auto[cost, path] = BB::DFS(m);
                std::cout << "Cost: " << cost << std::endl;
                for(int i : path) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                saveResults2(cost, path,size,isSymmetric,alg);

            }
            else if(alg == "BEST") {
                std::cout << "BEST" << std::endl;
                Matrix m;
                m.generate(size);
                if(isSymmetric) {
                    m.fillRandomSymmetric();
                }else {
                    m.fillRandom();
                }
                auto[cost, path] = BB::bestFS(m);
                std::cout << "Cost: " << cost << std::endl;
                for(int i : path) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                saveResults2(cost, path,size,isSymmetric,alg);

            }
            else if(alg == "ALL") {
                std::cout << "ALL" << std::endl;
                Matrix m;
                m.generate(size);
                if(isSymmetric) {
                    m.fillRandomSymmetric();
                }else {
                    m.fillRandom();
                }

                auto[cost, path] = BB::BFS(m);
                std::cout << "Cost BFS: " << cost << std::endl;
                for(int i : path) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                auto[cost1, path1] = BB::DFS(m);
                std::cout << "Cost DFS: " << cost1 << std::endl;
                for(int i : path1) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                auto[cost2, path2] = BB::bestFS(m);
                std::cout << "Cost bestFS: " << cost2 << std::endl;
                for(int i : path2) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;


            }
            else {
                std::cout << "Reading from file >> Invalid algorithm." << std::endl;
            }
        }else {
            std::cout << "Reading from file >> Invalid format." << std::endl;
            file.close();
        }                                                        ////END GENERATING

    }else {
        std::cout << "Invalid config file." << std::endl;
        file.close();
    }

    std::cout <<"Closing" << std::endl;
    file.close();
}

void App::saveResults(int cost, const std::vector<int>& path, const std::string &filename, const std::string &alg) {
    std::ofstream fileToSave;
    fileToSave.open("read_"+filename+"_"+alg+".xls",std::ios::app);

    if(fileToSave.is_open() && fileToSave.good()) {
        fileToSave << "Results from file: \t"<<filename <<"\t algorithm: "<<alg<<" \n";
        fileToSave << " Cost"<< cost;
        for(int i : path) {
            fileToSave << i << " \t";
        }
        fileToSave << "\n";
        fileToSave.close();

    }
}

void App::saveResults2(int cost, const std::vector<int>& path, const int size, bool isSymmetric, const std::string &alg) {
    std::ofstream fileToSave;
    fileToSave.open("generate_"+std::to_string(size)+"_"+std::to_string(isSymmetric)+"_"+alg+".xls",std::ios::app);

    if(fileToSave.is_open() && fileToSave.good()) {
        fileToSave << "Results from generated matrix: \t"<<"size: \t"<<size<<"\t isSymmetric: \t"<< isSymmetric<<"\t algorithm: "<<alg<<" \n";
        fileToSave << " Cost \t"<< cost << "\n";
        fileToSave << " Path: \t";
        for(int i : path) {
            fileToSave << i << " \t";
        }
        fileToSave << "\n";
        fileToSave.close();
        std::cout <<"file to save closed\n";

    }
}

void App::runTest() {
   //robienie testów

        //zapisywanie wyników do excela
        //              time
        //sizes:   |  BF   |   NN  |   Rnd  |
        //   5:    |  -    |    -  |    -   |
        //   6:    |  -    |    -  |    -   |
        //   7:    |  -    |    -  |    -   |
        //   ...     ...       ...     ...
    Timer stoper;
    std::ofstream fileToSave;
    if(fileToSave.good()) {
        std::cout << "File created" << std::endl;
        fileToSave.open("result_3-9_asymmetric.xls",std::ios::app);

        int sizes[] = {10,11}; //, 5, 6, 7, 8, 9, 10,11,12
        fileToSave  << "asymetryczne \n";
        fileToSave << "sizes: \t"<<"BFS \t"<<"DFS"<<"\tbestFS"<<"\n";
        for(int i : sizes) {
            std::cout <<"SIZE: " << i << " ";
            Matrix matrix;
            matrix.generate(i);
            long long int sredBFS=0;
            long long int sredDFS=0;
            long long int sredbestFS=0;
            fileToSave << i << "\t";

            for(int j = 0; j < 100; j++) {
                matrix.fillRandom();
                //matrix.fillRandomSymmetric();

                //BFS
                LARGE_INTEGER a = stoper.startTimer();
                BB::BFS(matrix);
                LARGE_INTEGER b =stoper.stopTimer();
                sredBFS += stoper.diffTime(a,b);

                //DFS
                a = stoper.startTimer();
                BB::DFS(matrix);
                b =stoper.stopTimer();
                sredDFS += stoper.diffTime(a,b);

                //bestFS
                a = stoper.startTimer();
                BB::bestFS(matrix);
                b =stoper.stopTimer();
                sredbestFS += stoper.diffTime(a,b);

            }
            fileToSave << sredBFS/100 <<"\t"<< sredDFS/100<<"\t" << sredbestFS/100 << "\n";
        }
    }else {
        std::cout << "File not created" << std::endl;
        fileToSave.close();
    }
    fileToSave.close();
    std::cout << "File saved and closed";

}


