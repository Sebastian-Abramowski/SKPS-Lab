import matplotlib.pyplot as plt

duty_cycle = 0.5
dt = 0.001
duration = 0.25
epsilon = 1e-6


def calc_periods(frequency, duty_cycle):
    period = 1 / frequency
    high_signal_period = period * duty_cycle
    low_signal_period = period - high_signal_period
    return high_signal_period, low_signal_period


def variable_frequency(time, duration, min_freq=30, max_freq=1000):
    half_duration = duration / 2
    if time <= half_duration:
        return min_freq + (max_freq - min_freq) * (time / half_duration)
    else:
        return max_freq - (max_freq - min_freq) * (
            (time - half_duration) / half_duration)


def variable_duty_cycle(time, duration, min_duty=0.1, max_duty=0.9):
    half_duration = duration / 2
    if time <= half_duration:
        return min_duty + (max_duty - min_duty) * (time / half_duration)
    else:
        return max_duty - (max_duty - min_duty) * (
            (time - half_duration) / half_duration)


def generate_values_for_freq_plot():
    values = []
    times = []
    current_t = 0
    while current_t < duration:
        frequency = variable_frequency(current_t, duration)
        high_period, low_period = calc_periods(frequency, duty_cycle)
        values.append(1)
        times.append(current_t)
        values.append(1)
        times.append(current_t + high_period)
        values.append(0)
        times.append(current_t + high_period + epsilon)
        values.append(0)
        times.append(current_t + high_period + epsilon + low_period)
        current_t += high_period + epsilon + low_period
    return times, values


def generate_values_for_fill_plot(frequency=5):
    values = []
    times = []
    current_t = 0
    while current_t < duration:
        variable_duty = variable_duty_cycle(current_t, duration)
        high_period, low_period = calc_periods(frequency, variable_duty)
        values.append(1)
        times.append(current_t)
        values.append(1)
        times.append(current_t + high_period)
        values.append(0)
        times.append(current_t + high_period + epsilon)
        values.append(0)
        times.append(current_t + high_period + epsilon + low_period)
        current_t += high_period + epsilon + low_period
    return times, values


times, values = generate_values_for_freq_plot()
plt.plot(times, values)
plt.xlabel("Czas [s]")
plt.ylabel("Sygnał PWM")
plt.title("Sygnał częstotliwość 30Hz-1000Hz-30Hz")
plt.style.use("fivethirtyeight")
plt.grid(True)
plt.show()
