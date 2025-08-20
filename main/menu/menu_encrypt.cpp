#include <iostream>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "../main/menu/menu.hpp" // odpowiadający nagłówek menu.hpp
#include "../main/log/logWriter.hpp" // dodatkowe nagłówki (system logowania)
#include "io/file.hpp"
#include "io/userio.hpp"
#include "io/file.hpp"
// --- DEFINICJE METOD UŻYWANYCH PODCZAS SZYFROWANIA KLUCZA ---
// Funkcja pozwalająca wybrać tryb pobrania ciągu (plik, ciąg podany)
int Menu::get_message_mode(){
    int type = 0;
    error_state err = error_state::NONE; // na początku ustaw na domyślne - brak błędu
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄       ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Wybierz tryb:" << "     ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                              "1. Plik .bin/.txt" << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset << "2. Ciąg znaków\n";
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::blue)+"Błędne dane!\n");
                break;
            case (error_state::NUM):
                log.write(LogWriter::log_type::WARNING, "User entered wrong number\n", false, std::string(ansi::blue)+"Wprowadź poprawną liczbe!\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for encrypt mode\n", false, std::string(ansi::red)+"Wybierz tryb: ");
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

// Funkcja pobierająca ciąg znaków (lub klucz) z pliku
void Menu::get_message_from_file(std::string& msg){
    // przypadek dla pliku .txt
    error_state err = error_state::NONE;
    std::string new_path;
    bool error_read = false;
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄        ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Wprowadź ścieżke" << "   ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                              "do pliku .txt/.bin" << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::blue)+"Ścieżka nie może być pusta!\n");
                break;
            case (error_state::NUM):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::blue)+"Nieznana ścieżka lub plik nie istnieje! Spróbuj ponownie\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for regular message path\n", false, std::string(ansi::red)+"Wprowadź ścieżke ");
        std::getline(std::cin, new_path);
        // sprawdź błędne wejście - w razie potrzeby, wejdź w stan błędu
        msg = file::read_text(new_path, error_read);
        if(new_path.empty()){
            err = error_state::CIN;
        }
        else if(error_read){
            err = error_state::NUM;
            continue;
        }
        else{ // gdy nie ma błędu wejścia, zakończ pętle
            err = error_state::NONE;
            break;
        }
    }
}

// Funkcja pobierająca ciąg znaków od użytkownika
void Menu::get_message_from_output(std::string& msg){
    // przypadek dla podanego ciągu znaków
    error_state err = error_state::NONE;
    while(true){
        // ui
        ansi::clear_console();
       std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                                "Wprowadź c-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                                "iąg znaków:" << ansi::red << "  ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::blue)+"Ciąg do zaszyfrowania nie może być pusty!\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for regular string to encrypt\n", false, std::string(ansi::red)+"Wprowadź ciąg: ");
        std::getline(std::cin, msg);
        // sprawdź błędne wejście - w razie potrzeby, wejdź w stan błędu
        if(msg.empty()){
            err = error_state::CIN;
        }
        else{ // gdy nie ma błędu wejścia, zakończ pętle
            err = error_state::NONE;
            break;
        }
    }
}
// Funkcja pobierania klucza od użytkownika (ewentualnie generowania go)
void Menu::get_key_from_user(int& key){
    std::string key_str;
    error_state err = error_state::NONE;
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                                "Wprowadź k-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                                "lucz (zost-" << ansi::red << "  ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset << "aw dla losowego, wpisz \"FILE\" żeby pobrać z pliku)\n";
        // wyświetl stan błędu
        switch(err){
            case error_state::NUM:
                log.write(LogWriter::log_type::WARNING, "Imported wrong data. Trying again\n", false, std::string(ansi::blue)+"Klucz musi być liczbą!\n");
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
        else{
            break;
        }
    }
    // rozważ przypadki outputu
    if(key_str.empty()){ // jeżeli puste, wygeneruj
        int size = 0; 
        int next_key_index = file::get_key_count()+1;
        // ui
        ansi::clear_console();
        // generuj klucz (taka sama dlugosc jak wiadomosc - najmniej wykrywalne)
        key = keys_manager.random_key();
        ansi::clear_console();
        // poinformuj o nowym kluczu
        // ui
        std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Twój unika-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << 
                              "towy" << ansi::reset << " klucz: " << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
        // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
        log.write(LogWriter::log_type::KEY, "Succesfully created unique key\n", false, std::to_string(key)+"\n"); 
        create_key_modules(std::to_string(key), next_key_index);
        // zakończ proces
        log.write(LogWriter::log_type::ASK, "Ending key generation\n", false, "Kliknij ENTER aby rozpoczac szyfrowanie... "); std::cin.get();
        return;
    }
    // w przeciwnym wypadku, poprostu wyświetl klucz (klucz jest gotowy)
    ansi::clear_console();
    std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Twój unika-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << 
                              "towy" << ansi::reset << " klucz: " << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
    // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
    log.write(LogWriter::log_type::KEY, "Succesfully imported key\n", false, key_str+"\n"); 
    // zakończ proces
    log.write(LogWriter::log_type::ASK, "Ending key generation\n", false, "Kliknij ENTER aby rozpoczac szyfrowanie... "); std::cin.get();
}

// Funkcja do urochomienia modułów po szyfrowaniu wiadomośći
void Menu::encrypt_modules(const std::string& msg, const std::string& key){
    ansi::clear_console();
    std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄   ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Pomyślnie" << "     ▌ ▌▌▙▖▚ ▌▌▙▘\n" << 
                              "zaszyfrowano!" << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
    log.write(LogWriter::log_type::KEY, "Sucesfully encoded message with Caesar's chip\n", false, msg);
    std::cout << std::string(ansi::gray) << " [" << ansi::red << "CHAR" << ansi::gray << "]\n" << ansi::reset;
    std::cout << ansi::red << "Użyto klucza:\n";
    log.write(LogWriter::log_type::KEY, "Displaying used key\n", false, key);
    std::cout << std::string(ansi::gray) << " [" << ansi::red << "CHAR" << ansi::gray << "]\n" << ansi::reset;
    // dodatkowe moduły zapisujące szyfr do plików
    encrypt_save_modules(msg, msg);
}
// Funkcja do uruchomienia modułów zapisowych po szyfrowaniu
void Menu::encrypt_save_modules(const std::string& msg, const std::string& hex_msg){
    int encrypted_files_count = file::get_encrypted_count()+1;
    std::string new_path_encrypted = "../encrypted/encrypted_"+std::to_string(encrypted_files_count)+"/";
    // stwórz folder
    if (!std::filesystem::exists(new_path_encrypted)) { 
        std::filesystem::create_directories(new_path_encrypted);
    }
    std::vector<unsigned char> bytes(msg.begin(), msg.end());
    // skopiuj do schowka
    int err = 0;
    con::copy_to_clipboard(log, msg, err);
    if(err != 1){log.write(LogWriter::log_type::INFO, "Copied key to clipboard\n", false, "Skopiowano zaszyfrowaną wiadomość do schowka"); std::cout << "\n";}
    // zapisz do .bin
    file::save_bin(new_path_encrypted+"unique_key_"+std::to_string(encrypted_files_count)+".bin", bytes);
    log.write(LogWriter::log_type::INFO, "Saved char encrypted message to bin file ("+new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".bin)\n", false, "Zapisano znakowo do pliku binarnego ("+new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".bin)"); std::cout << "\n";
    // zapis do .txt
    file::save_txt(new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".txt", hex_msg);
    log.write(LogWriter::log_type::INFO, "Saved char encrypted message to txt file ("+new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".txt)\n", false, "Zapisano znakowo do pliku tekstowego ("+new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".txt)"); std::cout << "\n";
    // aktualizuj config json
    file::save_json("encryptedFilesCount",encrypted_files_count);
    log.write(LogWriter::log_type::INFO, "Updated json config state\n", false, "Zaktualizowano stan konfiguracji (config.json)"); std::cout << "\n";
    // zakończ moduł
    log.write(LogWriter::log_type::ASK, "Ending encrypting module\n", false, "Kliknij ENTER aby zakonczyc... ");
    std::cin.get();
}