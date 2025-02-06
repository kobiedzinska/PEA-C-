cmake_minimum_required(VERSION 3.20)
project(PEA_zad4)

set(CMAKE_CXX_STANDARD 20)

# Add pugixml
add_library(pugixml STATIC
        cpp/Utilities/pugixml/pugixml.cpp
)
target_include_directories(pugixml PUBLIC cpp/Utilities/pugixml)

# Find Python and pybind11
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Create the Python module
pybind11_add_module(genetic_tsp_cpp binding/genetic_tsp_binding.cpp
        cpp/Genetic/genetic_tsp.cpp
        cpp/Utilities/Parser/Parser.cpp
        cpp/Utilities/Matrix/Matrix.cpp
)

# Link pugixml to the Python module
target_link_libraries(genetic_tsp_cpp PRIVATE pugixml)

# Create the main executable
add_executable(PEA_zad4 main.cpp
        cpp/Genetic/genetic_tsp.cpp
        cpp/Genetic/genetic_tsp.h
        cpp/Utilities/Parser/Parser.cpp
        cpp/Utilities/Parser/Parser.h
        cpp/Utilities/Matrix/Matrix.cpp
        cpp/Utilities/Matrix/Matrix.h
        cpp/Utilities/Matrix/Structure.h
        cpp/App/App.cpp
        cpp/App/App.h
)

# Link pugixml to the main executable
target_link_libraries(PEA_zad4 PRIVATE pugixml)