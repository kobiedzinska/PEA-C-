#include <iostream>
#include <string>

#include "application/app.h"
#include "utilities/parser/Parser.h"
#include "utilities/SA/SimulatedAnnealing.h"
#include "utilities/structure/Matrix.h"


int main() {
    //app a;
    //a.runConfig();
    ///return 0;
    try {
        std::string file_path = "rbg403.xml"; // Ścieżka do pliku XML
        auto graph = Parser::loadGraphFromXML(file_path);
        Matrix matrix;
        matrix.fromGraph(graph);
        //matrix.printMatrix();

        SimulatedAnnealing sa(matrix, 1000000, 120.0,  0.99,2);

        for(int i = 0; i < 4; i++) {
            std::cout<<"File: " <<file_path<< " iter: "<< i <<std::endl;
          auto [bestPath, bestDistance] = sa.solve(i);
        std::cout << "Najlepsza znaleziona trasa:\n";
        for (int city : bestPath) {
            std::cout << city << " -> ";
        }
        std::cout << bestPath[0] << std::endl;
        std::cout << "Calkowita dlugosc trasy: " << bestDistance << std::endl;
        std::cout << "Blad wzgledny [%]: " << std::fabs(bestDistance - sa.getInitialCost()) / sa.getInitialCost() * 100 << std::endl;

        }


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
