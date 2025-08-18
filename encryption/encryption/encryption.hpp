#pragma once
#include <iostream>
#include "../main/log/logWriter.hpp"
// Główna klasa do szyfrowania/deszyfrowania
class Encryption{
private:
    // klucz 
    int key;
    // obiekt logowania
    LogWriter& log;
    // główny alfabet
    const std::u32string alph = U"AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŹŻaąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż";
public:
    Encryption(LogWriter& log);
    void set_key(int new_key){this->key = new_key;} // ustawia nowy klucz
    std::string ceaser_chip_encrypt(const std::string& msg); // główna funkcja do szyfrowania
    static std::string return_hex(const std::string& msg);
};