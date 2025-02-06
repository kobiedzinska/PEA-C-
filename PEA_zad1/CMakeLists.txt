cmake_minimum_required(VERSION 3.27)
project(PEA_zad1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEA_zad1 main.cpp
        Utilities/Macierz/Matrix.cpp
        Utilities/Macierz/Matrix.h
        Algorytmy/BruteForceA.cpp
        Algorytmy/BruteForceA.h
        Algorytmy/NNA.cpp
        Algorytmy/NNA.h
        Algorytmy/RandomA.cpp
        Algorytmy/RandomA.h
        Aplikacja/Application.cpp
        Aplikacja/Application.h
        Utilities/Timer/Stoper.cpp
        Utilities/Timer/Stoper.h
        Utilities/Timer/Stoper.h
        Utilities/Tests/Testing.cpp
        Utilities/Tests/Testing.h
        Utilities/Permutation/Permutation.cpp
        Utilities/Permutation/Permutation.h
)
