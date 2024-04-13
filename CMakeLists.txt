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

# Add executable for tests
add_executable(main 
    src/custom_exception.cc 
    src/image/capture.cc src/image/filter.cc
    src/image/canny.cc src/image/hough.cc 
    src/main.cc)
add_executable(test 
    src/custom_exception.cc 
    src/image/capture.cc src/image/filter.cc 
    src/image/canny.cc src/image/hough.cc 
    tests/unit_test.cc)

# Link Google Test and the test executable
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(test GTest::GTest GTest::Main ${OpenCV_LIBS})
