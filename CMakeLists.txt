cmake_minimum_required(VERSION 3.10)
project(LaneDetection)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Google Test
find_package(GTest REQUIRED)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(include)

# Add executable for main
add_executable(Main
src/custom_exception.cc
src/capture.cc src/filters.cc src/edge_features.cc
src/main.cc
)

# Add executable for tests
add_executable(test 
    src/custom_exception.cc
    src/capture.cc
    tests/unit_test.cc)

# Link Google Test and the test executable
target_link_libraries(Main ${OpenCV_LIBS})
target_link_libraries(test GTest::GTest GTest::Main ${OpenCV_LIBS})
