## Skompilowanie pakietu tak jak na poprzednich labach

Korzystając z SDK:

- dodanie to pliku feeds.conf.default ścieżkę do katalogu z pakietami: `scr-link skps /sciezka/do/.../cw4_owrt_pkg`

Aktualizacja listy pakietów

```
export LANG=C
./scripts/feeds update -a
./scripts/feeds install -p skps -a
```

Kompilacja paczki

```
make package/feeds/skps/cwicz4mak/compile
```

I potem przerzucenie pakietu na RPi

## Cw4a

Uruchamianie

```
cw4a liczba_klientów liczba_próbek okres_próbkowania czas_przetwarzania
```

liczba_klientów - ile programów cw4b odpala (generuje zestawy próbek i przekazuje je przez pamięć dzieloną do uruchomionych instancji cw4b - po wygnerowaniu i odebraniu zleconej liczby zestawów próbek cały system kończy pracę)

czas_przetwarzania - liczba wykonań pętli

okres_próbkowania - w mikrosekundach (proponowane 10_000 - czyli 100 zestawów próbek na sekundę) - ile śpimy w pojedynczej pętli

server.txt - w nim będą zapisane momenty pobrania kolejnych zestawów próbek w mikrosekundach

cli_N.txt - zapiasne są momenty pobrania i dostarczenia kolejnych zestawów próbek oraz opóźnienie dostarczania danych

## Cw4b

pierwszy arguemnt - numer klienta
drugi - liczba próbek do przetworzenia, na ile próbek oczekuje
trzeci - ilość iteracji do symulowania opóźnienia

Czyta dane napisane przez serwer, jak są nowe dane to je pobiera,
odczytuje czas dostarczenia próbek i liczy różnicę czasu,
symuluje opóźnienie w przetwarzaniu danych, jak przetworzy określną
liczbę próbek to się kończy

## Przetestowania działania programów na gospodarzu

```
export PATH=$PATH:./
make
```

## Zad. 3

Trzeba będzie pozmieniać te wartości ostatnie - czas przetwarzania

```
stress-ng --matrix 0 -t 30s
cw4a 3 300 10000 200000

stress-ng --matrix 0 -t 30s
cw4a 3 300 10000 400000
```

```
cw4a 3 300 10000 600000

cw4a 1 300 10000 800000
```

Liczbę dostępnych rdzeni dla RPi można ustawić w pliku cmdline.txt na partycji boot w
katalogu user dodając parametr maxcpus=N, gdzie N to liczba dostępnych rdzeni.

## Zad. 4

To co nam wyjdzie w zad. 3 zrobić histogramy dla połowy czasu czasu przetwarzania

## Zad. 5

Aktywne oczekiwanie - trzeba będzie zrobić przez zmienne środowisko, aby nie modyfikować argumentów programu

TODO: Będzie trzeba ogarnąć co tam zrobić

## Zad. 6

Pewnie chodzi o różnice w czasach na minusie, ale to się jeszcze zobaczy

TODO: To też będzie trzeba ogarnąć

---

TODO: Przygotować sobie program do generowania histogramów
