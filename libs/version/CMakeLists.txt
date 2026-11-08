cmake_minimum_required (VERSION 3.10.2)

# Eigenes project() -> eigene Variablen PROJECT_NAME / PROJECT_VERSION* für die Bibliothek.
# Diese Werte landen über configure_file() in version_config.h.
project (version_library VERSION 0.1.2 LANGUAGES CXX)

# ACHTUNG: wird beim CONFIGURE ausgewertet, nicht bei jedem Build -> "Configure-Datum"
string(TIMESTAMP BUILD_DATE "%Y-%m-%dT%H:%M:%S")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include dependencies
find_package(fmt REQUIRED)

############################################################
# Generated header (configure step)
############################################################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/version_config.h"
    @ONLY
)

set(VERSION_LIBRARY_SOURCES
    src/version.cpp
    src/version_c_api.cpp
)

############################################################
# Static library (Teil 3)
############################################################

add_library(version_library STATIC ${VERSION_LIBRARY_SOURCES})

# Namespaced alias: a typo like tea24::versoin is a CMake error at configure time,
# whereas a typo in a plain name silently becomes "-lversoin" and fails late in the linker.
add_library(tea24::version ALIAS version_library)

target_include_directories(version_library
    PUBLIC
        # API headers -> needed by the library AND by everybody who links against it
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        # generated version_config.h -> implementation detail of the library
        ${CMAKE_CURRENT_BINARY_DIR}/include
)

# PRIVATE: fmt is only used inside version.cpp, not in version.h.
# For a STATIC library CMake still forwards fmt as $<LINK_ONLY:fmt::fmt> to consumers,
# because the .a file contains unresolved references to fmt.
target_link_libraries(version_library PRIVATE fmt::fmt)

############################################################
# Bonus 4: the same sources as shared library
############################################################

add_library(version_library_shared SHARED ${VERSION_LIBRARY_SOURCES})
add_library(tea24::version_shared ALIAS version_library_shared)

set_target_properties(version_library_shared PROPERTIES
    OUTPUT_NAME version_library          # -> libversion_library.so next to libversion_library.a
    VERSION     ${PROJECT_VERSION}       # -> libversion_library.so.0.1.2
    SOVERSION   ${PROJECT_VERSION_MAJOR} # -> libversion_library.so.0 (ABI version)
)

target_include_directories(version_library_shared
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_link_libraries(version_library_shared PRIVATE fmt::fmt)

############################################################
# Bonus 1: unit tests for isAtLeast() and friends
############################################################

if(BUILD_TESTS)
    add_executable(version_library_test test/version_test.cpp)
    target_link_libraries(version_library_test PRIVATE tea24::version Catch2::Catch2WithMain)
    add_catch2_test(TARGET version_library_test)
endif()
