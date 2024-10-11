cmake_minimum_required(VERSION "3.29")

project("Oyster Engine")

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

# Include project header files
include_directories(include)

# Add subdirectory for sources
add_subdirectory("src")

# Link the libraries to the program
target_link_libraries(oyster_engine PRIVATE glfw glm Vulkan::Vulkan)

# Copy shaders to build directory
file(COPY shaders/simple_shader.vert.spv DESTINATION ${CMAKE_BINARY_DIR}/shaders)
file(COPY shaders/simple_shader.frag.spv DESTINATION ${CMAKE_BINARY_DIR}/shaders)

# Enable testing
enable_testing()

add_subdirectory("tests")