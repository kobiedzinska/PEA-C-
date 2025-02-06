// genetic_tsp_binding.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
// Zmień ścieżkę do twojego Parser.h
#include <fstream>

#include "../cpp/Utilities/Parser/Parser.h"
#include "../cpp/Genetic/genetic_tsp.h"

namespace py = pybind11;

// Wrapper dla wyników
struct TSPResult {
    double final_distance;
    std::vector<std::pair<double, double>> evolution_data; // (czas, błąd względny)
};

// Wrapper dla algorytmu genetycznego
// genetic_tsp_binding.cpp
class TSPWrapper {
private:
    std::unique_ptr<genetic_tsp> ga;

public:
    TSPWrapper(const std::string& xml_path,
               int populationSize,
               double crossoverRate,
               double mutationRate,
               std::string crossoverMethod = "OX",
               std::string mutationMethod = "swap") {

        // Sprawdzenie ścieżki
        std::ifstream file(xml_path);
        if (!file.good()) {
            throw std::runtime_error("Cannot open XML file: " + xml_path);
        }
        file.close();

        // Tworzenie instancji genetic_tsp z właściwymi parametrami
        ga = std::make_unique<genetic_tsp>(xml_path,
                                         populationSize,
                                         crossoverRate,
                                         mutationRate,
                                         crossoverMethod,
                                         mutationMethod);
    }

    TSPResult run() {
        ga->run();

        TSPResult result;
        result.final_distance = ga->getBestDistance();
        result.evolution_data = ga->getEvolutionData();

        return result;
    }
};



// Binding dla struktury Graph
void bind_graph_structures(py::module& m) {
    py::class_<Edge>(m, "Edge")
        .def(py::init<>())
        .def_readwrite("target", &Edge::target)
        .def_readwrite("cost", &Edge::cost);

    py::class_<Graph>(m, "Graph")
        .def(py::init<>())
        .def_readwrite("name", &Graph::name)
        .def_readwrite("source", &Graph::source)
        .def_readwrite("description", &Graph::description)
        .def_readwrite("adjacency_list", &Graph::adjacency_list);
}

PYBIND11_MODULE(genetic_tsp_cpp, m) {
    // Binding dla struktur Graph i Edge
    bind_graph_structures(m);

    // Binding dla struktury wyników
    py::class_<TSPResult>(m, "TSPResult")
        .def_readwrite("final_distance", &TSPResult::final_distance)
        .def_readwrite("evolution_data", &TSPResult::evolution_data);

    // Binding dla głównej klasy algorytmu
    py::class_<TSPWrapper>(m, "GeneticTSP")
        .def(py::init<std::string, int, double, double, double,
             std::string, std::string>(),
             py::arg("xml_path"),
             py::arg("population_size"),
             py::arg("crossover_rate"),
             py::arg("mutation_rate"),
             py::arg("crossover_method") = "OX",
             py::arg("mutation_method") = "swap")
        .def("run", &TSPWrapper::run);
}