
#include <iostream>
#include <fstream>
#include "Matrix.h"

void Matrix::readFromFile(const std::string &filename) {
    const std::string filepath = "C:/Users/obied/CLionProjects/PEA_zad1/Aplikacja/FilesToRead/";
    const std::string extension = ".txt";

    std::ifstream file;
    file.open(filepath + filename + extension);
    if(!file.is_open()){
        std::cout << "File not found" << std::endl;
        return;
    }

    file >> size;
    array = new int*[size];
    for(int i = 0; i < size; i++){
        array[i] = new int[size];
        for(int j = 0; j < size; j++){
            file >> array[i][j];
        }
    }
    file.close();
}

int Matrix::getWeight(int i, int j) const {
    if(i < 0 || i >= size || j < 0 || j >= size){
        std::cout << "Index out of bounds" << std::endl;
        return -2;
    }

    return array[i][j];
}

void Matrix::printMatrix() {
    std::cout << "Matrix: " << std::endl;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            std::cout << array[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int Matrix::getSize() const {
    return size;
}

bool Matrix::isSymmetric() const {
    return symmetric;
}

void Matrix::generate(int size) {
    this->size = size;
    array = new int*[size];
    for(int i = 0; i < size; i++){
        array[i] = new int[size];
        for(int j = 0; j < size; j++){
            array[i][j] = -1;
        }
    }
}

void Matrix::fillRandom() {
    symmetric = false;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i != j){
                array[i][j] = rand() % 100;
            }
        }
    }
}


void Matrix::fillRandomSymmetric() {
    symmetric = true;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i == j)
                array[i][j] = -1;
            else if(i > j)
                array[i][j] = array[j][i];
            else
                array[i][j] = rand() % 100;
        }
    }

}

