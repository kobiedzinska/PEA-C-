

#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <string>

class Matrix {
private:
    int **array;
    int size;
    bool symmetric;

public:

    Matrix(): array(nullptr), size(0), symmetric(false) {};            //default constructor
    //explicit Matrix(const std::vector<std::vector<int>> & move);;            //default constructor
    ~Matrix(){
        for(int i = 0; i < size; i++){              //destructor
            delete[] array[i];
        }
        delete[] array;
    }

    void readFromFile(const std::string& filename); //reads matrix from file
    void generate(int size);                //generates random matrix
    void fillRandom();                      //generates random matrix
    void fillRandomSymmetric();            //generates random symmetric matrix
    void printMatrix();                             //prints matrix
    int getWeight(int i, int j) const;                   //returns element from matrix
    int getSize() const;                                  //returns size of matrix
    Matrix(const Matrix &other);
    Matrix &operator=(const Matrix &other);
    bool operator==(const Matrix &other) const;

    //std::pair<int,Matrix> reduceMatrix(Matrix& matrix);       //return the reduced cost, it changes the original matrix
    void changeValue(int x, int y, int value);
};



#endif //MATRIX_H
