class MotorPlant:
    def __init__(self, gain: float = 1.0, time_constant: float = 1.0, cycT: float = 0.01):
        self.gain = gain
        self.time_constant = time_constant
        self.cycT = cycT
        self.speed = 0.0

    def update(self, control_input: float) -> float:
        # 一阶惯性系统离散化公式
        self.speed += (self.gain * control_input - self.speed) * (self.cycT / self.time_constant)
        return self.speed