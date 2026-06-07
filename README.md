# AcoustiQt 🎧

# Michał Sadecki

## Automatyka i Robotyka 2023

AcoustiQt to nowoczesna, wielowątkowa aplikacja desktopowa napisana w C++ z
wykorzystaniem frameworka Qt. Służy do analizy, przetwarzania i wizualizacji na
żywo sygnałów audio przesyłanych bezprzewodowo przez UDP z mikrokontrolerów (np.
ESP32). Projekt stworzony na potrzeby kursu o Wizualizacja danych sensorycznych.

## 📡 Architektura i Przepływ Danych

System został zaprojektowany do ścisłej współpracy z dedykowanym sprzętem. Proces akwizycji i przesyłu sygnału opiera się na następującej architekturze:

**Mikrofon INMP441** (I2S) ➔ **ESP32-S3-Dev-kit-nxrx** ➔ **Sieć Wi-Fi** (UDP) ➔ **AcoustiQt**

*   **Czujnik:** INMP441 – cyfrowy, wielokierunkowy mikrofon MEMS przesyłający precyzyjne dane cyfrowe za pomocą magistrali I2S.
*   **Mikrokontroler:** Płytka ESP32-S3-Dev-kit-nxrx – odpowiada za ciągłe próbkowanie sygnału audio, pakowanie go i bezprzewodową transmisję w czasie rzeczywistym.

## ✨ Główne funkcjonalności

*   **Wizualizacja w czasie rzeczywistym:** Płynne generowanie wykresów widma
częstotliwościowego oraz spektrogramu na podstawie szybkiej transformaty Fouriera (FFT).
*   **Analiza sygnału (DSP):** Ciągłe wyliczanie kluczowych statystyk audio:
    *   RMS (Root Mean Square)
    *   ZCR (Zero Crossing Rate) ze sprzętową bramką szumów
    *   Peak (Wartość szczytowa)
*   **Niezawodność (Watchdog sieciowy):** Wbudowany system monitorowania ciągłości pakietów UDP. Aplikacja automatycznie wykrywa utratę zasilania/sygnału z ESP32, czyści wykresy i powiadamia użytkownika.
*   **Integracja z InfluxDB:** Asynchroniczne przesyłanie wyliczonych statystyk do czasowej bazy danych InfluxDB za pomocą REST API.
*   **Personalizacja interfejsu:** 
    *   Dynamiczna zmiana języka (i18n) "w locie" (Polski / Angielski).
    *   Obsługa motywów graficznych: Jasny (Light), Ciemny (Dark) oraz Intensywny (Intense).
*   **Skalowalne osie:** Możliwość automatycznego dopasowania jednostek osi (Hz / kHz) do analizowanego pasma.

## 🛠️ Technologie

*   **Język:** C++17
*   **Framework UI & Sieciowy:** Qt 6 (Widgets, Network, Core, Linguist)
*   **System budowania:** CMake
*   **Baza danych:** InfluxDB

## Instalacja i uruchomienie

Instrukcja dotyczy kompilacji oprogramowania ze źródeł oraz instalacji w przestrzeni użytkownika (bez uprawnień administratora `sudo`). Taki sposób zapewnia poprawną integrację skrótu aplikacji oraz ikony SVG z menedżerami okien (X11/Wayland).

### Wymagania systemowe (Linux)

Przed rozpoczęciem upewnij się, że masz zainstalowany kompilator C++, narzędzie CMake oraz biblioteki deweloperskie Qt 6 (w tym moduł SVG).

*   **Arch Linux:**
```bash
    sudo pacman -S base-devel cmake qt6-base qt6-svg
    ```
*   **Ubuntu / Debian:**
```bash
    sudo apt install build-essential cmake qt6-base-dev qt6-svg-dev
    ```

## ⚙️ Konfiguracja przed uruchomieniem

Zanim skompilujesz i uruchomisz projekt, musisz skonfigurować poświadczenia sieciowe oraz dostęp do bazy danych.

### 1. Sieć Wi-Fi (ESP32)
Aby mikrokontroler połączył się z Twoją siecią, zmień nazwę pliku
`secrets.hh.example` na `secrets.hh`. Otwórz ten plik i wpisz w nim poprawną
nazwę swojej sieci (SSID), hasło oraz IP (standardowe IPv4).

### 2. Baza danych InfluxDB
Aplikacja przechowuje ustawienia połączenia z bazą w pliku konfiguracyjnym w katalogu domowym użytkownika. 
Utwórz plik konfiguracyjny pod ścieżką `~/.config/acoustiqt/acoustiqt.conf` i uzupełnij go poniższymi danymi:

```ini
[InfluxDB]
Url=[https://us-east-1-1.aws.cloud2.influxdata.com/api/v2/write?org=ORGANIZACJA&bucket=BUCKET&precision=ms](https://us-east-1-1.aws.cloud2.influxdata.com/api/v2/write?org=ORGANIZACJA&bucket=BUCKET&precision=ms)
Token=Token TUTAJ_TOKEN
```

Należy wprowadzić nazwę organizacji, token oraz nazwę bucketa. Jest to
zaznaczone WIELKIMI LITERAMI w przykładowej ścieżce.

### Kompilacja i instalacja

Wykonaj poniższe polecenia w terminalu, aby sklonować, zbudować i zainstalować aplikację:

```bash
git clone
[https://github.com/misadecki/acoustiqt.git](https://github.com/misadecki/acoustiqt.git)
cd acoustiqt

cmake -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build

cmake --install build --prefix ~/.local
```

## Odinstalowywanie

Wykonaj w katalogu projektu:

```bash
cd build
xargs rm < install_manifest.txt
```

## 📚 Dokumentacja kodu (Doxygen)

Projekt korzysta z systemu **Doxygen** do automatycznego generowania dokumentacji technicznej bezpośrednio z komentarzy w kodzie źródłowym.

### 1. Generowanie dokumentacji
Aby wygenerować lub zaktualizować dokumentację, otwórz terminal w głównym katalogu projektu i wykonaj polecenie:

```bash
doxygen docs/Doxyfile
```

### 2. Przeglądanie dokumentacji

Aby otworzyć i przeglądać pełną dokumentację, kliknij dwukrotnie w plik startowy
(w przypadku posiadania okienkowego systemu plików):

```
docs/html/index.html
```

Plik otworzy się w Twojej domyślnej przeglądarce internetowej.

W przypadku braku okienkowego systemu plików należy przejść do katalogu html i
wywołać

```bash
python -m http.server 8000
```

Potem należy w przeglądarce wpisać:
```
localhost:8000
```

lub

```
127.0.0.1:8000
```
