# Automatically find and include all .cpp files in src/ folder
file(GLOB SOURCES "*.cpp")

# Create the main executable
add_executable("oyster_engine" ${SOURCES} "oys_pipeline.cpp" "oys_model.cpp"  "oys_renderer.cpp")

if(ENABLE_CHECKSTYLE)
    target_compile_options(oyster_engine PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4          # Set warning level to 4 for MSVC
            /WX          # Treat warnings as errors in MSVC
            /w44365>    # Disable specific MSVC warning
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
            -W4          # Set warning level to 4 for GCC/Clang
            -WX          # Treat warnings as errors in GCC/Clang
            -w44365
        >
    )
endif()