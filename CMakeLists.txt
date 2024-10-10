cmake_minimum_required(VERSION "3.29")

project("Oyster Engine")

# Define the C++ standard
set("CMAKE_CXX_STANDARD 17")

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

# Enable testing
enable_testing()

# Add subdirectory for sources and tests
add_subdirectory("src")
add_subdirectory("tests")

# Link the libraries to the program
target_link_libraries(oyster_engine PRIVATE glfw glm Vulkan::Vulkan)