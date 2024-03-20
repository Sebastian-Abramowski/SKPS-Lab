import time
import gpio4

buzzer = gpio4.SysfsGPIO(17)
buzzer.export = True
buzzer.direction = 'out'

c_dur_frequencies = [261.63, 293.66, 329.63, 349.23, 392, 440, 493.88,
                     523.25, 587.33, 659.25, 698.46, 783.99, 880, 987.77]

duty_cycle = 0.5

for freq in c_dur_frequencies:
    start = time.time()
    stop = 0
    period = 1/freq
    while (stop - start < 1):
        buzzer.value = 1
        time.sleep(duty_cycle * period)
        buzzer.value = 0
        time.sleep((1 - duty_cycle) * period)
        stop = time.time()

buzzer.export = False
