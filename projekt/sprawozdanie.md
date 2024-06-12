# Raport z laboratorium projektowych - nr. 5, 6 - 29.05.2024, 12.06.2024

### Sebastian Abramowski, 325142

### Bogumił Stoma, 325233

---

## Temat: Zdalny oscyloskop 4-kanałowy, oraz sterowanie efektorem na bazie odczytu z ADC + joystick

## Schemat połączenia

<img src="tutaj będzie schemat połączenia" width="500">

## Fizyczne połączenie

<img src="tutaj będzie jak podłaczylismy" width="500">

## Przykład działania

Wykresy jak nie dotykamy joysticków:

<img src="werwer" width="500">

Wykresy jak przy zmianie joysticków:

<img src="werwerwer" width="500">

Pokazanie działania diody w zależności od ustawienia joysticka:

<img src="wykres" width="500">

<img src="wykres" width="500">

Podłączaliśmy go GPIO 18, 19 do pinów dla PWM

## Co udało nam się zrobić na pierwszych labach

- przetestowaliśmy wysyłanie testowych informacji (4 flotów) do innego programu obsługujacego LED oraz wysyłanie informacji przez sieć do klienta na hoście, który robił na podstawie tego wykresy

- przetestowaliśmy odczytywanie wartości (4 floatów) z przetowrnika ADC - MCP3424

## Co udało nam sie zrobić na drugich labach

## Odpalanie

mkfifo tmp/signal_to_led_fifo
slave &
master

błędem było danie takiego samego czasu spania w masterze i w slavie

problem z RPi

## Co znajduje się w konkretnych plikach
