
#ifndef BB_H
#define BB_H

#include "../Utilities/Macierz/Matrix.h"


class BB {
    struct myNode {
        int city;                 // Current city
        int cost;                 // Total cost of the node
        std::vector<int> path;    // Visited nodes
        Matrix reducedMatrix;

    };
    struct CompareCost {
        bool operator()(const myNode& a, const myNode& b) const {
            return a.cost > b.cost; // Smaller cost, higher priority
        }
    };


  public:
    static std::pair<int, std::vector<int>> BFS(Matrix &matrix);
    static std::pair<int, std::vector<int>> DFS(Matrix &matrix);
    static std::pair<int, std::vector<int>> bestFS(Matrix &matrix);
    static myNode createNode(const myNode &parent, const Matrix& matrix, int nextCity);

};



#endif //BB_H
