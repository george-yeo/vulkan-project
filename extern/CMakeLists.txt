include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# Fetch glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# Link Vulkan SDK (if available)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK not found. Install it and set VULKAN_SDK environment variable.")
endif()
