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

// Funkcja pozwalająca użytkownkowi wybrać tryb deszyfrowania (HEX/DEC)
int Menu::get_decoding_style_mode(){
    int type = 0;
    error_state err = error_state::NONE; // na początku ustaw na domyślne - brak błędu
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄   ▖▖▄▖▄▖\n" << 
                                    "Wybierz tryb:" << " ▚▘▌▌▙▘\n" << ansi::reset << 
                                    "1. HEX" << ansi::green << "        ▌▌▙▌▌▌\n" << ansi::reset << "2. Char (DEC)\n";
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::red)+"Błędne dane!\n");
                break;
            case (error_state::NUM):
                log.write(LogWriter::log_type::WARNING, "User entered wrong number\n", false, std::string(ansi::red)+"Wprowadź poprawną liczbe!\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for encrypt mode\n", false, std::string(ansi::green)+"Wybierz tryb: ");
        std::cin >> type;
        // sprawdź błędne wejście - w razie potrzeby, wejdź w stan błędu
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            err = error_state::CIN;
        }
        else if(type < 1 || type > 2){
            err = error_state::NUM;
        }
        else{ // gdy nie ma błędu wejścia, zakończ pętle
            err = error_state::NONE;
            break;
        }
    }
    // wyczyść bufor po korzystaniu z cin
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return type;
}

// Pobierz klucz od użytkownika do desyzfrowania wiadomości
void Menu::get_key_to_decode(std::string& key, const std::string& msg, bool hex_default){
    while(true){
        error_state err = error_state::NONE;
        // ui
        ansi::clear_console();
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                    "Wprowadź k-" << "  ▚▘▌▌▙▘\n" << 
                                    "lucz" << ansi::reset << " (wpisz " << ansi::green << " ▌▌▙▌▌▌\n" << ansi::reset << "\"FILE\" jeżeli chcesz wczytać z pliku)\n";
        switch(err){
            case error_state::CIN:
                log.write(LogWriter::log_type::WARNING, "User entered empty data\n", false, std::string(ansi::red)+"Klucz nie może być pusty!\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for key\n", false, std::string(ansi::green)+"Wprowadź klucz: ");
        std::getline(std::cin, key);

        // rozważ przypadki outputu
        if(key.empty()){ // jeżeli puste, wygeneruj
            err = error_state::CIN;
        }
        else if(key == "FILE" || key == "file"){ // jeżeli "FILE", pobierz z pliku
            get_message_from_file(key);
            break;
        }
        else{
            break;
        }
    }
    // poinformuj o kluczu
    // ui
    ansi::clear_console();
    std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄ ▖▖▄▖▄▖\n" << 
                                "Twoj uzyty" << "  ▚▘▌▌▙▘\n" <<
                                "klucz:      ▌▌▙▌▌▌\n" << ansi::reset;
    // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
    log.write(LogWriter::log_type::KEY, "Succesfully downloaded key\n", false, key); 
    std::cout << ansi::green << " (Dlugosc: " << key.size() << ")" << ansi::reset << "\n";
    std::cout << ansi::green << "Dla zaszyfrowanej wiadomości:\n";
    // dla wygodnego wyswietlenia: zamien wiadomosc na HEX (jeżeli nie użyto tego przez domniemanie)
    if(!hex_default){
        std::string hex = Encryption::return_hex(msg);
        log.write(LogWriter::log_type::KEY, "Displaying encrypted msg\n", false, hex);
    }
    else{
        log.write(LogWriter::log_type::KEY, "Displaying encrypted msg\n", false, msg);
    }
    std::cout << ansi::gray << " [" << ansi::green << "HEX" << ansi::gray << "]\n";
    // zakończ proces
    std::cout << ansi::green << "Deszyfrowanie gotowe!\n" << ansi::reset;
    log.write(LogWriter::log_type::ASK, "Ending key download\n", false, "Kliknij ENTER aby rozpoczac deszyfrowanie... "); std::cin.get();
}