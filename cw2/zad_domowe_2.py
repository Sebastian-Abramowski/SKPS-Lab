import math
import time
from matplotlib import pyplot as plt

ITERATIONS = 41
MULTIPLIER = math.pi / 40


def pwm_signal(min_hz, max_hz, step):

    signal = 1
    x_axis = [0]
    y_axis = [0]
    freq = min_hz
    start = time.time()
    while freq >= min_hz:
        if freq > max_hz:
            step = -step

        time.sleep(1 / freq)
        x_axis.append(time.time() - start)

        y_axis.append(signal)
        signal = signal ^ 1
        freq += step

    return x_axis, y_axis


signal = pwm_signal(30, 1000, 30)
plt.step(signal[0], signal[1])
plt.title("Sygnał liniowy 30Hz-1000Hz-30Hz")
plt.savefig("zad2_a.png")
plt.show()

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
