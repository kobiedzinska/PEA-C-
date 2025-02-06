cmake_minimum_required(VERSION 3.29)
project(PEA_zad4_v2)

set(CMAKE_CXX_STANDARD 20)

add_executable(PEA_zad4_v2 main.cpp
        cpp/Genetic/genetic_tsp.h
        cpp/Utilities/Matrix/Matrix.cpp
        cpp/Utilities/Parser/Parser.cpp
        cpp/Utilities/pugixml/pugixml.cpp
        cpp/App/App.cpp
        cpp/Tests/Test.cpp
        cpp/Tests/Test.h
        cpp/Genetic/genetic_tsp.cpp
        cpp/Utilities/Parser/Parser.h)

add_library(pugixml STATIC
        cpp/Utilities/pugixml/pugixml.cpp
)
# Link pugixml to the main executable
target_link_libraries(PEA_zad4_v2 PRIVATE pugixml)