import cv2 as cv
import numpy as np

def apply_canny_edge_detection(frame, upper_thresh=30, lower_thresh=10):
    grayscale = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    blurred = cv.GaussianBlur(grayscale, (5, 5), 0)
    edges = cv.Canny(blurred, lower_thresh, upper_thresh)
    return edges

def define_region_of_interest(frame):
    frame_height, frame_width = frame.shape[:2]
    top_width = frame_width // 3
    bottom_width = frame_width
    mid_height = frame_height // 2
    mask_vertices = np.array([
        [(frame_width // 2 - top_width // 2, mid_height),
         (frame_width // 2 + top_width // 2, mid_height),
         (frame_width, frame_height),
         (0, frame_height)]
    ])
    mask = np.zeros_like(frame)
    cv.fillPoly(mask, mask_vertices, 255)
    masked_frame = cv.bitwise_and(frame, mask)
    return masked_frame

def identify_lane_lines(frame, detected_lines):
    lanes_left = []
    lanes_right = []
    for line in detected_lines:
        x1, y1, x2, y2 = line.reshape(4)
        fit = np.polyfit((x1, x2), (y1, y2), 1)
        slope = fit[0]
        if slope < 0:
            lanes_left.append([x1, y1, x2, y2])
        else:
            lanes_right.append([x1, y1, x2, y2])
    return lanes_left, lanes_right

def draw_lane_lines(frame, lanes_left, lanes_right):
    lane_visualization = np.zeros_like(frame)
    for lane in lanes_left + lanes_right:
        x1, y1, x2, y2 = lane
        cv.line(lane_visualization, (x1, y1), (x2, y2), (0, 255, 0), 5)
    return lane_visualization

# Main execution block
video_capture = cv.VideoCapture(r"C:\Users\josep\Downloads\input.mp4")
while video_capture.isOpened():
    successful_frame_read, frame = video_capture.read()
    if not successful_frame_read:
        break

    canny_edges = apply_canny_edge_detection(frame)
    roi_frame = define_region_of_interest(canny_edges)
    hough_lines = cv.HoughLinesP(roi_frame, 2, np.pi / 180, 100, np.array([]), minLineLength=100, maxLineGap=50)
    lanes_left, lanes_right = identify_lane_lines(frame, hough_lines)
    
    lane_lines_image = draw_lane_lines(frame, lanes_left, lanes_right)
    combined_output = cv.addWeighted(frame, 0.9, lane_lines_image, 1, 1)
    cv.imshow("Detected Lanes", combined_output)

    if cv.waitKey(10) & 0xFF == ord('q'):
        break

video_capture.release()
cv.destroyAllWindows()
