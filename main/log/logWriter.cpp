#include <iostream>
#include <string>
#include <ctime> // dla daty logów
#include <cstdlib> // dla krytycznego wyjśćia
#include <filesystem> // dla zarządzania folderem logs
#include "../main/log/logWriter.hpp"
#include "io/userio.hpp"

// Główny konstruktor LogWriter
LogWriter::LogWriter(){
    // tutaj, używając biblioteki filesystem
    // sprawdź czy folder "logs" istnieje, jeżeli nie - utwórz
    if (!std::filesystem::exists("../logs")) { 
        std::filesystem::create_directories("../logs");
    }
    // tutaj, wywołujemy funkcje get_timestamp() aby otrzymać czyste logi (z datą) 
    std::string name = "Logs (" + get_timestamp() + ")";
    std::string path = "../logs/" + name + ".log";

    // teraz, otwieramy nasz plik
    _txt_save_file.open(path, std::ios::out | std::ios::app);
    if(!_txt_save_file.is_open()){
        std::cerr << "[FATAL ERROR] Błąd podczas otwierania pliku logów! Sprawdź swój działający folder i spróbuj ponownie..." << std::endl; // jeżeli plik nie chce sie otworzyć, przejdź do krytycznego zamknięcia programu
        std::exit(1);
    }
}

// Destruktor aby pomyślnie zamknąć plik log
LogWriter::~LogWriter(){ 
    if (_txt_save_file.is_open()){ 
        write(log_type::INFO, "Ended LOG module succesfully!\n");
        _txt_save_file.close();
    }
}

// Funkcja która zwraca czas w przyjaznym formatowaniu za pomocą std::strftime
std::string LogWriter::get_timestamp(){
    std::time_t t = std::time(nullptr);
    std::tm now{};
    #ifdef _WIN32
        localtime_s(&now, &t); // dla Windowsa
    #else
        localtime_r(&t, &now); // dla Linux/macOS/Unix
    #endif
    char buf[32]; // przykładowy bufor daty - nie powinnen przekraczać 32 znaków (NIEBEZPIECZNE)
    std::strftime(buf, sizeof(buf), "%Y-%m-%d, %H:%M:%S", &now);
    return std::string(buf);
}
void LogWriter::write(log_type type, std::string_view log_msg, bool same_console_out, std::string_view con_msg){
    std::string add_string;
    // sprawdź czy tekst ma być taki sam w logu jak w wyjściu
    if(same_console_out){
        add_string.assign(log_msg);
    }
    // zapisz log do pliku
    std::string log_msg_out = "[" + get_timestamp() + "] " + return_log_type_file(type) + std::string(log_msg);
    _txt_save_file << log_msg_out;
    
    // wypisz log na ekran
    std::string log_msg_cout = return_log_type_con(type) + std::string(same_console_out ? add_string :con_msg);
    std::cout << log_msg_cout;
}

// --- Dodatkowe funkcje ---
// Zwróć formatowanie loga do konsoli
std::string LogWriter::return_log_type_con(log_type type) {
    switch (type) {
        case log_type::ASK:
            return std::string(ansi::gray) + "[" + ansi::green + "?" + ansi::gray + "] " + ansi::reset;
        case log_type::ERROR:
            return std::string(ansi::gray) + "[" + ansi::red + "!" + ansi::gray + "] " + ansi::reset;
        case log_type::INFO:
            return std::string(ansi::gray) + "[" + ansi::blue + "!" + ansi::gray + "] " + ansi::reset;
        case log_type::WARNING:
            return std::string(ansi::gray) + "[" + ansi::light_yellow + "!" + ansi::gray + "] " + ansi::reset;
        case log_type::INVALID:
            return std::string(ansi::gray) + "[" + ansi::gray + "?" + ansi::gray + "] " + ansi::reset;
        case log_type::KEY:
            return std::string(ansi::gray) + "[" + ansi::green+ "KEY" + ansi::gray + "] " + ansi::reset;
        default:
            return std::string(ansi::gray) + "[" + ansi::gray + "?" + ansi::gray + "] " + ansi::reset;
    }
}


// Zwróc formatowanie loga do pliku log
std::string LogWriter::return_log_type_file(log_type type){
     switch (type) {
        case log_type::ASK:
            return "[ASK] ";
        case log_type::ERROR:
            return "[ERROR] ";
        case log_type::INFO:
            return "[INFO] ";
        case log_type::WARNING:
            return "[WARNING] ";
        case log_type::INVALID:
            return "[INVALID] ";
        case log_type::KEY:
            return "[KEY] ";
        default:
            return "[UNKNOWN] ";
    }
}