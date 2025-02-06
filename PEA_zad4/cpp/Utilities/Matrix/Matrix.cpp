

#include "Matrix.h"
#include <fstream>
#include <iostream>
#include "../pugixml/pugixml.hpp"

void Matrix::readFromFile(const std::string &filename) {

    const std::string extension = ".txt";

    std::ifstream file;
    file.open(filename + extension);

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

void Matrix::fromGraph(const Graph &graph) {
    // Set size from graph
    this->size = graph.adjacency_list.size();

    // Allocate memory for the matrix
    if (this->size > 0) {
        this->array = new int*[this->size];
        for (int i = 0; i < this->size; i++) {
            this->array[i] = new int[this->size]();
        }
    } else {
        std::cerr << "Error Matrix.cpp: Graph is empty, cannot create a matrix." << std::endl;
        return;
    }

    // Fill the matrix with costs from the graph
    for (int i = 0; i < graph.adjacency_list.size(); ++i) {
        for (const auto& edge : graph.adjacency_list[i]) {
            // Convert double cost to int (since matrix stores ints)
            this->array[i][edge.target] = static_cast<int>(edge.cost);
        }
    }
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

void Matrix::setSize(int size) {
    this->size = size;
}


Matrix::Matrix(const Matrix& other) {
    size = other.size;
    array = new int*[size];
    for(int i = 0; i < size; i++) {
        array[i] = new int[size];
        for(int j = 0; j < size; j++) {
            array[i][j] = other.array[i][j];
        }
    }
}
Matrix& Matrix::operator=(const Matrix& other) {
    if(this != &other) {
        // Zwolnij starą pamięć
        for(int i = 0; i < size; i++) {
            delete[] array[i];
        }
        delete[] array;

        // Kopiuj nowe dane
        size = other.size;
        array = new int*[size];
        for(int i = 0; i < size; i++) {
            array[i] = new int[size];
            for(int j = 0; j < size; j++) {
                array[i][j] = other.array[i][j];
            }
        }
    }
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (size != other.size) return false; // Sprawdzenie rozmiaru
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (array[i][j] != other.array[i][j]) return false; // Porównanie elementów
    return true; // Macierze są równe
}


void Matrix::changeValue(int x, int y, int value) {
    array[x][y] = value;
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



