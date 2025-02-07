FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(UnitTests test_main.cpp)
target_link_libraries(UnitTests PRIVATE OysterEngine gtest gtest_main)

add_test(NAME EngineTests COMMAND UnitTests)
