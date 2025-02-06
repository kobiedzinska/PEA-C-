

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <vector>

// Struktura reprezentująca krawędź
struct Edge {
    int target; // Numer wierzchołka docelowego
    double cost; // Koszt przejścia
};

// Struktura reprezentująca graf
struct Graph {
    std::string name;
    std::string source;
    std::string description;
    std::vector<std::vector<Edge>> adjacency_list; // Lista sąsiedztwa
};

#endif //STRUCTURE_H
