cmake_minimum_required(VERSION "3.29")

project("oys_engine")

# Define the C++ standard
set("CMAKE_CXX_STANDARD 23")
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the FetchContent module
include(FetchContent)

# Fetch GLFW 3.4
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Fetch GLM (replace 0.9.9.8 with 1.0.1 if available)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Find Vulkan SDK
find_package(Vulkan REQUIRED)

# Copy shaders to build directory
find_program(GLSLC_EXECUTABLE glslc REQUIRED)

if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc shader compiler not found!")
endif()

set(SHADERS
    ${CMAKE_SOURCE_DIR}/shaders/simple_shader.vert
    ${CMAKE_SOURCE_DIR}/shaders/simple_shader.frag
)

set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(COMPILED_SHADER ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv)

    # Add custom command to compile the shader
    add_custom_command(
        OUTPUT ${COMPILED_SHADER}                            # The output file
        COMMAND ${GLSLC_EXECUTABLE} -o ${COMPILED_SHADER} ${SHADER}  # The command
        DEPENDS ${SHADER}                                    # The shader source file                 # Specify that this is a generated file
        COMMENT "Compiling shader ${SHADER} to ${COMPILED_SHADER}"   # Status message
        VERBATIM
    )

    # Collect the output shaders to a list for later use if needed
    list(APPEND COMPILED_SHADERS ${COMPILED_SHADER})
endforeach()

# Add a custom target to compile all shaders
add_custom_target(compile_shaders ALL
    DEPENDS ${COMPILED_SHADERS}
    COMMENT "Compiling all shaders"
)

# Include project header files
include_directories(include)

# Add subdirectory for sources
add_subdirectory("src")

# Link the libraries to the program
target_link_libraries(oyster_engine PRIVATE glfw glm Vulkan::Vulkan)

# Enable testing
enable_testing()

add_subdirectory("tests")