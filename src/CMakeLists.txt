# Automatically find and include all .cpp files in src/ folder
file(GLOB SOURCES "*.cpp")

# Create the main executable
add_executable("oyster_engine" ${SOURCES} "oys_pipeline.cpp")