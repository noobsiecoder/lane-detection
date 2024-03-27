# Lane Detection using Hough Transform

This project is developed as the final project for the EECE5639 Computer Vision course, Spring 2024, at Northeastern University. The project focuses on detecting lanes in road images and videos using the Hough Transform algorithm. This README provides an overview of the project, its contents, and instructions for usage.

## Team Members
- Abhishek Sriram
- Bhanu Prasad AJ
- Xiatao Wu
- Yinkai Liu

## Overview
Lane detection is a crucial task in computer vision, especially in the domain of autonomous vehicles and advanced driver-assistance systems (ADAS). The Hough Transform is a widely used technique for detecting lines in images. In this project, we implement lane detection using the Hough Transform algorithm in both Python and C++.

## Folder Structure

```
lane_detection_project/
│
├── data/
│   ├── videos/
│   │   ├── video1.mp4
│   │   ├── video2.mp4
│   │   └── video3.mp4
│   └── ...
│
├── include/
│   ├── greet.h
│   └── ...
│
├── src/
│   ├── cpp/
│   │   ├── greet.cc
│   │   └── ...
│   ├── python/
│   │   ├── greet.py
│   │   └── ...
│   └── ...
│
├── tests/
│   ├── greet.cc
│   └── ...
│
└── .gitignore
└── CMakeLists.txt
└── LICENSE
└── README.md
```

## Dependencies

- [OpenCV](https://opencv.org/)
- [GTest](http://google.github.io/googletest/)

## Installation

```bash
  # Clone the repository to your local machine
  git clone https://gitlab.com/sriram.ab/lane-detection.git

  # check g++ version (only gcc)
  g++ --version
```

## Tests

```bash
  # in the root directory
  mkdir build
  cd build
  
  # initializing gtests inside build/
  cmake ..
  make
  
  # running tests
  ./<TestName>
```

## License
This project is licensed under the [MIT License](LICENSE).

## Acknowledgements
We would like to express our gratitude to our instructor and teaching assistants for their guidance and support throughout the course.

## References