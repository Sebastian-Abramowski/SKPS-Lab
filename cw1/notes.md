## Notatki

### Komendy, które się mogą przydać
```sh
wget https://buildroot.org/downloads/buildroot-2021.08.tar.bz2
sudo apt-get install build-essential - jakby nie było do kompilacji
sudo apt-get install libncurses5-dev - przed odpaleniem menuconfig
```

Jeśli odznaczymy tworzenie tworzenie systemu plików w ext/2/3/4 tą opcje, to qemu trzeba będzie odpalać z komendy, który wygląda podobnie do tej:
qemu-system-aarch64 -M virt -cpu cortex-a57 -nographic -smp 4 -kernel output/images/Image -append "console=ttyAMA0"

Jeśli nie odznaczymy to można odpalać normalnie przez start-qemu.sh

---

Może będzie trzeba instalować qemu typowo pod jakąś architekture typu
```
sudo apt-get install qemu-system-arm
sudo apt-get install qemu-system-x86-64
```

Warto dodać nano przy konfiguracji

emulator QEMU lepiej zamykać przez 'poweroff' niż przez zamykanie terminala

## Kopiowanie pomiędzy urządzeniami w tej samej sieci
Stawiamy serwer znajdując sie w odpowiednim katalogu
```
python3 -m http.server
```

Przez port 8000 jest udostępniona zawartość foldera, aby z niego ściągać pliki
```
wget http://<adres_ip_hosta>:8000/<nazwa_pliku>
```
