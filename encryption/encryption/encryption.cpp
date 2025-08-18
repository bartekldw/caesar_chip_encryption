#include <iostream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <chrono>
#include "../main/log/logWriter.hpp"
#include "io/userio.hpp"
#include "encryption/encryption.hpp"
#include "../external/utfcpp/utf8.h"
// Główna funkcja do szyfrowania
std::string Encryption::ceaser_chip_encrypt(const std::string& msg){
    // krok 1: UTF-8 -> UTF-32
    std::u32string msg32;
    auto it = msg.begin();
    while (it != msg.end()) {
        char32_t cp = utf8::unchecked::next(it); // pobiera pełny znak
        msg32 += cp;
    }
    // krok 2: szyfrowanie
    std::u32string result32;
    int n = alph.size();
    for (char32_t element : msg32) {
        // krok 2.1: znajdz aktualny indeks
        auto pos = alph.find(element);
        if (pos == std::u32string::npos) {
            result32.push_back(element); // znak spoza alfabetu zostaje
            continue;
        }
        int new_index = ((pos + key) % n + n) % n;
        // krok 2.2: animacje:
        for(size_t i = 0; i < new_index; ++i){
            ansi::clear_console();
            con::print_encrypt_header(false);
            std::cout << con::u32str_to_utf8(result32);
            std::cout << ansi::red << con::u32char_to_utf8(alph[i]) << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3));

        }
        result32.push_back(alph[new_index]);
    }

    // krok 3: UTF-32 -> UTF-8
    std::string result;
    utf8::unchecked::utf32to8(result32.begin(), result32.end(), std::back_inserter(result));

    return result;
}
// Główna funkcja do deszyfrowania
std::string Encryption::ceaser_chip_decrypt(const std::string& msg){
    // krok 1: UTF-8 -> UTF-32
    std::u32string msg32;
    auto it = msg.begin();
    while (it != msg.end()) {
        char32_t cp = utf8::unchecked::next(it); // pobiera pełny znak
        msg32 += cp;
    }
    // krok 2: szyfrowanie
    std::u32string result32;
    int n = alph.size();
    for (char32_t element : msg32) {
        // krok 2.1: znajdz aktualny indeks
        auto pos = alph.find(element);
        if (pos == std::u32string::npos) {
            result32.push_back(element); // znak spoza alfabetu zostaje
            continue;
        }
        int new_index = ((pos - key) % n + n) % n;
        // krok 2.2: animacje:
        for(size_t i = 0; i < new_index; ++i){
            ansi::clear_console();
            con::print_encrypt_header(true);
            std::cout << con::u32str_to_utf8(result32);
            std::cout << ansi::red << con::u32char_to_utf8(alph[i]) << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3));

        }
        result32.push_back(alph[new_index]);
    }

    // krok 3: UTF-32 -> UTF-8
    std::string result;
    utf8::unchecked::utf32to8(result32.begin(), result32.end(), std::back_inserter(result));

    return result;
}
// Statyczna funkcja do zwracania bajtowej reprezentacji znaków
std::string Encryption::return_hex(const std::string& msg){
    std::ostringstream oss_hex;
    oss_hex << std::hex << std::setw(2) << std::setfill('0');
    for(const auto& element : msg){
        oss_hex << static_cast<int>(element);
    }
    return oss_hex.str();
}