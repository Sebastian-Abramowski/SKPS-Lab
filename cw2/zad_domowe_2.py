import math
import time
from matplotlib import pyplot as plt

MIN_HZ = 30
MAX_HZ = 1000
ITERATIONS = 41
MULTIPLIER = math.pi / 40
x_axis = []
y_axis = []
freqs = []

for freq in range(MIN_HZ, MAX_HZ):
    freqs.append(freq)
    # pwm(freq)
for freq in range(MAX_HZ, MIN_HZ, -1):
    freqs.append(freq)
    # pwm(freq)

plt.plot(freqs)
plt.title("Sygnał liniowy 30Hz-1000Hz-30Hz")
plt.savefig("zad1.png")
plt.show()


for i in range(ITERATIONS):
    x = i
    y = abs(math.sin(MULTIPLIER * i)) * 0.8 + 0.1
    # time.sleep(10/ITERATIONS)
    x_axis.append(MULTIPLIER * i)
    y_axis.append(y)

plt.scatter(x_axis, y_axis)
plt.title(f"Sygnał sinusowy 10%-90%-10%")
plt.savefig("zad2.png")
plt.show()
