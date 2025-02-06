#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "pugixml.hpp"

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

// Funkcja do wczytania grafu z pliku XML
Graph loadGraphFromXML(const std::string& file_path) {
    pugi::xml_document doc;
    if (!doc.load_file(file_path.c_str())) {
        throw std::runtime_error("Unable to load XML file.");
    }

    Graph graph;

    // Parsowanie głównych informacji o grafie
    auto root = doc.child("travellingSalesmanProblemInstance");
    if (!root) {
        throw std::runtime_error("Invalid XML format: Missing root element.");
    }

    graph.name = root.child("name").text().as_string();
    graph.source = root.child("source").text().as_string();
    graph.description = root.child("description").text().as_string();

    // Parsowanie wierzchołków i krawędzi
    auto graph_node = root.child("graph");
    if (!graph_node) {
        throw std::runtime_error("Invalid XML format: Missing <graph> element.");
    }

    for (auto vertex_node : graph_node.children("vertex")) {
        std::vector<Edge> edges;

        for (auto edge_node : vertex_node.children("edge")) {
            Edge edge;
            edge.target = edge_node.text().as_int();
            edge.cost = edge_node.attribute("cost").as_double();
            edges.push_back(edge);
        }

        graph.adjacency_list.push_back(edges);
    }

    return graph;
}

// Przykład użycia
int main() {
    try {
        std::string file_path = "ulysses16.xml"; // Ścieżka do pliku XML
        Graph graph = loadGraphFromXML(file_path);

        // Wyświetlanie załadowanych danych
        std::cout << "Graph Name: " << graph.name << std::endl;
        std::cout << "Source: " << graph.source << std::endl;
        std::cout << "Description: " << graph.description << std::endl;

        for (size_t i = 0; i < graph.adjacency_list.size(); ++i) {
            std::cout << "Vertex " << i << " edges:" << std::endl;
            for (const auto& edge : graph.adjacency_list[i]) {
                std::cout << "  -> Vertex " << edge.target << " (Cost: " << edge.cost << ")" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
