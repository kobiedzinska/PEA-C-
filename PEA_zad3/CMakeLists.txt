cmake_minimum_required(VERSION 3.29)
project(PEA_zad3)

set(CMAKE_CXX_STANDARD 20)

add_executable(PEA_zad3 main.cpp
        utilities/parser/Parser.cpp
        utilities/parser/Parser.h
        utilities/structure/Matrix.cpp
        utilities/structure/Matrix.h
        utilities/structure/Structure.h
        application/app.cpp
        application/app.h
        utilities/SA/SimulatedAnnealing.cpp
        utilities/SA/SimulatedAnnealing.h)

add_library(pugixml STATIC utilities/pugixml/pugixml.cpp)
target_link_libraries(PEA_zad3 PRIVATE pugixml)
