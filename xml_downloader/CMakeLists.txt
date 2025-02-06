cmake_minimum_required(VERSION 3.29)
project(xml_downloader)

set(CMAKE_CXX_STANDARD 20)

add_executable(xml_downloader main.cpp pugixml.cpp)


