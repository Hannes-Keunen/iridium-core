function(add_res_target)
    set(TARGET_NAME ${ARGV0})
    add_library(${TARGET_NAME} INTERFACE)
    list(POP_FRONT ARGN)
    foreach(RESOURCE ${ARGN})
        message("Resource ${RESOURCE}")
        get_filename_component(RESOURCE_DIR ${RESOURCE} DIRECTORY)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" RESOURCE_NAME ${RESOURCE})
        set(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_NAME})
        string(REPLACE "/" "-" RESOURCE_TARGET_SUFFIX ${RESOURCE_NAME})
        set(RESOURCE_TARGET ${TARGET_NAME}-${RESOURCE_TARGET_SUFFIX})

        add_custom_target(${RESOURCE_TARGET} DEPENDS ${OUTPUT})
        add_custom_command(
            OUTPUT ${OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCE} ${OUTPUT}
            DEPENDS ${RESOURCE}
            VERBATIM)
        add_dependencies(${TARGET_NAME} ${RESOURCE_TARGET})
    endforeach()
endfunction(add_res_target)