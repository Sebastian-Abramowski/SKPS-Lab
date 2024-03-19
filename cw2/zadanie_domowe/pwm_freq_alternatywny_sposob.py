import matplotlib.pyplot as plt

duty_cycle = 0.5
dt = 0.001
duration = 0.24
epsilon = 1e-6

values = []
times = []


def calc_periods(frequency, duty_cycle):
    period = 1 / frequency
    high_signal_period = period * duty_cycle
    low_signal_period = period - high_signal_period
    return high_signal_period, low_signal_period


def variable_frequency(t, duration, min_freq=30, max_freq=1000):
    half_duration = duration / 2
    if t <= half_duration:
        return min_freq + (max_freq - min_freq) * (t / half_duration)
    else:
        return max_freq - (max_freq - min_freq) * (
            (t - half_duration) / half_duration)


current_t = 0
while current_t < duration:
    frequency = variable_frequency(current_t, duration)
    high_period, low_period = calc_periods(frequency, duty_cycle)
    print(f"{current_t} : {frequency}")
    values.append(1)
    times.append(current_t)
    values.append(1)
    times.append(current_t + high_period)
    values.append(0)
    times.append(current_t + high_period + epsilon)
    values.append(0)
    times.append(current_t + high_period + epsilon + low_period)
    current_t += high_period + epsilon + low_period

plt.plot(times, values)
plt.xlabel("Czas [s]")
plt.ylabel("Sygnał PWM")
plt.title("Sygnał liniowy 30Hz-1000Hz-30Hz")
plt.style.use("fivethirtyeight")
plt.grid(True)
plt.show()
