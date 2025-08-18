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
// --- DEFINICJE METOD UŻYWANYCH PODCZAS DESZYFROWANIA KLUCZA ---

// Funkcja pozwalająca użytkownikowi wybrać tryb wiadomości do deszyfrowania (używa innej identycznej funkcji, lepsza czytelność)
int Menu::get_decoding_mode(){
    return get_message_mode();
}

// Funkcja zamieniająca wartość hex do dec
std::string Menu::hex_to_dec(const std::string& hex){
    std::string out;
    out.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
        out.push_back(byte);
    }
    return out;
}

// Funkcja uruchamiająca moduły do deszyfrowania
void Menu::decrypt_modules(const std::string& decrypted){
    std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄    ▖▖▄▖▄▖\n" << 
                                "Pomyślnie      ▚▘▌▌▙▘\n" <<
                                "odszyfrowano!: ▌▌▙▌▌▌\n" << ansi::reset;
    // usun niepotrzebne znaki nowej linii jako kopia (nadal zapisywane w plikach)
    log.write(LogWriter::log_type::KEY, "Sucesfully encoded message with XOR\n", false, decrypted);
    std::cout << std::string(ansi::gray) << " [" << ansi::green << "HEX" << ansi::gray << "]\n" << ansi::reset;
    // dodatkowe moduły zapisujące szyfr do plików
    decrypt_save_modules(decrypted);
}

// Dodatkowe moduły zapisu do odszyfrowania
void Menu::decrypt_save_modules(const std::string& decrypted){
    int decrypted_files_count = file::get_decrypted_count()+1;
    std::string new_path_decrypted = "../decrypted/decrypted_"+std::to_string(decrypted_files_count)+"/";
    // stwórz folder
    if (!std::filesystem::exists(new_path_decrypted)) { 
        std::filesystem::create_directories(new_path_decrypted);
    }
    // zapis do .txt 
    file::save_txt(new_path_decrypted+"hex_unique_key_"+std::to_string(decrypted_files_count)+".txt", decrypted);
    log.write(LogWriter::log_type::INFO, "Saved decrypted message to txt file ("+new_path_decrypted+"hex_unique_key_"+std::to_string(decrypted_files_count)+".txt)\n", false, "Zapisano do pliku tekstowego ("+new_path_decrypted+"hex_unique_key_"+std::to_string(decrypted_files_count)+".txt)"); std::cout << "\n";
    // aktualizuj config json
    file::save_json("decryptedFilesCount",decrypted_files_count);
    log.write(LogWriter::log_type::INFO, "Updated json config state\n", false, "Zaktualizowano stan konfiguracji (config.json)"); std::cout << "\n";
    // zakończ moduł
    log.write(LogWriter::log_type::ASK, "Ending encrypting module\n", false, "Kliknij ENTER aby zakonczyc... ");
    std::cin.get();
}

// Pobierz klucz od użytkownika do desyzfrowania wiadomości
void Menu::get_key_to_decode(int& key, const std::string& msg){
    std::string key_str;
    error_state err = error_state::NONE;
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                                "Wprowadź k-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                                "lucz (wpisz" << ansi::red << "  ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset << "\"FILE\" żeby pobrać z pliku)\n";
        // wyświetl stan błędu
        switch(err){
            case error_state::NUM:
                log.write(LogWriter::log_type::WARNING, "Imported wrong data. Trying again\n", false, std::string(ansi::blue)+"Klucz musi być liczbą!\n");
                break;
            case error_state::CIN:
                log.write(LogWriter::log_type::WARNING, "Imported wrong data. Trying again\n", false, std::string(ansi::blue)+"Klucz nie może być pusty!\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for key\n", false, std::string(ansi::red)+"Wprowadź klucz: ");
        std::getline(std::cin, key_str);
        if(key_str == "FILE" || key_str == "file"){
            try{
                get_message_from_file(key_str);
                key = std::stoi(key_str);
                break;
            } catch(...){
                err = error_state::NUM;
                continue;
            }
        }
        else if(key_str.size() >= 1){
            try{
                key = std::stoi(key_str);
                break;
            } catch(...){
                err = error_state::NUM;
                continue;
            }
        }
        else if(key_str.empty()){
            err = error_state::CIN;
        }
        else{
            break;
        }
    }
    ansi::clear_console();
    std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Twój unika-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << 
                              "towy" << ansi::reset << " klucz: " << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
    // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
    log.write(LogWriter::log_type::KEY, "Succesfully imported key\n", false, key_str+"\n"); 
    // zakończ proces
    log.write(LogWriter::log_type::ASK, "Ending key generation\n", false, "Kliknij ENTER aby rozpoczac deszyfrowanie... "); std::cin.get();
}