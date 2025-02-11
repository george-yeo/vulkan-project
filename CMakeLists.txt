cmake_minimum_required(VERSION 3.20)
project(OysterEngine VERSION 1.0)

# Include FetchContent for downloading dependencies
include(FetchContent)
include(CTest)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and configure GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Download and configure GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

# Download and configure GoogleTest for testing
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# Download stb for image loading
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

# Make the dependencies available
FetchContent_MakeAvailable(glfw glm googletest stb)

# Find Vulkan package
find_package(Vulkan)

if(NOT Vulkan_FOUND)
    message(FATAL_ERROR 
        "\nVulkan not found! Please install the Vulkan SDK:\n"
        "Windows: https://vulkan.lunarg.com/sdk/home#windows\n"
        "Linux: https://vulkan.lunarg.com/sdk/home#linux\n"
        "macOS: https://vulkan.lunarg.com/sdk/home#mac\n"
        "\nAfter installation, you may need to:\n"
        "1. Restart your computer (Windows)\n"
        "2. Set VULKAN_SDK environment variable\n"
        "3. Source the SDK's setup-env.sh script (Linux)\n"
    )
endif()

# Check for glslc compiler
if(NOT Vulkan_GLSLC_EXECUTABLE)
    message(FATAL_ERROR 
        "\nVulkan shader compiler (glslc) not found!\n"
        "Please ensure the Vulkan SDK is properly installed and the bin directory is in your PATH"
    )
endif()

# Optional: Print Vulkan SDK information
message(STATUS "Found Vulkan:")
message(STATUS "  Version: ${Vulkan_VERSION}")
message(STATUS "  Include Dir: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "  Libraries: ${Vulkan_LIBRARIES}")
message(STATUS "  glslc: ${Vulkan_GLSLC_EXECUTABLE}")

# Find Python for shader compilation
find_package(Python3 REQUIRED)

# Set source files (excluding main.cpp)
set(LIB_SOURCES 
    "src/vulkan/vulkan_instance.cpp"
    "src/vulkan/swapchain.cpp"
    "src/vulkan/vulkan_device.cpp"
    "src/vulkan/pipeline.cpp"
    "src/window/window.cpp"
    "src/core/application.cpp"
)

# Set header files
set(LIB_HEADERS
    "include/vulkan/vulkan_instance.hpp"
    "include/vulkan/swapchain.hpp"
    "include/vulkan/vulkan_device.hpp"
    "include/vulkan/pipeline.hpp"
    "include/window/window.hpp"
    "include/core/application.hpp"
)

# Set test files
set(TEST_SOURCES
    "tests/vulkan_tests.cpp"
)

# Shader files
file(GLOB SHADER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag
)
message(STATUS "Found shaders: ${SHADER_SOURCES}")

# Function to compile shaders
set(SPIRV_BINARY_FILES "")
function(compile_shader SHADER_SOURCE)
    get_filename_component(FILENAME ${SHADER_SOURCE} NAME)
    get_filename_component(DIRNAME ${SHADER_SOURCE} DIRECTORY)
    get_filename_component(EXTENSION ${SHADER_SOURCE} EXT)
    
    string(SUBSTRING ${EXTENSION} 1 -1 STAGE)
    set(SPIRV "${CMAKE_BINARY_DIR}/shaders/${FILENAME}.spv")
    
    message(STATUS "Setting up compilation for: ${FILENAME}")
    message(STATUS "Output will be: ${SPIRV}")
    
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders/"
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER_SOURCE} -o ${SPIRV}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling shader ${FILENAME}"
        VERBATIM
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    set(SPIRV_BINARY_FILES ${SPIRV_BINARY_FILES} PARENT_SCOPE)
    message(STATUS "Current SPIRV_BINARY_FILES: ${SPIRV_BINARY_FILES}")
endfunction()

# Compile shaders
foreach(SHADER ${SHADER_SOURCES})
    compile_shader(${SHADER})
endforeach()

add_custom_target(shaders ALL 
    DEPENDS ${SPIRV_BINARY_FILES}
    COMMENT "Compiling all shaders"
    VERBATIM
)
message(STATUS "Shader target depends on: ${SPIRV_BINARY_FILES}")

# Create the engine library
add_library(${PROJECT_NAME}_lib STATIC
    ${LIB_SOURCES}
    ${LIB_HEADERS}
)
add_dependencies(${PROJECT_NAME}_lib shaders)

# Create main executable
add_executable(${PROJECT_NAME} src/main.cpp)
add_dependencies(${PROJECT_NAME} shaders)

# Create test executable
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

# Set include directories for the library
target_include_directories(${PROJECT_NAME}_lib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Vulkan_INCLUDE_DIRS}
        ${stb_SOURCE_DIR}
)

# Link libraries to the engine library
target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC 
        Vulkan::Vulkan
        glfw
        glm
)

# Link the main executable with the engine library
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_lib
)

# Configure test executable
target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        GTest::gtest_main
        ${PROJECT_NAME}_lib
)

# Platform-specific configurations
if(WIN32)
    target_compile_definitions(${PROJECT_NAME}_lib
        PUBLIC 
        VK_USE_PLATFORM_WIN32_KHR
        NOMINMAX 
        WIN32_LEAN_AND_MEAN
    )
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME}_lib
        PUBLIC 
        VK_USE_PLATFORM_XCB_KHR
    )
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME}_lib
        PUBLIC 
        VK_USE_PLATFORM_METAL_EXT
    )
endif()

# Compile definitions for debug/release
target_compile_definitions(${PROJECT_NAME}_lib
    PUBLIC
        $<$<CONFIG:Debug>:DEBUG_MODE>
        $<$<CONFIG:Release>:RELEASE_MODE>
)

# Enable compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME}_lib PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME}_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Register tests
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)