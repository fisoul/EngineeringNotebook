class MovAvgFilter:
    def __init__(self, window_size: int):
        self.window_size = window_size
        self.values = []
    def add_value(self, value: float):
        self.values.append(value)
        if len(self.values) > self.window_size:
            self.values.pop(0)
    def get_average(self) -> float:
        if not self.values:
            return 0.0
        return sum(self.values) / len(self.values)

class LowPassFilter:
    def __init__(self, alpha: float):
        self.alpha = alpha
        self.last_value = None
    def add_value(self, value: float):
        if self.last_value is None:
            self.last_value = value
        else:
            self.last_value = self.alpha * value + (1 - self.alpha) * self.last_value
    def get_filtered_value(self) -> float:
        return self.last_value if self.last_value is not None else 0.0

class BandPassFilter:
    def __init__(self, low_cutoff: float, high_cutoff: float, sample_rate: float):
        self.low_cutoff = low_cutoff
        self.high_cutoff = high_cutoff
        self.sample_rate = sample_rate
        self.last_value = None
        self.prev_value = None
    def add_value(self, value: float):
        if self.last_value is None:
            self.last_value = value
            self.prev_value = value
        else:
            # Simple band-pass filter logic (not a true implementation)
            filtered_value = (value - self.prev_value) * (self.high_cutoff - self.low_cutoff) / self.sample_rate + self.last_value
            self.prev_value = self.last_value
            self.last_value = filtered_value
    def get_filtered_value(self) -> float:
        return self.last_value if self.last_value is not None else 0.0