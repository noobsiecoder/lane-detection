import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import glob
import os


PATHS = ["./data/drive_0001_sync/image_02/data/",
         "./data/drive_0002_sync/image_02/data/",
         "./data/drive_0011_sync/image_02/data/",
         "./data/drive_0015_sync/image_02/data/",
         "./data/drive_0027_sync/image_02/data/",
         "./data/drive_0029_sync/image_02/data/",
         "./data/drive_0032_sync/image_02/data/",
         "./data/drive_0038_sync/image_02/data/",
         "./data/drive_0042_sync/image_02/data/"
         ]


def capture_image(path):
    return cv.imread(path)


def grayscale(img):
    return cv.cvtColor(img, cv.COLOR_BGR2GRAY)


def roi(img: cv.typing.MatLike):
    # Define the region of interest (ROI) coordinates (x, y, width, height)
    x = 400  # x-coordinate of the top-left corner of the ROI
    y = 200   # y-coordinate of the top-left corner of the ROI
    width = 500  # Width of the ROI
    height = img.shape[0]   # Height of the ROI

    # Crop the image using numpy slicing
    return img[y:y+height, x:x+width]


def dewarp(img, matrix, old_points, new_points):
    inv_matrix = np.linalg.inv(matrix)
    # print(old_points[3,0])
    return cv.warpPerspective(img, inv_matrix, (int(old_points[3, 0]), int(old_points[3, 1])))


def planar_warp(img):
    # print(img.shape)
    # cv.circle(img, (130, 1), 5, (255, 0, 0), 5)
    # cv.circle(img, (250, 1), 5, (0, 255, 0), 5)
    # cv.circle(img, (0, img.shape[0]), 5, (0, 0, 255), 5)
    # cv.circle(img, (450, img.shape[0]), 5, (255, 255, 0), 5)

    old_points = np.float32(
        [[100, 10], [300, 10], [1, img.shape[0]], [450, img.shape[0]]])
    new_points = np.float32([[0, 0], [500, 0], [0, 500], [500, 500]])
    matrix = cv.getPerspectiveTransform(old_points, new_points)
    return (cv.warpPerspective(img, matrix, (500, 500)), matrix, old_points, new_points)


def gaussian_blur(img):
    return cv.GaussianBlur(img, (5, 5), 10.0)


def bilateral_blur(img):
    return cv.bilateralFilter(img, 2, 0.5, 10)


def dilate(img):
    # Morphological operations (dilation)
    kernel = np.ones((5, 5), np.uint8)
    return cv.dilate(img, kernel, iterations=1)


def erode(img):
    # Morphological operations (dilation)
    kernel = np.ones((5, 5), np.uint8)
    return cv.erode(img, kernel, iterations=1)


def canny(img):
    return cv.Canny(img, 100, 150)


def hough_lines(o_img, img, matrix):
    # Perform Hough Line Transform
    # lines = cv.HoughLines(img, 1, np.pi / 180, 255, np.array([]), min_theta=0.0, max_theta=np.pi / 2)
    inv_matrix = np.linalg.inv(matrix)
    count = 0
    lines = cv.HoughLinesP(img, 1, np.pi / 180, 20,
                           minLineLength=100, maxLineGap=90)

    # Initialize a list to store the de-warped lines
    de_warped_lines = []

    # Define the top-left corner coordinates of the ROI in the original image
    top_left_x = 400
    top_left_y = 200

    # Apply the inverse transformation to each endpoint of the detected lines
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line[0]
            # Transform the endpoints from the warped image to the original image
            p1 = np.array([[x1, y1, 1]]).T
            p2 = np.array([[x2, y2, 1]]).T
            p1_de_warped = np.dot(inv_matrix, p1)
            p2_de_warped = np.dot(inv_matrix, p2)
            # Convert homogeneous coordinates to regular coordinates
            p1_de_warped = (p1_de_warped[:2] / p1_de_warped[2]).astype(int)
            p2_de_warped = (p2_de_warped[:2] / p2_de_warped[2]).astype(int)
            # Adjust the coordinates based on the top-left corner of the ROI
            p1_de_warped[0] += top_left_x
            p1_de_warped[1] += top_left_y
            p2_de_warped[0] += top_left_x
            p2_de_warped[1] += top_left_y

            de_warped_lines.append(
                [p1_de_warped[0], p1_de_warped[1], p2_de_warped[0], p2_de_warped[1]])

    # Draw the de-warped lines on the original image
    for line in de_warped_lines:
        x1, y1, x2, y2 = line
        # print(x1[0], y1)
        cv.line(o_img, (x1[0], y1[0]), (x2[0], y2[0]),
                (0, 0, 255), 6)  # Example: Draw a line

    # for line in lines:
    #     # original_points = cv.perspectiveTransform(np.float32(line[0].reshape(-1, 1, 2)), np.float32(inv_matrix))
    #     # x,y = original_points
    #     x1,y1,x2,y2 = line[0]
    #     cv.line(o_img,(x1, y1), (x2, y2),(255, 0,0),10)
    #     # cv.line(o_img,(x1,y1),(x2,y2),(255, 0,0),10)
    #     count += 1

    return o_img


def plot_graph(img):
    for i in range(9):
        plt.subplot(3, 3, i + 1)
        plt.imshow(img[i])
        plt.title(f'Image {i+1}')
    plt.show()


if __name__ == "__main__":
    imgs = []
    # absolute path to search all text files inside a specific folder
    path = r'./data/drive_0029_sync/image_02/data/'
    image_files = sorted([os.path.join(path, file) for file in os.listdir(
        path) if file.endswith(('.png', '.jpg'))])
    # print(image_files)
    for file in image_files:
        img = capture_image(file)
        roi_img = roi(img)
        warp_img, matrix, old_points, new_points = planar_warp(roi_img)
        g_img = grayscale(warp_img)
        g_blur_img = gaussian_blur(g_img)
        b_blur_img = bilateral_blur(g_img)
        canny_img = canny(g_blur_img)
        dilate_img = dilate(canny_img)
        erode_img = erode(dilate_img)
        houghL_img = hough_lines(img, erode_img, matrix)
        imgs.append(houghL_img)

    # Get the dimensions of the first image
    # print(imgs[0])
    height, width, channels = imgs[0].shape

    # Define the codec and create VideoWriter object
    fourcc = cv.VideoWriter_fourcc(*'XVID')
    output_video = cv.VideoWriter(
        'output_video_0029.avi', fourcc, 10.37, (width, height))

    # Write each image to the video
    for image in imgs:
        output_video.write(image)

    # Release the VideoWriter object and close all windows
    output_video.release()
    cv.destroyAllWindows()

    # plot_graph(imgs)
