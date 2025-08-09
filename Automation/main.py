import matplotlib.pyplot as plt
from controller import PID
from plant import MotorPlant


if __name__ == "__main__":
    # while True:
    #     s = input("Enter 'run' to start the simulation or 'exit' to quit: ").strip().lower()
    #     if s == 'quit':
    #         break
    #     else:
    #         print(s)

    a = [1, 2, 3]
    b = (1, 2, 2, 3)
    c = {1, 2, 3, 3}
    d = {1: "one", 2: "two", 3: "three"}
    print(type(a), type(b), type(c), type(d))

    print(a.__sizeof__(), b.__sizeof__(), c.__sizeof__())
    print(a)
    print(b)
    print(c)

    

    # # Example usage
    # pid_controller = PID(kp=3.0, ki=3, kd=0.01, cycT=0.01)
    # motor_plant = MotorPlant(gain=1.0, time_constant=1.5, cycT=0.01)
    # setpoint = 10.0  # Desired speed

    # speeds = []
    # controls = []
    # setpoints = []

    # for i in range(2000):
    #     error = setpoint - motor_plant.speed
    #     control_signal = pid_controller.update(error)
    #     motor_speed = motor_plant.update(control_signal)
    #     speeds.append(motor_speed)
    #     controls.append(control_signal)
    #     setpoints.append(setpoint)

    #     # if i % 50 == 0:
    #     #     print(f"Speed: {motor_speed:.2f}, Control Signal: {control_signal:.2f}")

    # # Plotting the results
    # plt.figure(figsize=(10, 5))
    # plt.plot(speeds, label="Motor Speed")
    # plt.plot(controls, label="Control Signal")
    # plt.plot(setpoints, label="Setpoint", linestyle="--")
    # plt.xlabel("Step")
    # plt.ylabel("Value")
    # plt.title("PID Control Simulation")
    # plt.legend()
    # plt.grid(True)

    # # 参数字符串
    # param_text = (
    #     f"system_define cycT={pid_controller.cycT}\n"
    #     f"pid: "
    #     f"kp={pid_controller.kp}, "
    #     f"ki={pid_controller.ki}, "
    #     f"kd={pid_controller.kd}\n"
        
    #     f"motor_plant: "
    #     f"gain={motor_plant.gain}, "
    #     f"tc={motor_plant.time_constant}"
    # )

    # # 在图右上角标注参数
    # plt.gca().text(
    #     0.98, 0.98, param_text,
    #     transform=plt.gca().transAxes,
    #     fontsize=10,
    #     verticalalignment='top',
    #     horizontalalignment='right',
    #     bbox=dict(boxstyle="round,pad=0.5", facecolor="wheat", alpha=0.5)
    # )

    # plt.show()