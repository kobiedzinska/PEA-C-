cmake_minimum_required(VERSION 3.29)
project(PEA_zad2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEA_zad2 main.cpp
        Utilities/Macierz/Matrix.cpp
        Utilities/Macierz/Matrix.h
        Utilities/Czasomierz/Timer.cpp
        Utilities/Czasomierz/Timer.h
        AlgorytmyBB/BB.cpp
        AlgorytmyBB/BB.h
        Utilities/Stack/Stack.h
        Utilities/Queue/Queue.h
        Menu/Application/App.cpp
        Menu/Application/App.h
        Utilities/Queue/PriorityQueue.h
        Utilities/Utilities.cpp
        Utilities/Utilities.h
)
