# Renders every *.dot file in a directory to an SVG next to it (needs Graphviz).
#
#   add_graphviz_diagrams(<target> <directory>)
#
# Creates a custom target that is NOT part of ALL, e.g.:
#   cmake --build build --target exercise-002-diagrams
#   cmake --build build --target doc-diagrams
#
# Without Graphviz the normal build is not affected. The diagram targets then
# only print how to install Graphviz and fail.
find_program(DOT_EXECUTABLE dot)

if(DOT_EXECUTABLE)
    message(STATUS "Graphviz found: ${DOT_EXECUTABLE} - diagram targets enabled")
else()
    message(STATUS "Graphviz (dot) not found - diagram targets disabled")
endif()

function(add_graphviz_diagrams target directory)
    if(NOT DOT_EXECUTABLE)
        add_custom_target(${target}
            COMMAND ${CMAKE_COMMAND} -E echo "Graphviz (dot) not found, cannot render the diagrams of ${target}."
            COMMAND ${CMAKE_COMMAND} -E echo "Install it (Debian/Ubuntu: sudo apt-get install graphviz) and run cmake again."
            COMMAND ${CMAKE_COMMAND} -E false
            VERBATIM
        )
        return()
    endif()

    # CONFIGURE_DEPENDS: new *.dot files are picked up without re-running cmake by hand
    file(GLOB dot_sources CONFIGURE_DEPENDS "${directory}/*.dot")
    set(svg_outputs "")
    foreach(dot_file ${dot_sources})
        get_filename_component(name ${dot_file} NAME_WE)
        set(svg_file "${directory}/${name}.svg")
        add_custom_command(
            OUTPUT  ${svg_file}
            COMMAND ${DOT_EXECUTABLE} -Tsvg ${dot_file} -o ${svg_file}
            DEPENDS ${dot_file}
            COMMENT "Rendering ${name}.svg"
        )
        list(APPEND svg_outputs ${svg_file})
    endforeach()
    add_custom_target(${target} DEPENDS ${svg_outputs})
endfunction()
