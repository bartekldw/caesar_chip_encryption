#pragma once
#include <iostream>
// Główna klasa do szyfrowania/deszyfrowania
class Encryption{
private:
    // klucz 
    int key;
    // główny alfabet
    const std::u32string alph =
    U"abcdefghijklmnopqrstuvwxyz"
    U"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    U"ąćęłńóśżźĄĆĘŁŃÓŚŻŹ";
public:
    void set_key(int new_key){this->key = new_key;} // ustawia nowy klucz
    std::string ceaser_chip_encrypt(const std::string& msg); // główna funkcja do szyfrowania
    std::string ceaser_chip_decrypt(const std::string& msg);
    static std::string return_hex(const std::string& msg);
};