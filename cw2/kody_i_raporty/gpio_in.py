from gpio4 import SysfsGPIO
import time

gpio_led = SysfsGPIO(27)
gpio_led.export = True
gpio_led.direction = 'out'

gpio_button = SysfsGPIO(10)
gpio_button.export = True
gpio_button.direction = 'in'

gpio_led.value = 0

while True:
    if gpio_button.value == 0:
        gpio_led.value = 1 - gpio_led.value
        time.sleep(0.5)

# gpio_led.export = False
# gpio_button.export = False
