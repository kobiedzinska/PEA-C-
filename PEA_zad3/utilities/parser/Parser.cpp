#include <stdexcept>
#include "Parser.h"
#include <vector>
#include "../pugixml/pugixml.hpp"

Graph Parser::loadGraphFromXML(const std::string &file_path) {
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
            Edge edge{};
            edge.target = edge_node.text().as_int();
            edge.cost = edge_node.attribute("cost").as_double();
            edges.push_back(edge);
        }

        graph.adjacency_list.push_back(edges);
    }

    return graph;
}

