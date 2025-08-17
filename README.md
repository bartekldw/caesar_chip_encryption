# Szyfrowanie Cezara (terminal, C++) [PL 🇵🇱]

Prosty program pokazujący szyfrowanie Cezara w terminalu
Projekt obsługuje:
- Szyfrowanie i deszyfrowanie dowolnej wiadomości HEX/DEC (znakowo)
- Generowanie losowych kluczy
- Zapis w .bin oraz .txt
- Kopiowanie klucza/zaszyfrowanego tekstu do schowka
- Ładne UI (hacker-vibes lata 90.)

## Funkcje
- Szyfrowanie
- Deszyfrowanie
- Generowanie klucza  

## Kompilacja i uruchomienie
Projekt korzysta z CMake
### 1. Klonowanie repozytorium
```bash
git clone https://github.com/bartekldw/xor_encryption.git
cd xor_encryption
```
### 2. Tworzenie katalogu build
```bash
mkdir build
cd build
```
### 3. Konfiguracja i kompilacja
```bash
cmake ..
make
```
### 4. Uruchomienie programu
```bash
./xor_encrypt
```
## Dodatkowe biblioteki
Projekt korzysta z dodatkowej biblioteki: <a href="https://github.com/nlohmann/json"> nlohmannjson </a>
## Licencja
Projekt udostępniany na podstawie licencji <a href="LICENSE"> MIT </a>

<p align="center">
  <a href="https://buymeacoffee.com/brtekld_prog" target="_blank">
    <img src="https://img.shields.io/badge/☕%20Pomóż%20mi%20rozwijać%20pasje-yellow?style=for-the-badge" alt="Buy Me A Coffee">
  </a>
</p>