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

# Add executable
# add_executable(opencv_test src/cpp/opencv_test.cc)

# Link OpenCV libraries
# target_link_libraries(opencv_test ${OpenCV_LIBS})

# Add executable for tests
add_executable(ImageProcessTest tests/unit_test.cc src/cpp/image_processor.cc src/cpp/custom_exception.cc)
add_executable(main src/cpp/image_processor.cc src/cpp/lane_detection.cc src/cpp/custom_exception.cc src/cpp/main.cc)

# Link Google Test and the test executable
target_link_libraries(ImageProcessTest GTest::GTest GTest::Main ${OpenCV_LIBS})
target_link_libraries(main ${OpenCV_LIBS})
