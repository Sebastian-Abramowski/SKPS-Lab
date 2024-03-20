from periphery import GPIO, sleep_ms
from time import time, sleep

output = GPIO("/dev/gpiochip0", 18, "out")
frequencies = [261.63, 293.66, 329.63, 349.23, 392., 440., 493.88, 523.25, 587.33, 659.25, 698.46, 783.99, 880, 987.77, 1046.50]

dt = 1e-4
duty_cycle = 0.5

for freq in frequencies:
    start = time()
    print(freq)
    stop = 0
    while (stop - start < 1):
        output.write(True)
        sleep(duty_cycle/freq)
        output.write(False)
        sleep(duty_cycle/freq)
        stop = time()

output.close()
