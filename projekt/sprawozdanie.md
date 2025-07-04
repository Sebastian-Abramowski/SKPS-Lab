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

- Przetestowaliśmy działanie wszystkiego razem
- Przygotaliśmy package ipk
- Dodaliśmy 2 lampkę led
- Porawiliśmy wygląd GUI
- Przetestowaliśmy różne opóźnienia w dwóch różnych miejscach
- Opóźnienie 0.1s z modułu app_opoznienie_0_1.ipk było za małe, i plik client.py nie nadążał z przetwarzaniem danych, a opóźnienie 0.5s było za duże i interaktywność z joystickiem była słaba. Opóźnienie 0.2s było idealne, interaktywność była dobra, a lampki zmieniały się płynnie i plik client.py nadążał z przetwarzaniem.

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

## Napotkane błedy

- Mieliśmy lekki problem z dobraniem odpowiednich czasów spania w poszczególnych miejscach, aby wszystko działało w miarę płynnie (wynikało głównie to z tego, że nie mogliśmy na szybko zmieniać wartości programu)
- Napotkaliśmy także problemu z konwerterem UART działającym przy RPi - wyświetlał on dziwne znaczki czasem, podobno przez współdzielenie "ścieżki" jakiegoś pinu z "ścieżką" UARTa
- Na końcu mieliśmy problem z zmierzenime różnicy czasu (od wygenerowania danych przez urządzenie do odebrania przez sieć), wydaje nam się, że to przez to, że próbowaliśmy wysłac timestamp w C przez floata (ponieważ wyniki wysyłaliśmy wszystkie inne w floacie więc było wygodnie), przez to chyba mieliśmy za małą dokładność aby obliczyć prawidłową różnicę czasu

## Co znajduje się w konkretnych plikach

- master.c - program główny, który odczytuje dane z konwertera ADC - MCP3424, wysyła odczytane do kolejki (pliku) /etc/singal_to_led_fifo oraz wysyla pakiety UDP z odcztanymi danymi przez gniazdo przez port 8080
- mcp3424.\* - sterownik do ADC dostosowany  do naszych potrzeb
- server.\* - zajmuje się wysylaniem pakietow oraz poczatkowym polaczeniem z klientem
- slave.\* - program odbierający dane od master.c i sterujący dwoma LEDami na podstawie
- Makefile - do tworzenia pakietu ipk przez SDK, który był przerzucany na RPi
- CMakeLists.txt - użyta do przetestowania czy wszystko się kompiluje przed laboratorium


## Co się znajduje w sterowniku mcp3424?

### Opis działania sterownika MCP3424
- Sterownik MCP3424 napisaliśmy samodzielnie, ponieważ jego budowa jest bardzo prosta. Aby odczytać wartości z konwertera, należy postępować według poniższych kroków:

- Otworzyć plik /dev/i2c-1 i zapisać deskryptor pliku.
- W funkcji ioctl podać jako argumenty deskryptor pliku oraz adres konwertera, który w naszym przypadku wynosi 0x68. Piny Adr0 i Adr1 na konwerterze odpowiadają za ustawienie adresu i2c konwertera.
- Jeśli oba zostaną ustawione na 0, domyślny adres wynosi 0b01101000, czyli 0x68 (zgodnie z dokumentacją).
### Należy pamiętać o odpowiednim ustawieniu konfiguracji:
- configuration: bit 0 RDY; 1-2 channels; 3 mode; 4-5 resolution; 6-7 PGA
- Najważniejsze są bity 1-2 odpowiedzialne za wybór kanału (dostępne są 4 kanały).

### Odczyt danych
- W zależności od wartości rozdzielczości (resolution), musimy odpowiednio odczytać dane. Dla rozdzielczości wynoszącej 12 bitów według dokumentacji:

- 12-bitów: MMMMD11 ~ D8 (1. bajt danych) - D7 ~ D0 (2. bajt danych)
Odczytane dane musimy zamienić na liczbę całkowitą (int). Operacja ta wygląda następująco:


- ((reading[0] & 0x0f) << 8) | reading[1]
### Mapowanie odczytu na wartość napięcia
- Ostatnią rzeczą, o której należy pamiętać, jest zmapowanie odczytu na wartość napięcia w zakresie od 0 do 3.3V. Wartość odczytu zależy od ustawionej rozdzielczości. W domyślnym przypadku, czyli dla 12 bitów, wartości te mieszczą się w zakresie od -2048 do 2047.