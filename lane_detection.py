import cv2 as cv
import numpy as np
import math

from particle_filter import Particle_filter

def apply_canny_edge_detection(frame, upper_thresh=30, lower_thresh=10):
    grayscale = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    blurred = cv.GaussianBlur(grayscale, (5, 5), 0)
    edges = cv.Canny(blurred, lower_thresh, upper_thresh)

    ker_size = 3
    kernel = np.ones((ker_size,ker_size))
    closed = cv.dilate(edges, kernel)
    
    return closed

def define_region_of_interest(frame):
    frame_height, frame_width = frame.shape[:2]
    top_width = frame_width // 3
    bottom_width = frame_width
    mid_height = 3 * frame_height // 4
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


def verify_lines(lanes, side, dim):
    c = 20
    FLAG = True
    LANE_FOUND = False
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
            LANE_FOUND = True
            slope = (y2 - y1) / (x2 - x1)

            y3 = (3 * dim[0]) // 4
            x3 = int((y3 - y2) // slope) + x2

            y0 = dim[1]
            x0 = int((y0 - y2) // slope) + x2
    
            if side == 'l':
                AGC_verifed_lanes.append([x0, y0, x3, y3])
            else:
                AGC_verifed_lanes.append([x3, y3, x0, y0])
    
    return AGC_verifed_lanes
            # if FLAG:
            #     x_new = x0
            #     FLAG = False

            # if side == 'l' and x0 > x_new:
            #     x_new = x0
            # elif side == 'r' and x0 < x_new:
            #     x_new = x0

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

def side_debug(frame, lanes_left, lanes_right, edge = None):

    # Getting the dimensions of the image
    dim = np.shape(frame)

    # Drawing thee lines
    left_lane_image = debug_draw(frame, lanes_left)
    right_lane_image = debug_draw(frame, lanes_right)

    if(edge is not None):
        frame = left_lane_image + edge.reshape(dim[0],dim[1],1)
        frame = right_lane_image + edge.reshape(dim[0],dim[1],1)

    # Overlaying the lines on the image
    combined_output_left = cv.addWeighted(frame, 0.9, left_lane_image, 1, 1)
    combined_output_right = cv.addWeighted(frame, 0.9, right_lane_image, 1, 1)

    # cv.imshow("left", combined_output_left)
    # cv.imshow("right", combined_output_right)

    return np.hstack((combined_output_left, combined_output_right))    

class lane_detec():
    
    def __init__(self, path) -> None:
        
        FIRST_RUN = True
        
        left_lane = []
        right_lane = []

        left_lane_t1 = []
        right_lane_t1 = []

        left_lane_t2 = []
        right_lane_t2 = []

        estimate_l = []
        estimate_r = []

        particle_filter = Particle_filter()

        video_capture = cv.VideoCapture(path)
        while video_capture.isOpened():
            successful_frame_read, frame = video_capture.read()
            if not successful_frame_read:
                break

            # Getting the edge image and defining roi
            canny_edges = apply_canny_edge_detection(frame)
            roi_frame = define_region_of_interest(canny_edges)

            # Getting the hough lines from the image
            hough_lines = cv.HoughLinesP(roi_frame, 2, np.pi / 180, 100, np.array([]), minLineLength=100, maxLineGap=150)
            dim = np.shape(frame)
            
            # Splitting the left and right lanes
            lanes_left, lanes_right = split_lanes(dim[1], hough_lines)

            # Filtering the lines using AGC
            lanes_left = verify_lines(lanes_left, "l", dim)
            lanes_right = verify_lines(lanes_right, "r", dim)

            if FIRST_RUN and len(lanes_left) > 0 and len(lanes_right) > 0:
                xl_targ = lanes_left[0][0]
                xr_targ = lanes_right[0][0]

                for [x0, y0, x1, y1] in lanes_left:
                    if x0 < xl_targ:
                        xl_targ = x0
                        left_lane = [[x0, y0, x1, y1]]

                for [x1, y1, x0, y0] in lanes_right:
                    if x1 < xr_targ:
                        xr_targ = x1
                        right_lane = [[x1, y1, x0, y0]]

                left_lane_t1 = left_lane.copy()
                left_lane_t2 = left_lane.copy()

                right_lane_t1 = right_lane.copy()
                right_lane_t2 = right_lane.copy()

                FIRST_RUN = False

            if len(left_lane_t1) > 0 and len(left_lane_t2) > 0:
                delta_l = np.array(left_lane_t1) - np.array(left_lane_t2)
                estimate_l = np.array(left_lane) + np.array(delta_l)
                estimate_l = estimate_l.tolist()[0]
            else:
                estimate_l = left_lane.copy()
            
            if len(right_lane_t1) > 0 and len(right_lane_t2) > 0:
                delta_r = np.array(right_lane_t1) - np.array(right_lane_t2)
                estimate_r = np.array(right_lane) + np.array(delta_r)
                estimate_r = estimate_r.tolist()[0]
            else:
                estimate_r = right_lane.copy()

            left_lane_t2 = left_lane_t1.copy()
            left_lane_t1 = left_lane.copy()

            right_lane_t2 = right_lane_t1.copy()
            right_lane_t1 = right_lane.copy()

            """ The scoring function goes here"""
            left_lane = particle_filter.get_estimate(lanes_left, estimate_l)
            right_lane = particle_filter.get_estimate(lanes_right, estimate_r)
            # print(left_lane)

            # lane_lines_image = draw_lane_lines(frame, lanes_left, lanes_right)
            lane_lines_image = draw_lane_lines(frame, left_lane, right_lane)
            combined_output = cv.addWeighted(frame, 0.9, lane_lines_image, 1, 1)

            # Uncomment to view the each side
            # combined_output = side_debug(frame, left_lane, right_lane)
            # combined_output = side_debug(frame, lanes_left, lanes_right)
            
            cv.imshow("Lane Lines", combined_output)

            if cv.waitKey(10) & 0xFF == ord('q'):
                break

        video_capture.release()
        cv.destroyAllWindows()

if __name__ == '__main__':
    lane_detec(r"data\video\video2.mp4")