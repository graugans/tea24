cmake_minimum_required (VERSION 3.10.2)

# Set the project name to your project name, template isn't very descriptive
project (version_library VERSION 0.1.0 LANGUAGES CXX)
string(TIMESTAMP BUILD_DATE "%Y-%m-%dT%H:%M:%S")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include dependencies
find_package(fmt REQUIRED)

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(version_library STATIC 
    src/version.cpp
)

target_include_directories(version_library
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

# Add libraries
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/version_config.h" @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include") # add the output path to the include PATH