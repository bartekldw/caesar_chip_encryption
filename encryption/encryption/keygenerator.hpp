#pragma once
#include <iostream>
#include <random>
#include "../main/log/logWriter.hpp"
// Klasa do zarządzania kluczami (generowanie unikatowych haseł-kluczy)
class Key{
private:   
    // obiekt do logowania
    LogWriter& log;
    // ustaw maszyne losującą
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> int_dist;
public:
    Key(LogWriter& log);
    int random_key();
    static int randint(int beg, int end);
};