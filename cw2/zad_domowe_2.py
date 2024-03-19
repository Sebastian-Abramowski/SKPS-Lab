import math
import time
from matplotlib import pyplot as plt

ITERATIONS = 41
MULTIPLIER = math.pi / 40


def pwm_signal(min_hz, max_hz, step):
    start = time.time()
    signal = 1
    x_axis = []
    y_axis = []
    freq = min_hz
    while freq >= min_hz - step:
        if freq > max_hz:
            step = -step

        time.sleep(1 / freq)
        x_axis.append(time.time() - start)
        signal = signal ^ 1
        y_axis.append(signal)
        freq += step

    return x_axis, y_axis


signal = pwm_signal(30, 1000, 90)
plt.step(signal[0], signal[1])
plt.title("Sygnał liniowy 30Hz-1000Hz-30Hz")
plt.savefig("zad2_a.png")

x_axis = []
y_axis = []
for i in range(ITERATIONS):
    x = i
    y = abs(math.sin(MULTIPLIER * i)) * 0.8 + 0.1
    # time.sleep(10/ITERATIONS)
    x_axis.append(MULTIPLIER * i)
    y_axis.append(y)

plt.scatter(x_axis, y_axis)
plt.title(f"Sygnał sinusowy 10%-90%-10%")
plt.savefig("zad2_b.png")
