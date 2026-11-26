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

############################################################
# E.2: {fmt} header-only - der {fmt}-Code wird in das Programm übersetzt
############################################################

# Nicht tea24::version: Die Bibliothek bringt sonst über LINK_ONLY wieder libfmt.so mit.
# fmt::fmt und fmt::fmt-header-only dürfen nicht im selben Programm gemischt werden.
add_executable(${PROJECT_NAME}-static main.cpp)
target_link_libraries(${PROJECT_NAME}-static PRIVATE
                                        tea24::version_fmt_header_only
                                        fmt::fmt-header-only
)

############################################################
# E.3: alles statisch - auch libstdc++, libm und libc
############################################################

# -static braucht die statischen Varianten libc.a, libm.a und libstdc++.a
# (Debian/Ubuntu: libc6-dev, Fedora: glibc-static und libstdc++-static).
add_executable(${PROJECT_NAME}-full-static main.cpp)
target_link_libraries(${PROJECT_NAME}-full-static PRIVATE
                                        tea24::version_fmt_header_only
                                        fmt::fmt-header-only
)
target_link_options(${PROJECT_NAME}-full-static PRIVATE -static)

# Diagrams for SOLUTION.md (optional, needs Graphviz):
#   cmake --build build --target exercise-004-diagrams
add_graphviz_diagrams(${PROJECT_NAME}-diagrams "${CMAKE_CURRENT_SOURCE_DIR}/doc")
