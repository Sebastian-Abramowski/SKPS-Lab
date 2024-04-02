# Raport z laboratorium 3 - 03.04.2024

### Sebastian Abramowski, 325142

### Bogumił Stoma, 325233

---

## Instalacja OpenWRT wersja 21.02.1

Instalacje systemu OpenWRT przeprowadziliśmy w identyczny sposób jak na laboratorium nr. 2

<img src="" width="500">

## Pierwszy pakiet

Zaczęcie pracy z SDK

```
git clone https://git.openwrt.org/openwrt/openwrt.git
```

---

Dodaliśmy do pliku feeds.conf.default ścieżkę do katalogu zawierającego katalogi z naszymi pakietami:

```
src-link skps /pelna/sciezka/demo1_owrt_pkg
```

Zaktualizowaliśmy listy pakietów

```
export LANG=C
./scripts/feeds update -a
./scripts/feeds install -p skps -a
```

Kompilacja paczek

```
make package/feeds/skps/demo1/compile
make package/feeds/skps/demo1mak/compile
```

Przenieślimy paczki w postaci plików .ipk na RPi i je zainstelowaliśmy

<img src="" width="500">

Wynik działania programu demo1

<img src="" width="500">

## Pakiety worms i buggy

Pobraliśmy katalogi z worms i buggy i umieściliśmy je w tym samym miejscu co poprzednio demo1 i demo1mak

W katalogach `buggy` i `worms` umieściilśmy pliki Makefile, analogiczne do tych wcześniejszych

Kompilacja paczek

```
export LANG=C
./scripts/feeds update -a
./scripts/feeds install -p skps -a
make package/feeds/skps/worms/compile
make package/feeds/skps/buggy/compile
```

Instalacja pakietów po ich przeniesieniu

```
opkg install worm.ipk
opkg install buggy.ipk
```

Działanie programów:

<img src="" width="500">

## Debugowanie zdalne

```
opkg update
opkg install gdb
opkg install gdbserver
```

Odpaliliśmy serwer gdb na RPi

```
gdbserver :9000 /usr/bin/bug1
```

Połącznie do serwera gdb z komputera hosta (analogicznie dla innych programów)

```
./scripts/remote-gdb adres_ip_naszego_RPI:9000 ./build_dir/target-aarch64_cortex-a72_musl/buggy-1.0/bug1
```

Ustawienie katalogu, w któym gdb ma szukać kodu:
np. directory ../demo1_owrt_pkg/buggy/src

<img src="" width="500">

### Program bug1

Na czym polegał bug1 - zapis do niezaalokowanej tablicy

Błąd:
<img src="" width="500">

### Program bug2

Na czym polegał bug2 - wyjście poza zakres tablicy

Błąd:
<img src="" width="500">

### Program bug3

Na czym polegał bug3 - wyjście poza zakres tablicy, brak Segmentation faulta w tym przypadku (nie wyszliśmy na tyle daleko, że na pewno trafimy na obszar do którego nie mamy uprawnień jak w przypadku programu bug2)

Błąd:
<img src="" width="500">

## Inne doświadczenia z gdb

Ustawienie breakpointu

Praca krokowa

Podgląd wartości zmiennej (print vs display)

Backtrace

Watchpointy w programie bug3, aby sprawdzić kiedy następuje zapisanie s1[10] pod niewłaściwym adresem
