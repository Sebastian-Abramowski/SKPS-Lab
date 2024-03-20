import time
import gpio4

buzzer = gpio4.SysfsGPIO(17)
buzzer.export = True
buzzer.direction = 'out'

frequencies = [261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25, 587.33, 659.25, 698.46, 783.99, 880, 987.77]

duty_cycle = 0.5

for freq in frequencies:
    start = time.time()
    stop = 0
    while (stop - start < 1):
        buzzer.value = 1
        time.sleep(duty_cycle/freq)
        buzzer.value = 0
        time.sleep(duty_cycle/freq)
        stop = time.time()

buzzer.export = False
