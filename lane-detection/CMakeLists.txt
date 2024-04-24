cmake_minimum_required(VERSION 3.10)
project(LaneDetection)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Google Test
find_package(GTest REQUIRED)

# Include directories
include_directories(include)

# Add executable for tests
add_executable(GreetTest tests/unit_test.cc src/cpp/greet.cc)

# Link Google Test and the test executable
target_link_libraries(GreetTest GTest::GTest GTest::Main)
