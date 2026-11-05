cmake_minimum_required (VERSION 3.10.2)

# Set the project name to your project name, template isn't very descriptive
project (exercise-002 VERSION 0.0.1 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Include dependencies
find_package(fmt REQUIRED)


# add the executable
add_executable(${PROJECT_NAME} 
                                main.cpp
                                module.cpp
)

# Add libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
                                        fmt::fmt
)

# This creates a custom command that prints out the firmware binary size information.
# Text is the code, data stores variables that have a non-zero initial value and have to be stored in flash,
# bss stores zero initial values that only take up ram.
# dec and hex are just the cumulative size in decimal and hexadecimal notation respectively.
# Example:
# text	   data	    bss	    dec	    hex	filename
# 3432	     20	   1572	   5024	   13a0	most_commented_embedded_cmakelists.elf
# Resources: https://mcuoneclipse.com/2013/04/14/text-data-and-bss-code-and-data-size-explained/
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND size --format=sysv  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
)