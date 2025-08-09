class PID:
    def __init__(self, kp, ki, kd, cycT):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.cycT = cycT
        self.error = 0.0
        self.last_error = 0.0
        self.integral = 0.0
        self.derivative = 0.0
        self.output = 0.0
        self.P = self.I = self.D = 0.0

    def update(self, error: float = None) -> float:
        self.error = error
        self.P = self.kp * self.error
        self.integral += self.error * self.cycT
        self.I = self.ki * self.integral
        self.derivative = (self.error - self.last_error) / self.cycT
        self.D = self.kd * self.derivative
        self.last_error = self.error
        return self.P + self.I + self.D

    def __str__(self):
        return f"PID(p={self.p}, i={self.i}, d={self.d})"
    def __repr__(self):
        return self.__str__()




