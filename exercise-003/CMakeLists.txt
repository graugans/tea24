cmake_minimum_required (VERSION 3.10.2)

# Set the project name to your project name, template isn't very descriptive
# C is needed for Bonus 3 (c_consumer.c)
project (exercise-003 VERSION 0.0.1 LANGUAGES C CXX)

# ACHTUNG: evaluated at CONFIGURE time, not on every build
string(TIMESTAMP BUILD_DATE "%Y-%m-%dT%H:%M:%S")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include dependencies
find_package(fmt REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)

# Generated config header of the PROGRAM (not of the library)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/config.h" @ONLY)

############################################################
# Teil 4: the exercise, linked against the STATIC library
############################################################

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE TEA_LINKAGE="statisch")

# No add_subdirectory(../libs/version) needed: the root CMakeLists.txt already knows the target.
# Linking against tea24::version also brings its PUBLIC include directory -> #include "version/version.h"
target_link_libraries(${PROJECT_NAME} PRIVATE
                                        tea24::version
                                        fmt::fmt
)

############################################################
# Bonus 2: a second consumer of the same library
############################################################

add_executable(${PROJECT_NAME}-cli version_cli.cpp)
target_link_libraries(${PROJECT_NAME}-cli PRIVATE
                                        tea24::version
                                        fmt::fmt
                                        CLI11::CLI11
                                        nlohmann_json::nlohmann_json
)

############################################################
# Bonus 3: a plain C program using the C API
############################################################

add_executable(${PROJECT_NAME}-c c_consumer.c)
# CMake notices that version_library contains C++ code and links with the C++ driver
# (libstdc++ is needed for std::string inside the library).
target_link_libraries(${PROJECT_NAME}-c PRIVATE tea24::version)

############################################################
# Bonus 4: same main.cpp, linked against the SHARED library
############################################################

add_executable(${PROJECT_NAME}-shared main.cpp)
target_include_directories(${PROJECT_NAME}-shared PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_compile_definitions(${PROJECT_NAME}-shared PRIVATE TEA_LINKAGE="dynamisch")
target_link_libraries(${PROJECT_NAME}-shared PRIVATE
                                        tea24::version_shared
                                        fmt::fmt
)

# Diagrams for SOLUTION.md (optional, needs Graphviz):
#   cmake --build build --target exercise-003-diagrams
add_graphviz_diagrams(${PROJECT_NAME}-diagrams "${CMAKE_CURRENT_SOURCE_DIR}/doc")
