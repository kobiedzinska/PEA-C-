

#include "BB.h"
#include "../Utilities/Macierz/Matrix.h"
#include "../Utilities/Queue/Queue.h"
#include "../Utilities/Utilities.h"
#include "../Utilities/Stack/Stack.h"
#include <vector>
#include <queue>
#include <stack>
using namespace std;


std::pair<int, std::vector<int>> BB::BFS(Matrix &matrix) {
    int MINIMUM = INT_MAX;
    const int vertexes = matrix.getSize();
    auto bestPath = vector<int>(vertexes);

    // Implementacja wierzchołka startowego
    int startPoint = 0;
    auto [M, C] = reducedMatrix(matrix);
    myNode start = {0, C, {0}, M};
    //start.reducedMatrix.printMatrix();

    // Implementacja kolejki
    //Queue<myNode>queue(vertexes);
    queue<myNode> queue;
    queue.push(start);

    // Start algorytmu
    while (!queue.empty()) {
        myNode current = queue.front();
        queue.pop();
        //std::cout <<"\n\nCurrent node " << current.city <<"\n";

        for(int i = 0 ; i < vertexes ; i++) {
           if(current.reducedMatrix.getWeight(i,0) != -1) {         // jeśli wierzchołek nie był odwiedzony, to wchodzimy
               //std::cout << current.city << "-> " << i <<std::endl;
               myNode node = createNode(current,current.reducedMatrix, i);                           // robimy obliczenia dla macierzy
               queue.push(node);

               if(node.path.size() == vertexes) {                           // jeśli ścieżka jest pełna, to sprawdzamy < MINIMUM
                   node.path.push_back(startPoint);
                   if(node.cost + current.reducedMatrix.getWeight(node.city,startPoint) < MINIMUM) {
                       MINIMUM = node.cost + current.reducedMatrix.getWeight(node.city,startPoint);
                       bestPath = node.path;
                   }
               }
           }
        }
    }

return {MINIMUM,bestPath};
}

std::pair<int, std::vector<int>> BB::DFS(Matrix &matrix) {
    int MINIMUM = INT_MAX;
    const int vertexes = matrix.getSize();
    auto bestPath = vector<int>(vertexes);

    // Implementacja wierzchołka startowego
    int startPoint = 0;
    auto [M, C] = reducedMatrix(matrix);
    myNode start = {0, C, {0}, M};
    //start.reducedMatrix.printMatrix();

    // Implementacja stosu
    stack<myNode> queue;
    queue.push(start);

    // Start algorytmu
    while (!queue.empty()) {
        myNode current = queue.top();
        queue.pop();

        for(int i = 0 ; i < vertexes ; i++) {
            if(current.reducedMatrix.getWeight(i,0) != -1) {         // jeśli wierzchołek nie był odwiedzony, to wchodzimy
                myNode node = createNode(current,current.reducedMatrix, i);                           // robimy obliczenia dla macierzy
                if(node.cost < MINIMUM) {
                    queue.push(node);
                }

                if(node.path.size() == vertexes) {                           // jeśli ścieżka jest pełna, to sprawdzamy < MINIMUM
                    node.path.push_back(startPoint);
                    if(node.cost + current.reducedMatrix.getWeight(node.city,startPoint) < MINIMUM) {
                        MINIMUM = node.cost + current.reducedMatrix.getWeight(node.city,startPoint);
                        bestPath = node.path;
                    }
                }
            }
        }
    }

    return {MINIMUM,bestPath};
}


std::pair<int, std::vector<int>> BB::bestFS(Matrix &matrix) {
    int MINIMUM = INT_MAX;
    const int vertexes = matrix.getSize();
    auto bestPath = vector<int>(vertexes);

    // Implementacja wierzchołka startowego
    int startPoint = 0;
    auto [M, C] = reducedMatrix(matrix);
    myNode start = {0, C, {0}, M};

    // Implementacja kolejki priorytetowej
    std::priority_queue<myNode, std::vector<myNode>, CompareCost> queue;
    queue.emplace(start);

    // Start algorytmu
    while (!queue.empty()) {
        myNode current = queue.top();
        queue.pop();

        for(int i = 0 ; i < vertexes ; i++) {
            if(current.reducedMatrix.getWeight(i,0) != -1) {         // jeśli wierzchołek nie był odwiedzony, to wchodzimy
                myNode node = createNode(current,current.reducedMatrix, i);                           // robimy obliczenia dla macierzy

                if(node.path.size() == vertexes) {                           // jeśli ścieżka jest pełna, to sprawdzamy < MINIMUM
                    node.path.push_back(startPoint);
                    if(node.cost + current.reducedMatrix.getWeight(node.city,startPoint) < MINIMUM) {
                        MINIMUM = node.cost + current.reducedMatrix.getWeight(node.city,startPoint);
                        bestPath = node.path;
                    }
                    continue;
                }

                if(node.cost < MINIMUM) {
                  queue.push(node);
                }
            }
        }
    }

    return {MINIMUM,bestPath};
}


BB::myNode BB::createNode(const myNode &parent, const Matrix& matrix, int nextCity) {
                                            // Kopiujemy macierz kosztów i ścieżkę
    Matrix copy = matrix;
    std::vector<int> newPath = parent.path;
    newPath.push_back(nextCity);            // dodajemy miasto do ścieżki

    const int size = copy.getSize();

    // Ustaw wiersz parent.city na -1
    for (int j = 0; j < size; ++j) {
        copy.changeValue(parent.city, j, -1);
    }

    // Ustaw kolumnę nextCity na -1
    for (int i = 0; i < size; ++i) {
        copy.changeValue(i, nextCity, -1);
    }

    copy.changeValue(nextCity, 0, -1); // żeby nie można było wrócić do wierzchołka początkowego

    auto [matrix0, cost0] = reducedMatrix(copy); // Redukujemy macierz  > koszt z redukcji tego wierzchołka

                                                // Sumujemy koszty jako nowy koszt całej ścieżki
    const int newCost = parent.cost + parent.reducedMatrix.getWeight(parent.city,nextCity) + cost0;

    return myNode{nextCity, newCost, newPath, matrix0};

}



//we create the first node (root) v0 = 1
    //we mark as inactive
    //we reduce and receive cost c0

    //przechodizmy na niższy level drzewa (połączenia zaczynają się od wierzchołka zerowego i wychodzą do drugiego wierzchołka

    //pętla
        // bierzemy aktywny wierzchołek np. v = 4
        // we set in matrix infinities for v0 row and v (= 4) column
            // row wierzchołka z którego wychodzimy
            // col wierzchołka do którego wchodzimy
                // i musimy ustawić, zeby nie wrócić do v0 przez v, więc ustawiamy go jako infinity
    // (przy kolejnych iteracjach) jeśli jakiś wierzchołek został już odwiedzony, to trzeba ustawić infinity //głównie to chodzi o wierzchołek startowy
