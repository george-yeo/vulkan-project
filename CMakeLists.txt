cmake_minimum_required(VERSION "3.29")

project("Oyster Engine")

# Define the C++ standard
set("CMAKE_CXX_STANDARD 17")

# Enable testing
enable_testing()

# Add subdirectory for sources and tests
add_subdirectory("src")
add_subdirectory("tests")