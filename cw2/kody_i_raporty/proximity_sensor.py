import gpio4
import time

SPEED_OF_SOUND = 340

trigger = gpio4.SysfsGPIO(5)
trigger.export = True
trigger.direction = 'out'

echo = gpio4.SysfsGPIO(6)
echo.export = True
echo.direction = 'in'


while True:
    trigger.value = 1
    time.sleep(0.01)
    trigger.value = 0

    while echo.value == 0:
        continue

    time_start = time.time()

    while echo.value == 1:
        continue

    elapsed_time_in_sec = time.time() - time_start

    distance_in_meters = elapsed_time_in_sec * SPEED_OF_SOUND / 2
    distance_in_cm = distance_in_meters * 100

    print(f'{distance_in_cm:.2f}')
    time.sleep(0.5)

# trigger.export = False
# echo.export = False
