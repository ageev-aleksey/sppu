find_program(CPP_LINT cpplint REQUIRED)

function(target_cpplint_disable_parameters target param1)
#TODO
endfunction()


function(target_enable_cpplint target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "argument target is not 'target' <- ${target}")
    endif()

    set(options "")
    set(one_value_args LINE_LENGTH)
    set(multi_value_args FILTERS SOURCES)
    cmake_parse_arguments(ARGUMENT "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    list(LENGTH ARGUMENT_SOURCES NUM_SOURCES)
    if(NUM_SOURCES GREATER 0)
        set(SOURCES ${ARGUMENT_SOURCES})
    else()
        get_property(SOURCES TARGET ${target} PROPERTY SOURCES)
    endif()

    list(LENGTH ARGUMENT_FILTERS NUM_FILTERS)
    if(NUM_FILTERS GREATER 0)
        set(_FILTERS_SETTINGS "--filter=")
        foreach(_filter ${ARGUMENT_FILTERS})
            string(APPEND _FILTERS_SETTINGS "${_filter},")
        endforeach()
            string(REGEX REPLACE ",$" "" FILTERS_SETTINGS ${_FILTERS_SETTINGS})
    endif()

    if(ARGUMENT_LINE_LENGTH)
        set(LINE_LENGTH "--linelength=${ARGUMENT_LINE_LENGTH}")
    endif()


    foreach(src ${SOURCES})
        get_filename_component(_path ${src} ABSOLUTE)
        list(APPEND _sources ${_path})
    endforeach()

    add_custom_target(${target}_cpplint_execute
            COMMAND ${CPP_LINT} ${FILTERS_SETTINGS} ${LINE_LENGTH} ${_sources}
            COMMENT "CppLint Execute")

    add_dependencies(${target} ${target}_cpplint_execute)
endfunction()

