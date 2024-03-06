# Raport z laboratorium 1 - 6.03.2024

### Sebastian Abramowski, 325142

### Bogumił Stoma, 325233

---

Raport wygenerowany automatycznie z pliku raport.md

## Przygotowanie stanowiska

Zrobiliśmy według instrukcji

<img src="obrazy/stanowisko3.png" width="500">

## Pierwsze uruchomienie RPi

Podłączyliśmy się do terminala UART do RPi przez program tio

<img src="obrazy/uart_podlaczenie.png" width="500">

Potem uruchomiliśmy DHCP, aby RPi dostało adres IP

<img src="obrazy/dhcp_uruchomienie.png" width="500">

Następnie odczytaliśmy IP RPi i hosta oraz sprawdziliśmy czy pingowanie działa w obie strony

Adres RPi: 10.42.0.63 </br>
Adres hosta: 10.42.0.1

Na RPi

<img src="obrazy/ip_malina.png" width="500">

</br>

Na host

<img src="obrazy/ip_host.png" width="500">

## Kopiowanie plików na RPi

Korzystaliśmy do przesyłania plików z metody z serwerem HTTP, na którym mieliśmy zawartość katalogu o nazwie 'test'

Tak utworzyliśmy serwer:

<img src="obrazy/serwer_http_utworzenie.png" width="500">

Sprawdziliśmy jego działanie przez pojedynczy plik 'test.txt', który znajdował się w katalogu 'test'

<img src="obrazy/serwer_http_dziala.png" width="500">

## Kompilacja obrazu Linuxa w Buildroot

> Korzystaliśmy z wersji Buildroot 2023.11.1, pobraliśmy ją przez 'wget https://buildroot.org/downloads/buildroot-2023.11.1.tar.xz'

### Obraz dla RPi z initramfs

Najpierw wykonaliśmy komendy:

```
make raspberrypi4_64_defconfig
make menuconfig
```

Ustawiliśmy Toolchain na External zgodnie z instrukcją oraz
w ustawieniach 'Filesystem images' dokonaliśmy następujących zmian

<img src="obrazy/menuconfig_z_initramfs.png" width="500">

Następnie zbudowaliśmy obraz poleceniem

```
make
```

## Uruchomienie zbudowanego obrazu

Potem skopiowaliśmy potrzebne nam pliki do zbudowania obrazu do naszego folderu 'test', którego zawartość jest dostępna przez serwer HTTP

<img src="obrazy/kopiowanie_do_serwera_1.png" width="500">

Sprawdziliśmy czy wszystko pliki są udostępnione

<img src="obrazy/serwer_http_dziala2.png" width="500">

Następnie zamontowaliśmy partycję 1 karty SD w katalogu /mnt i sprawdziliśmy jego zawartość

<img src="obrazy/monotwanie_partycji_malina.png" width="500">

Pobraliśmy 3 wcześniej wrzucone na serwer pliki do RPi oraz zmieniliśmy nazwę obrazu z Image na kernel8.img

<img src="obrazy/wgety_ramdysk.png" width="500">

Następnie uruchomiliśmy zbudowany system (trzymając przycisk SW4 zgodnie z instrukcją) oraz zapisaliśmy w nim jeden plik testowy

<img src="obrazy/ramdysk_odpalenie2.png" width="500">

Po restarcie systemu, pliku już nie było, co wskazywało na to, że system prawidłowo działał

<img src="obrazy/ramdysk_po_reboot.png" width="500">

### Obraz dla RPi bez initramfs

Wykonaliśmy komende w celu czyszenia po poprzednim obrazie według instrukcji i odpaliliśmy menu do konfiguracji

```
make linux-dirclean
make menuconfig
```

Zostawiliśmy Toolchain ustawiony na External, wyłączyliśmy initramfs,włączyliśmy wsparcie dla ext2/3/4 (ext2 wybraliśmy) oraz zaznaczyliśmy ustawienie kompresji na gzip

Dodatkowo dwukrotnie zwiększyliśmy wielkość systemu plików

<img src="obrazy/menuconfig.png" width="500">

Wykonaliśmy kompilację systemu przez

```
make clean all
```

Ponownie przerzuciliśmy wymagane pliki do udostępnionego folder (tym razem także plik z systemem plików)

<img src="obrazy/kopiowanie_plikow_potrzebnych_bez_ramdysk.png" width="500">

Sprawdziliśmy zawartość udostępnionego folderu

<img src="obrazy/serwer_wszystko_ok.png" width="500">

Wrzuciliśmy plik 'rootfs.ext2' na partycję drugą karty SD

<img src="obrazy/rootfs2222.png" width="500">

Zamontowaliśmy pierwszą partycję karty SD jak poprzednio i dodaliśmy do niej wymagane pliki (pamiętając o tym żeby zmienić nazwę obrazu z Image na kernel8.img)

<img src="obrazy/mount_zwykly_big.png" width="500">

Przetestowaliśmy działanie systemu z normalnym systemem plików poprzez zapiasanie w nim pliku testowego

<img src="obrazy/normalny_test.png" width="500">

Po restarcie systemu, plik oraz jego zawartość były dostępne, więc system bez initramfs działał prawidłowo

<img src="obrazy/normalny_plik_po_restarcie.png" width="500">

## Porównanie wielkości obrazów

Rozmiar obrazu systemu z initramfs

<img src="obrazy/rozmiar_ramdysk2.png" width="500">

Rozmiar obrazu systemu ze zwykłym systemem plików

<img src="obrazy/rozmiar_bez_ramdysku.png" width="500">

Czyli rozmiar obrazu systemu z initramfs był ponad dwa razy większy. Wynika to z tego, że startowy system plików jest wrzucony do środka obrazu systemu z initramfs. W systemie ze zwykłym systemem plików, system plików zajmuje miejsce na karcie SD.
