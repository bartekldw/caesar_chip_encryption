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
// --- DEFINICJE METOD UŻYWANYCH PODCZAS SZYFROWANIA KLUCZA ---
// Funkcja pozwalająca wybrać tryb pobrania ciągu (plik, ciąg podany)
int Menu::get_message_mode(){
    int type = 0;
    error_state err = error_state::NONE; // na początku ustaw na domyślne - brak błędu
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄   ▖▖▄▖▄▖\n" << 
                                    "Wybierz tryb:" << " ▚▘▌▌▙▘\n" << ansi::reset << 
                                    "1. Plik .txt" << ansi::green << "  ▌▌▙▌▌▌\n" << ansi::reset << "2. Ciag znakow\n";
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

// Funkcja pobierająca ciąg znaków (lub klucz) z pliku
void Menu::get_message_from_file(std::string& msg){
    // przypadek dla pliku .txt
    error_state err = error_state::NONE;
    std::string new_path;
    bool error_read = false;
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄        ▖▖▄▖▄▖\n" << 
                                        "Wprowadź ścieżke:" << "  ▚▘▌▌▙▘\n" << ansi::reset << 
                                        "do pliku .txt/.bin" << ansi::green << " ▌▌▙▌▌▌\n" << ansi::reset;
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::red)+"Ścieżka nie może być pusta!\n");
                break;
            case (error_state::NUM):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::red)+"Nieznana ścieżka lub plik nie istnieje! Spróbuj ponownie\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for regular message path\n", false, std::string(ansi::green)+"Wprowadź ścieżke ");
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
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                    "Wprowadź c-" << "  ▚▘▌▌▙▘\n" << 
                                    "iąg" << ansi::reset << " znaków:" << ansi::green << "  ▌▌▙▌▌▌\n" << ansi::reset;
        // wyświetlanie błędów
        switch(err){
            case (error_state::CIN):
                log.write(LogWriter::log_type::WARNING, "User entered wrong data\n", false, std::string(ansi::red)+"Ciąg do zaszyfrowania nie może być pusty!\n");
                break;
        }
        // pobierz dane od użytkownika
        log.write(LogWriter::log_type::ASK, "Asking user for regular string to encrypt\n", false, std::string(ansi::green)+"Wprowadź ciąg: ");
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
void Menu::get_key_from_user(std::string& key, size_t msg_size){
    // ui
    ansi::clear_console();
    std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                "Wprowadź k-" << "  ▚▘▌▌▙▘\n" << 
                                "lucz" << ansi::reset << " (zostaw" << ansi::green << " ▌▌▙▌▌▌\n" << ansi::reset << "dla losowego, wpisz \"FILE\" jeżeli chcesz wczytać z pliku)\n";
    // pobierz dane od użytkownika
    log.write(LogWriter::log_type::ASK, "Asking user for key\n", false, std::string(ansi::green)+"Wprowadź klucz: ");
    std::getline(std::cin, key);

    // rozważ przypadki outputu
    if(key.empty()){ // jeżeli puste, wygeneruj
        int size = 0; 
        int next_key_index = file::get_key_count()+1;
        // ui
        ansi::clear_console();
        // generuj klucz (taka sama dlugosc jak wiadomosc - najmniej wykrywalne)
        key = keys_manager.random_key();
        ansi::clear_console();
        // poinformuj o nowym kluczu
        // ui
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                    "Twoj losowy" << "  ▚▘▌▌▙▘\n"
                                    "klucz: (" << next_key_index << ")"<< (next_key_index < 100 ? " " : "") << ansi::green << " ▌▌▙▌▌▌\n" << ansi::reset;
        // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
        log.write(LogWriter::log_type::KEY, "Succesfully created unique key\n", false, key); 
        std::cout << ansi::green << " (Dlugosc: " << size << ")" << ansi::reset << "\n";
        create_key_modules(key, next_key_index);
        // zakończ proces
        log.write(LogWriter::log_type::ASK, "Ending key generation\n", false, "Kliknij ENTER aby rozpoczac szyfrowanie... "); std::cin.get();
        return;
    }
    else if(key == "FILE" || key == "file"){ // jeżeli "FILE", pobierz z pliku
        get_message_from_file(key);
        ansi::clear_console();
        // poinformuj o nowym kluczu
        // ui
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                    "Twoj unika-" << "  ▚▘▌▌▙▘\n" <<
                                    "towy klucz:  ▌▌▙▌▌▌\n" << ansi::reset;
        // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
        log.write(LogWriter::log_type::KEY, "Succesfully imported key from file\n", false, key); 
        std::cout << ansi::green << " (Dlugosc: " << key.size() << ")" << ansi::reset << "\n";
        // zakończ proces
        log.write(LogWriter::log_type::ASK, "Ending key generation\n", false, "Kliknij ENTER aby rozpoczac szyfrowanie... "); std::cin.get();
        return;
    }
    // w przeciwnym wypadku, poprostu wyświetl klucz (klucz jest gotowy)
    ansi::clear_console();
    std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                    "Twoj unika-" << "  ▚▘▌▌▙▘\n" <<
                                    "towy klucz:  ▌▌▙▌▌▌\n" << ansi::reset;
    // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
    log.write(LogWriter::log_type::KEY, "Succesfully imported key from output\n", false, key); 
    std::cout << ansi::green << " (Dlugosc: " << key.size() << ")" << ansi::reset << "\n";
    // zakończ proces
    log.write(LogWriter::log_type::ASK, "Ending key generation\n", false, "Kliknij ENTER aby rozpoczac szyfrowanie... "); std::cin.get();
}

// Funkcja do urochomienia modułów po szyfrowaniu wiadomośći
void Menu::encrypt_modules(const std::string& msg, const std::string& key){
    std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄    ▖▖▄▖▄▖\n" << 
                                "Pomyślnie      ▚▘▌▌▙▘\n" <<
                                "zaszyfrowano!: ▌▌▙▌▌▌\n" << ansi::reset;
    // wyodrębnij zaszyfrowaną wiadomosc do HEX
    std::string hex_msg = Encryption::return_hex(msg);
    // usun niepotrzebne znaki nowej linii jako kopia (nadal zapisywane w plikach)
    std::string char_display_msg = msg;
    char_display_msg.erase(std::remove(char_display_msg.begin(), char_display_msg.end(), '\n'), char_display_msg.end());
    log.write(LogWriter::log_type::KEY, "Sucesfully encoded message with XOR\n", false, hex_msg);
    std::cout << std::string(ansi::gray) << " [" << ansi::green << "HEX" << ansi::gray << "]\n" << ansi::reset;
    log.write(LogWriter::log_type::KEY, "Displaying encoded message\n", false, char_display_msg);
    std::cout << std::string(ansi::gray) << " [" << ansi::green << "CHAR" << ansi::gray << "]\n" << ansi::reset;
    std::cout << ansi::green << "Użyto klucza:\n";
    log.write(LogWriter::log_type::KEY, "Displaying used key\n", false, key);
    std::cout << std::string(ansi::gray) << " [" << ansi::green << "CHAR" << ansi::gray << "]\n" << ansi::reset;
    // dodatkowe moduły zapisujące szyfr do plików
    encrypt_save_modules(msg, hex_msg);
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
    // zapisz do .bin
    file::save_bin(new_path_encrypted+"unique_key_"+std::to_string(encrypted_files_count)+".bin", bytes);
    log.write(LogWriter::log_type::INFO, "Saved char encrypted message to bin file ("+new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".bin)\n", false, "Zapisano znakowo do pliku binarnego ("+new_path_encrypted+"char_unique_key_"+std::to_string(encrypted_files_count)+".bin)"); std::cout << "\n";
    // zapis do .txt szesnastkowo
    file::save_txt(new_path_encrypted+"hex_unique_key_"+std::to_string(encrypted_files_count)+".txt", hex_msg);
    log.write(LogWriter::log_type::INFO, "Saved hex encrypted message to txt file ("+new_path_encrypted+"hex_unique_key_"+std::to_string(encrypted_files_count)+".txt)\n", false, "Zapisano szesnastkowo do pliku tekstowego ("+new_path_encrypted+"hex_unique_key_"+std::to_string(encrypted_files_count)+".txt)"); std::cout << "\n";
    // aktualizuj config json
    file::save_json("encryptedFilesCount",encrypted_files_count);
    log.write(LogWriter::log_type::INFO, "Updated json config state\n", false, "Zaktualizowano stan konfiguracji (config.json)"); std::cout << "\n";
    // zakończ moduł
    log.write(LogWriter::log_type::ASK, "Ending encrypting module\n", false, "Kliknij ENTER aby zakonczyc... ");
    std::cin.get();
}