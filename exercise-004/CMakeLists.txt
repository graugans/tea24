cmake_minimum_required (VERSION 3.10.2)

# Teil E: Übungsaufgabe 3 schrittweise statisch linken.
# Teil A bis D (main.c, greet.c) werden von Hand mit gcc übersetzt, nicht mit CMake.
project (exercise-004 VERSION 0.1.0 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include dependencies
find_package(fmt REQUIRED)

############################################################
# E.1: Ausgangslage - Versionsbibliothek statisch, {fmt} und libc dynamisch
############################################################

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
                                        tea24::version
                                        fmt::fmt
)
