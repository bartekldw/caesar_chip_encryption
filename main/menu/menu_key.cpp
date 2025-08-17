#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "../main/menu/menu.hpp" // odpowiadający nagłówek menu.hpp
#include "../main/log/logWriter.hpp" // dodatkowe nagłówki (system logowania)
#include "io/file.hpp"
#include "io/userio.hpp"
#include "io/file.hpp"
// --- DEFINICJE METOD UŻYWANYCH PODCZAS GENEROWANIA KLUCZA ---
// Funkcja pobierająca klucz od użytkownika (ewentualnie losująca)
void Menu::get_key_from_user(int& len, std::string& str_len){
    error_state err = error_state::NONE;
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                "Podaj poza-" << "  ▚▘▌▌▙▘\n" <<
                                "dana dlugosc" << ansi::green << " ▌▌▙▌▌▌\n" << ansi::reset;
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, "Podaj prawidlowe dane!\n");
                break;
            case (error_state::NUM):
                log.write(LogWriter::log_type::WARNING, "User entered wrong number\n", false, "Podaj prawidlowa liczbe (1-512)!\n");
                break;
            default:
                break;
        }
        // pobierzdane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for desired key size\n", false, "Podaj dlugosc klucza (zostaw puste dla losowego):");
        std::getline(std::cin, str_len);
        // sprawdź błędne wejście - w razie potrzeby, wejdź w stan błędu
        if(str_len.empty()){
            break;
        }
        try{
            len = std::stoi(str_len);
        } catch(...){
            err = error_state::CIN;
        }
        if(len <= 0 || len > 512){
            err = error_state::NUM;
        }
        else{  // gdy nie ma błędu wejścia, zakończ pętle
            break;
        }
    }
}

// Moduły tworzenia klucza: skopiuj do schowka, stwórz plik .bin oraz .txt
void Menu::create_key_modules(const std::string& key, int key_files_count){
    std::string new_path = "../saved/key_"+std::to_string(key_files_count)+"/";
    // stwórz folder
    if (!std::filesystem::exists(new_path)) { 
        std::filesystem::create_directories(new_path);
    }
    std::vector<unsigned char> bytes(key.begin(), key.end());
    // skopiuj do schowka
    int err_st = 0;
    con::copyToClipboard(log, key,err_st);
    if(err_st != 1){log.write(LogWriter::log_type::INFO, "Copied key to clipboard\n", false, "Skopiowano klucz do schowka"); std::cout << "\n";}
    // zapisz do .bin
    file::save_bin(new_path+"unique_key_"+std::to_string(file::get_key_count())+".bin", bytes);
    log.write(LogWriter::log_type::INFO, "Saved key to bin file ("+new_path+"unique_key_"+std::to_string(key_files_count)+".bin)\n", false, "Zapisano klucz do pliku binarnego ("+new_path+"unique_key_"+std::to_string(key_files_count)+".bin)"); std::cout << "\n";
    // aktualizuj config json
    file::save_json("keyTextFilesCount",(file::get_key_count()+1));
    log.write(LogWriter::log_type::INFO, "Updated json config state\n", false, "Zaktualizowano stan konfiguracji (config.json)"); std::cout << "\n";
}