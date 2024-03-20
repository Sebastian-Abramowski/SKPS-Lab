from gpio4 import SysfsGPIO
from time import sleep

duration = 10
epsilon = 1e-6


def calc_periods(frequency, duty_cycle):
    period = 1 / frequency
    high_signal_period = period * duty_cycle
    low_signal_period = period - high_signal_period
    return high_signal_period, low_signal_period


def variable_duty_cycle(time, duration, min_duty=0, max_duty=1):
    half_duration = duration / 2
    if time <= half_duration:
        return min_duty + (max_duty - min_duty) * (time / half_duration)
    else:
        return max_duty - (max_duty - min_duty) * (
            (time - half_duration) / half_duration)


def generate_values_for_pwm(gpio, frequency=100):
    current_t = 0
    while current_t < duration:
        variable_duty = variable_duty_cycle(current_t, duration)
        high_period, low_period = calc_periods(frequency, variable_duty)
        gpio.value = 1
        current_t += high_period
        sleep(high_period)
        gpio.value = 0
        current_t += epsilon
        gpio.value = 0
        current_t += low_period
        sleep(low_period)


gpio = SysfsGPIO(27)
gpio.export = True
gpio.direction = 'out'
generate_values_for_pwm(gpio)
gpio.export = False
