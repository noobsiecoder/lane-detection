import numpy as np

class LaneTracker:
    def __init__(self, num_frames_to_median=19):
        self.num_frames_to_median = num_frames_to_median
        self.history = None

    def update_history(self, new_history):
        if self.history is None:
            self.history = np.array([new_history] * self.num_frames_to_median)
        elif not np.isnan(new_history).any():
            self.history[:-1] = self.history[1:]
            self.history[-1] = new_history

    def get_median_from_history(self):
        # Compute the median across the history for each variable
        if self.history is not None:
            left_bottom_median = np.median(self.history[:, 0])
            right_bottom_median = np.median(self.history[:, 1])
            x_intersect_median = np.median(self.history[:, 2])
            y_intersect_median = np.median(self.history[:, 3])
            return left_bottom_median, right_bottom_median, x_intersect_median, y_intersect_median
        else:
            return None, None, None, None
