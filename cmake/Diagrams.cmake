# Renders every *.dot file in a directory to an SVG next to it (needs Graphviz).
#
#   add_graphviz_diagrams(<target> <directory>)
#
# Creates a custom target that is NOT part of ALL, e.g.:
#   cmake --build build --target exercise-002-diagrams
#   cmake --build build --target doc-diagrams
find_program(DOT_EXECUTABLE dot)

function(add_graphviz_diagrams target directory)
    if(NOT DOT_EXECUTABLE)
        message(STATUS "Graphviz (dot) not found - target ${target} not available")
        return()
    endif()

    file(GLOB dot_sources "${directory}/*.dot")
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
