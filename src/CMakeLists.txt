set(SRC_FILES
    core/engine.cpp
)

add_library(OysterEngine STATIC ${SRC_FILES})

# Link Vulkan, GLFW, and other dependencies
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(OysterEngine PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(OysterEngine PRIVATE Vulkan::Vulkan glfw)