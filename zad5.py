import gpio4
from gpio4 import SysfsGPIO
import time

trigger = gpio4.SysfsGPIO(5)
trigger.export = True
trigger.direction = 'out'

echo = gpio4.SysfsGPIO(6)
echo.export = True
echo.direction = 'in'

gpio_button = SysfsGPIO(10)
gpio_button.export = True
gpio_button.direction = 'in'


while gpio_button.value == 1:
    # wysyłamy krótki impuls
    trigger.value = 1
    time.sleep(0.01)
    trigger.value = 0

    while echo.value == 0:
        continue

    time_start = time.time()

    while echo.value == 1:
        continue

    # liczymy po jakim czasie wrócił impuls
    elapsed_time_in_sec = time.time()-time_start

    # obliczamy odległość zamieniając mikrosekundy na centrymetry
    distance = elapsed_time_in_sec * 10**6 / 58
    print(f'{distance:.2f}')
    time.sleep(0.5)

trigger.export = False
echo.export = False
gpio_button.export = False
