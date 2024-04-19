# Lane Detection using Hough Transform

This project is developed as the final project for the EECE5639 Computer Vision course, Spring 2024, at Northeastern University. The project focuses on detecting lanes in road images and videos using the Hough Transform algorithm. This README provides an overview of the project, its contents, and instructions for usage.

## Demo

![figure_0029](/samples/output_video_0029.gif)
![figure_0027](/samples/output_video_0027.gif)
![figure_0002](/samples/output_video_0002.gif)

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
├── bin/
│   └── run.bash
│
├── data/
│   ├── drive_0001_sync/
│   │   ├── image_00/
│   │   │   ├── data/
│   │   │   │   ├── 0000000000.png
│   │   │   │   └── ...
│   │   │   ├── timestamps.txt
│   │   ├── image_01/
│   │   ├── image_02/
│   │   ├── image_03/
│   │   └── ...
│   ├── videos/
│   │   ├── video1.mp4
│   │   ├── video2.mp4
│   │   └── video3.mp4
│   └── ...
│
├── include/
│   ├── capture.h
│   ├── custom_exception.h
│   ├── custom_types.h
│   ├── edge_features.h
│   ├── filters.h
│   └── ...
│
├── src/
│   ├── capture.cc
│   ├── custom_exception.cc
│   ├── edge_features.cc
│   ├── filters.cc
│   ├── lane-detect.py
│   ├── main.cc
│   └── ...
│
├── tests/
│   ├── unit_test.cc
│   └── ...
│
└── .gitignore
└── CMakeLists.txt
└── LICENSE
└── proposal.pdf
└── README.md
```

## Dependencies

- [OpenCV](https://opencv.org/)
- [GTest](http://google.github.io/googletest/)

## Installation

```bash
  # Clone the repository to your local machine
  git clone https://gitlab.com/sriram.ab/lane-detection.git

  # worksace
  cd lane-detection

  # Install KITTI dataset from from this link and store in workspace directory
  # https://northeastern-my.sharepoint.com/:f:/r/personal/sriram_ab_northeastern_edu/Documents/EECE5639%20Project/data?csf=1&web=1&e=o6OaiN)

  # check g++ version (only gcc)
  g++ --version
```

## Tests

```bash
  # root directory
  mkdir build
  cd build

  # initializing gtests inside build/
  cmake ..
  make

  # running tests
  ./<TestExecutable> --gtest_filter=<TestSuite.TestCase>
```

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

We would like to express our gratitude to our instructor and teaching assistants for their guidance and support throughout the course. We would also like to thank the people behind the collection of the KITTI dataset.

## References

- [KITTI dataset](https://www.cvlibs.net/datasets/kitti/index.php)
- [OpenCV](https://opencv.org/)
- [Advanced Lane Detection for Autonomous Vehicles using Computer Vision techniques by Raj Uppula](https://towardsdatascience.com/advanced-lane-detection-for-autonomous-vehicles-using-computer-vision-techniques-f229e4245e41)
-  [Simple Lane Detection with OpenCV by Matt Hardwick](https://medium.com/@mrhwick/simple-lane-detection-with-opencv-bfeb6ae54ec0)
- [Akram et. al. Preprocessing Methods of Lane Detection and
  Tracking for Autonomous Driving](https://arxiv.org/pdf/2104.04755.pdf)
