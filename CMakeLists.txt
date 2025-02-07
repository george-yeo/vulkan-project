cmake_minimum_required(VERSION 3.20)
project(OysterEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable cross-platform support
if(WIN32)
    set(OS_PLATFORM "Windows")
elseif(APPLE)
    set(OS_PLATFORM "MacOS")
elseif(UNIX)
    set(OS_PLATFORM "Linux")
endif()

# Enable debugging symbols
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add external dependencies
add_subdirectory(extern)

# Add engine source
add_subdirectory(src)

# Enable unit tests
enable_testing()
add_subdirectory(tests)