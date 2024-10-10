# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Create the test executable
add_executable(test_my_program test_my_program.cpp)
target_link_libraries(test_my_program gtest_main)

# Add the test
add_test(NAME MyProgramTest COMMAND test_my_program)