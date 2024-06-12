# Raport z laboratorium projektowych - nr. 5, 6 - 29.05.2024, 12.06.2024

### Sebastian Abramowski, 325142

### Bogumił Stoma, 325233

---

## Temat: Zdalny oscyloskop 4-kanałowy, oraz sterowanie efektorem na bazie odczytu z ADC + joystick

## Schemat połączenia

<img src="schemat.png" width="500">

## Informace ogólne

Podłączaliśmy LEDy do GPIO 18, 19 do pinów dla PWM
Zastosowaliśmy nakładki w bootloaderze:

- dtoverlay=i2ci
- dtoverlay=pwm2-chan

## Co udało nam się zrobić na pierwszych labach

- przetestowaliśmy wysyłanie testowych informacji (4 flotów) do innego programu obsługujacego LED oraz wysyłanie informacji przez sieć do klienta na hoście, który robił na podstawie tego wykresy

Schemat podłączenia podczas testów:

- przetestowaliśmy przekazywanie wartości do programu sterującego pojedyczym ledem

Schemat podłączenia podczas testów:

<img src="img/lab_pierwszy/ledy.png" width="500">

## Co udało nam sie zrobić na drugich labach

Przetestowaliśmy działanie wszystkiego razem

Testowanie:

<img src="img/lab_drugi/dzialaniae.png" width="500">

Przykład działania

<img src="img/lab_drugi/dziala.png" width="500">

Robienie wykresów na podstawie zmian "w czasie rzeczywistym"

<img src="img/lab_drugi/dziala_wykresy.png" width="500">

## Uruchamienie projektu

- ustawienie wczesniej wspomnianych nakładek w pliku dla bootloadera
- budowa paczki przez specjalne SDK oraz Makefila
- instalacja pakietu
- stworzenie kolejki (czyli piku) przez który wysyłamy dane od programu 'master' do programu 'slave', który steruje ledem przez `mkfifo tmp/singal_to_led_fifo`
- odpalenie w tle programu `slave &`
- odpalenie programu `master`
- na hoście odpalenie pliku client.py z odpowiednim adresem IP, który robi wykresy

błędem było danie takiego samego czasu spania w masterze i w slavie

problem z RPi

## Co znajduje się w konkretnych plikach
