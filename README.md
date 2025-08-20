# Szyfrowanie Cezara (terminal, C++) [PL 🇵🇱]
### Sprawdź również mój podobny projekt przedstawiający <a href="https://github.com/bartekldw/xor_encryption">szyfrowanie XOR </a>

Prosty program pokazujący szyfrowanie Cezara w terminalu
Projekt obsługuje:
- Szyfrowanie i deszyfrowanie dowolnej wiadomości
- Generowanie losowych kluczy
- Zapis w .bin oraz .txt
- Kopiowanie klucza/zaszyfrowanego tekstu do schowka
- Ładne UI (hacker-vibes lata 90.)

## Zdjęcia
<div style="display:flex; gap:15px;">
  <img src="docs/menu.png" alt="Menu" width="200">
  <img src="docs/mode.png" alt="Tryb deszyfrowania" width="200">
  <img src="docs/encrypt_1.png" alt="Szyfrowanie" width="200">
</div>
<img src="docs/encrypt_2.png" alt="Szyfrowanie info" width="610">

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
./caesar_chiper
```
## Dodatkowe biblioteki
Projekt korzysta z dodatkowej biblioteki: <a href="https://github.com/nlohmann/json"> nlohmannjson </a>
## Uwagi:
### Program korzysta z unikatowego alfabetu:
```cpp
const std::u32string alph =U"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZąćęłńóśżźĄĆĘŁŃÓŚŻŹ";
```
Dlatego deszyfrowanie wiadomości szyfrowanej w tym programie z użyciem powyższego klucza z innym kluczem/w innym programie jest NIEMOŻLIWE
### Tryb animacji
Program domyślnie korzysta z animacji, jeżeli chcesz przyśpieszyć szyfrowanie/deszyfrowanie, usuń 24 linijke "add_compile_definitions(ANIMATION_MODE)" w CMakeLists.txt
## Licencja
Projekt udostępniany na podstawie licencji <a href="LICENSE"> MIT </a>

<p align="center">
  <a href="https://buymeacoffee.com/brtekld_prog" target="_blank">
    <img src="https://img.shields.io/badge/☕%20Pomóż%20mi%20rozwijać%20pasje-yellow?style=for-the-badge" alt="Buy Me A Coffee">
  </a>
</p>