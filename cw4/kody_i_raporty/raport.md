# Raport z laboratorium 4 - 24.04.2024

### Sebastian Abramowski, 325142

### Bogumił Stoma, 325233

---

## Przetestowanie działania programów na ''gospodarzu''

Zaktualizowaliśmy zmienną środowiskową PATH, zbudowaliśmy programy przez `make`, wykonaliśmy program `cw4a`, aby upewnić się, że działa

<img src="./screeny/Zrzut ekranu z 2024-04-24 13-04-50.png" width="500">

## Zbudowanie pakietu dla OpenWRT

Skompilowaliśmy pakiet przy pomocy SDK posługując się naszym raportem z poprzedniego laboratorium i przerzuciliśmy skompilowaną paczkę na RPi

Następnie zainstalowaliśmy nasz pakiet i uruchomiliśmy na RPi

<img src="./screeny/Zrzut ekranu z 2024-04-24 13-12-29.png" width="500">

## Przerzucanie plików

Przerzucanie plików pomiędzy hostem a RPi i odwrotnie było przeprowadzane przez serwer http używając polecenia `wget`

## Ustalenie granicznej wartości czasu przetwarzania

Naszym zadaniem w tym podpunkcie było zaobserwowanie, dla jakich wartości czasu przetwarzania danych (liczba pętli symulująca przetwarzenie danej wartości w programie `cw4b`), system przestaje nadążać przetwarzać dane w czasie rzeczywistym

Analizowaliśmy pliki `cli_*.txt`, aby zaobserować dla jakich wartości czasu przetwarzania, opóźnienia widoczne będą rosły i będą wynosiły sporo (w porównaniu do poprzednich wyników)

Standardowe wykonanie programu `cw4a`, z takimi argumentami testowaliśmy różne warianty:

```
cw4a 3 200 10000 <CZAS_PRZETWARZANIA, zaczynaliśmy od 100 000>
```

Liczba rdzeni była zmieniana poprzez zmianę pliku cmdline.txt (`maxcpus`) oraz reboot systemu

Pełne obciążenie powodowaliśmy komendą

```bash
stress-ng --matrix 0 -t 5m
```

Sprawdzenie obciążenia rdzeni sprawdzaliśmy poprzez program `htop`

### Wariant 1: 3 klientów, 1 rdzeń, pełne obciążenie

```bash
cw4a 3 200 10000 275000
# powyżej 200 000 są widoczne wzrosty opóźnień, a później jest ich coraz więcej
```

Sprawdzenie obciążenia

<img src="./screeny/Zrzut ekranu z 2024-04-24 13-19-33.png" width="500">

### Wariant 2: 3 klientów, 2 rdzenie, pełne obciążenie

```bash
cw4a 3 200 10000 450000
# 400 000 widać trochę, 450 000 widać duże opóźnienia
```

Sprawdzenie obciążenia

<img src="./screeny/Zrzut ekranu z 2024-04-24 13-38-50.png" width="500">

### Wariant 3: 3 klientów, 2 rdzenie, bez obnciążenia

```bash
cw4a 3 200 10000 550000
# przy 500 000 widać trochę opóźnień, 550 000 się zaczynają stałe opóxnianie
```

### Wariant 4L 1 klient, 4 rdzenie, bez obciążenia

```bash
cw4a 3 200 10000 810000
# przy 800 000 się zacznały większe opóźnia, dla 810 000 te opóźnienia były dużo większe
```

### Podsumowanie zadania nr. 3

| Numer wariantu | Graniczny czas przetwarzania |
| -------------- | ---------------------------- |
| 1              | 275 000                      |
| 2              | 450 000                      |
| 3              | 550 000                      |
| 4              | 810 000                      |

### Czy wyniki mają sens?

Wariant 3 działa bez obciążenia, więc jego wartość graniczna czasu przetwarzania powinna być większa od wariantu 2, co się zgadza.

Wariant 1 działa na jednym obciążonym rdzeniu, a wariant 2 na dwóch obciążonych rdzeniach, więc wariant 2 powinien mieć wartość graniczną większą (przetwarzanie danych może trwać dłużej bez drastycznego wzrostu opóźnień) niż wariant 1, co też się zgadza.

Wariant 4 ma najdłuższy graniczny czas przetwarzania, pewnie wynika to z faktu, że jeden klient nie ma konkurencji dostępu do zasobów dzielonych (bufora) w porównaniu do przypadku gdy mamy 3 klientów.

## Rozkład czasu dostarczenia danych

Czas przetwarzania w tym podpunkcie wynosi połowe tych czasów co nam wyszły w podpunkcie powyżej. Przedstawione histogramy będą przedstawiały zależność `opóźnienia dostarczenia danych [us]` od `liczby wystąpień`

### Wariant 1

```bash
cw4a 3 200 10000 137500
```

<img src="./rezultaty/lab4/wariancja 41/wariancja 41_hist.png" width="500">

### Wariant 2

```bash
cw4a 3 200 10000 225000
```

<img src="./rezultaty/lab4/wariancja 42/wariancja 42_hist.png" width="500">

### Wariant 3

```bash
cw4a 3 200 10000 275000
```

<img src="./rezultaty/lab4/wariancja 43/wariancja 43_hist.png" width="500">

### Wariant 4

```bash
cw4a 3 200 10000 405000
```

<img src="./rezultaty/lab4/wariancja 44/wariancja 44_hist.png" width="500">

## Aktywne oczekiwanie

Ustawiliśmy aby można było ustawić tryb programu `cw4b` przez ustawienie odpowieniej zmiennej środowiskowej w termialu przed uruchomieniem programu (nie chcieliśmy zmieniać argumentów, które program przyjmuje), zmienne środowiskowe:

- `ACTIVE_WAIT` - wszyscy kliencie będą oczekiwać aktywnie
- `ACTIVE_WAIT_CLIENT_NR_0` - klient nr. 0 będzie aktywnie oczekiwał

Jest to zrealizowane tak, że jeśli klient ma aktywnie oczekiwać, to nie zawiesza się na zmiennej warunkowej (`pthread_cond_wait(&rbuf->cvar,&rbuf->cvar_lock)`), ale kontynuuje swoje działanie, ponownie sprawdzając w pętli czy pojawiły się nowe dany

### Jak aktywne oczekiwanie wpłyneło na opóźnienie dostarczenia danych?

Badanie przeprowadziliśmy dla wariantu 1 z zadania nr. 4 (tego powyżej) - 3 klientów, 1 rdzeń, pełne obciążenie. Przypomnienie histogramu bez aktywnego oczekiwania:

<img src="./rezultaty/lab4/wariancja 41/wariancja 41_hist.png" width="500">

### Aktywne oczekiwanie klient nr. 0

<img src="./rezultaty/lab4/zadanie 51/zadanie 51_hist.png" width="500">

### Aktywne oczekiwanie wszystkich klientów

<img src="./rezultaty/lab4/zadanie 52/zadanie 52_hist.png" width="500">

### Obserwacje po zakończeniu zadania

Możemy zaobserwować, że czasy opóźnień dostarczania danych bez aktywnego oczekiwania były znacząco mniejsze. Na dodatek, wtedy mogliśmy zaobserować w regularność i przewidywalność w czasach opóźnień.

Dla aktywnego oczekiwania klienta nr. 0, możemy zaobserwować, że opóźnienia tego klienta są znacznie większe (na oko w 1/3 przypadków) niż bez. Na dodatek, klienci nr. 1 i nr. 2 mają zwiększone czasy opóźnień (pewnie wynika to z faktu, że proces klienta nr. 0 bez sensu wykorzystuje czas procesora).

W aktywnym oczekiwaniu wszystkich klientów także możemy zobaczyć powyższy obserwacje. Mniejsza regularność, większe czasy opóźnień dla wszystkich klientów. Co ciekawe, w mniej więcej dwóch piątych przypadków, klient nr. 0 ma mniejsze opóźnienie przetwarzania niż w przypadku bez aktywnego oczekiwania (chodzi o niebieski pasek na wykresie z tytułem `zadanie 52` dla wartości czasu mniej więcej 90 us)

## Właściwy pomiar czasu

Badania w tym zadaniu przeprowadziliśmy dla wariantu 3 z zadania nr. 3 (`cw4a 3 200 10000 550000`)

Problem w programie `cw4a` polegał na tym, że w każdym wykonaniu pętli, było spanie w programie o okres równy czasu próbkowania. Czyli np. jeślibyśmy mieli okres próbkowania 1s, to czas pomiędzy wygenerowaniem kolejnych dwóch próbek wynosiłby 1s + czas przetwarzania próbki danych. Trzeba było zastosować korektę okresu próbkowania w pętli, abyśmy usypiali na tyle ile to jest konieczne.

Czyli np. jeśli okres próbkowania wynosi 1s oraz przetwarzamy przez 0.5s daną próbkę danych (u nas jest to aktualny czas), to w następnym kroku pętli, będziemy spali 0.5s zamiast 1s. W ten sposób uzyskamy program, w którym okres między pobraniami zestawów próbek jest mniej więcej właściwy.

### Wariant 3 z zadania nr. 3 przed poprawieniem programu

Wykresy przedstawiają czas pomiędzy pobraniami zestawów próbek w mikrosekundach oraz ile razy te czasy wystąpiły.

<img src="./rezultaty/lab4/wariancja 33/zad6_hist.png" width="500">

### Wariant 3 z zadania nr. 3 przed poprawieniem programu

W tym przypadku widzimy, że czas jest dużo bliższy okresowi próbowania, który wnosił `10000` us w naszym przypadku.

<img src="./rezultaty/lab4/zadanie 6/zad6_hist.png" width="500">
