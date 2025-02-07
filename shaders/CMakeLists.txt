file(GLOB SHADER_FILES ${CMAKE_SOURCE_DIR}/shaders/*.vert ${CMAKE_SOURCE_DIR}/shaders/*.frag)

foreach(SHADER IN LISTS SHADER_FILES)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/${FILENAME}.spv
        COMMAND glslc ${SHADER} -o ${CMAKE_BINARY_DIR}/shaders/${FILENAME}.spv
        DEPENDS ${SHADER}
    )
    list(APPEND SPIRV_SHADERS ${CMAKE_BINARY_DIR}/shaders/${FILENAME}.spv)
endforeach()

add_custom_target(Shaders DEPENDS ${SPIRV_SHADERS})
