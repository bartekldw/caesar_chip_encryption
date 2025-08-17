#pragma once
#include <iostream>
#include "../main/log/logWriter.hpp"
// Główna klasa do szyfrowania/deszyfrowania
class Encryption{
private:
    // klucz 
    std::string key;
    // obiekt logowania
    LogWriter& log;
public:
    Encryption(LogWriter& log);
    void set_key(const std::string& new_key){this->key = new_key;} // ustawia nowy klucz
    std::string xor_encrypt(const std::string& msg, bool decrypting = false); // główna funkcja do szyfrowania
    static std::string return_hex(const std::string& msg);
};