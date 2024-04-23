import cv2 as cv
import numpy as np
import math
from lane_tracking import LaneTracker

def apply_canny_edge_detection(frame, upper_thresh=30, lower_thresh=10):
    # Convert image to HSV colorspace
    hsv_image = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    # Define range of colors in HSV for yellow and white
    lower_yellow = np.array([30, 80, 80])
    upper_yellow = np.array([255, 255, 255])
    lower_white = np.array([0, 0, 200])
    upper_white = np.array([255, 20, 255])

    # Threshold the HSV image to get only yellow and white colors
    yellow_mask = cv.inRange(hsv_image, lower_yellow, upper_yellow)
    white_mask = cv.inRange(hsv_image, lower_white, upper_white)
    mask = cv.bitwise_or(yellow_mask, white_mask)

    # Bitwise-AND mask and the original image
    color_isolated = cv.bitwise_and(frame, frame, mask=mask)

    
    grayscale = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    blurred = cv.GaussianBlur(grayscale, (5, 5), 0)
    edges = cv.Canny(blurred, lower_thresh, upper_thresh)

    ker_size = 3
    kernel = np.ones((ker_size,ker_size))
    closed = cv.dilate(edges, kernel)
    
    return closed

def define_region_of_interest(edges):
    imshape = edges.shape
    mask = np.zeros_like(edges)
    ignore_mask_color = 255

    # Define a trapezoid to focus on lanes in the distance
    vertices = np.array([[
        (0, imshape[0]),  # Bottom left corner
        (imshape[1] * 0.45, imshape[0] * 0.6),  # Top left corner
        (imshape[1] * 0.55, imshape[0] * 0.6),  # Top right corner
        (imshape[1], imshape[0])  # Bottom right corner
    ]], dtype=np.int32)

    # Fill the defined polygon with white color in the mask image
    cv.fillPoly(mask, vertices, ignore_mask_color)

    # Masking the edges image with the defined polygon
    masked_edges = cv.bitwise_and(edges, edges, mask=mask)
    return masked_edges

def draw_lane_lines(frame, lanes_left, lanes_right):
    lane_visualization = np.zeros_like(frame)
    for lane in lanes_left + lanes_right:
        x1, y1, x2, y2 = lane
        cv.line(lane_visualization, (x1, y1), (x2, y2), (0, 255, 0), 5)
    return lane_visualization

def debug_draw(frame, lanes):
    lane_visualization = np.zeros_like(frame)
    for lane in lanes:
        x1, y1, x2, y2 = lane
        cv.line(lane_visualization, (x1, y1), (x2, y2), (0, 255, 0), 5)
    return lane_visualization


def verify_lines(lanes, side):
    c = 20
    AGC_verifed_lanes = []

    if side == 'l':
        lower_bound = -90
        upper_bound = -45 + c
    elif side == 'r':
        lower_bound = 45 - c
        upper_bound = 90

    for [x1, y1, x2, y2] in lanes:
        fit = np.polyfit((x1, x2), (y1, y2), 1)
        slope = math.degrees(fit[0])

        if lower_bound <= slope and slope <= upper_bound:
            AGC_verifed_lanes.append([x1, y1, x2, y2])
    
    return AGC_verifed_lanes

def split_lanes(width, hough_lines):
    left_lanes = []
    right_lanes = []

    for detec_line in hough_lines:
        x1, y1, x2, y2 = detec_line.reshape(4)        
        if (x1 + x2)/2 > width/2:
            right_lanes.append([x1, y1, x2, y2])
        else:
            left_lanes.append([x1, y1, x2, y2])

    return left_lanes, right_lanes

def calculate_slope_intercept(lanes):
    slopes = []
    intercepts = []
    for line in lanes:
        x1, y1, x2, y2 = line
        parameters = np.polyfit((x1, x2), (y1, y2), 1)
        slope, intercept = parameters
        slopes.append(slope)
        intercepts.append(intercept)
    return slopes, intercepts

def median_slope_intercept(slopes, intercepts):
    median_slope = np.median(slopes)
    median_intercept = np.median(intercepts)
    return median_slope, median_intercept

def calculate_median_lines(left_lanes, right_lanes):
    left_slopes, left_intercepts = calculate_slope_intercept(left_lanes)
    right_slopes, right_intercepts = calculate_slope_intercept(right_lanes)

    median_left_slope, median_left_intercept = median_slope_intercept(left_slopes, left_intercepts)
    median_right_slope, median_right_intercept = median_slope_intercept(right_slopes, right_intercepts)

    return (median_left_slope, median_left_intercept), (median_right_slope, median_right_intercept)

def calculate_intersection_and_bottom(median_left, median_right, frame_height):
    median_left_m, median_left_b = median_left
    median_right_m, median_right_b = median_right
    try:
        x_intersect = (median_left_b - median_right_b) / (median_right_m - median_left_m)
        y_intersect = median_right_m * x_intersect + median_right_b
    except ZeroDivisionError:
        x_intersect, y_intersect = None, None  
    try:
        left_bottom = (frame_height - median_left_b) / median_left_m
        right_bottom = (frame_height - median_right_b) / median_right_m
    except ZeroDivisionError:
        left_bottom, right_bottom = None, None  

    return (x_intersect, y_intersect), (left_bottom, right_bottom)

def draw_optimized_line(frame, left_bottom_median, right_bottom_median, x_intersect_median, y_intersect_median):
    # Create a blank image (same size as frame) to draw lines on
    line_image = np.zeros_like(frame)
    
    # Draw the left lane line
    if left_bottom_median is not None and x_intersect_median is not None and y_intersect_median is not None:
        cv.line(
            line_image,
            (np.int_(left_bottom_median), frame.shape[0]),
            (np.int_(x_intersect_median), np.int_(y_intersect_median)),
            (255, 0, 0), 5
        )

    # Draw the right lane line
    if right_bottom_median is not None and x_intersect_median is not None and y_intersect_median is not None:
        cv.line(
            line_image,
            (int(right_bottom_median), frame.shape[0]),
            (int(x_intersect_median), int(y_intersect_median)),
            (0, 0, 255), 5
        )

    # Combine the line image with the original frame
    lane_edges = cv.addWeighted(frame, 0.8, line_image, 1, 0)
    
    return lane_edges

def lane_tracking(lanes_left, lanes_right, frame, tracker):
    # Calculate median slope and intercept for both left and right lanes
    median_left, median_right = calculate_median_lines(lanes_left, lanes_right)

    # Calculate the intersection point of the median lines and the points at the bottom of the frame
    intersection, bottom_points = calculate_intersection_and_bottom(median_left, median_right, frame.shape[0])

    # Prepare the new history array with the current frame's calculated points
    new_history = [bottom_points[0], bottom_points[1], intersection[0], intersection[1]]

    # Update the tracker with the new history data
    tracker.update_history(new_history)

    # Retrieve the smoothed values (medians over the history) from the tracker
    smoothed_values = tracker.get_median_from_history()

    return smoothed_values


tracker = LaneTracker(num_frames_to_median=19)
# Main execution block
video_capture = cv.VideoCapture(r"C:\Users\josep\Downloads\input.mp4")
while video_capture.isOpened():
    successful_frame_read, frame = video_capture.read()
    if not successful_frame_read:
        break

    # Getting the edge image and defining roi
    canny_edges = apply_canny_edge_detection(frame)
    roi_frame = define_region_of_interest(canny_edges)
    cv.imshow("canny", roi_frame)
    # Getting the hough lines from the image
    hough_lines = cv.HoughLinesP(roi_frame, 2, np.pi / 180, 30, np.array([]), minLineLength=20, maxLineGap=1)
    dim = np.shape(frame)
    
    # Splitting the left and right lanes
    lanes_left, lanes_right = split_lanes(dim[1], hough_lines)

    # Filtering the lines using AGC
    lanes_left = verify_lines(lanes_left, "l")
    lanes_right = verify_lines(lanes_right, "r")

    #lane tracking
    smoothed_values = lane_tracking(lanes_left, lanes_right, frame, tracker)
    
    #lane_lines_image = draw_lane_lines(frame, lanes_left, lanes_right)
    combined_output = draw_optimized_line(frame, smoothed_values[0], smoothed_values[1],smoothed_values[2],smoothed_values[3])
    #combined_output = cv.addWeighted(frame, 0.9, lane_lines_image, 1, 1)
    cv.imshow("Lane Lines", combined_output)

    # Debug block, pay no heed
    #left_lane_image = debug_draw(frame, lanes_left)
    #right_lane_image = debug_draw(frame, lanes_right)

    #debug_line = lane_lines_image + canny_edges.reshape(dim[0],dim[1],1)

    #combined_output_left = cv.addWeighted(frame, 0.9, left_lane_image, 1, 1)
    #combined_output_right = cv.addWeighted(frame, 0.9, right_lane_image, 1, 1)

    #cv.imshow("Left", combined_output_left)
    #cv.imshow("Right", combined_output_right)

    if cv.waitKey(10) & 0xFF == ord('q'):
        break

video_capture.release()
cv.destroyAllWindows()
