#pragma once
#include <iostream>
#include <string>
#include <fstream>
// Klasa zarządzająca systemem logów
class LogWriter{
public:
    // typy logów trzymane w enum class
    enum class log_type {INVALID, ERROR, WARNING, INFO, ASK, KEY};
private:
    std::fstream _txt_save_file;

    // pomocnicza funkcja dla obecnego czasu (czyste logi)
    std::string get_timestamp();
    // funkcje zwracające czyste formatowanie loga na podstawie jego typu:
    std::string return_log_type_con(log_type type); // dla konsoli
    std::string return_log_type_file(log_type type); // dla pliku

public:
    LogWriter();
    ~LogWriter();
    void write(log_type type, std::string_view log_msg, bool same_console_out = true, std::string_view con_msg = "");
};
