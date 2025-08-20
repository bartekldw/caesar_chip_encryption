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
// Główny konstruktor klasy Menu - inicjalizuje klase log, klase kluczy oraz klase encryption, konstruuje system plików
Menu::Menu() : log(), keys_manager(log){
    log.write(LogWriter::log_type::INFO, "Log module loaded\n");
    file::init();
    log.write(LogWriter::log_type::INFO, "File system init succesfull\n");
}
// Główna funkcja do wyświetlania menu
void Menu::print(){
    // przygotuj zmienne
    int operation = 0;
    error_state err = error_state::NONE; // na początku ustaw na domyślne - brak błędu
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                                "Szyfrowanie" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                                "1. Szyfruj" << ansi::red << "   ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
        std::cout << "2. Deszyfruj\n3. Wygeneruj klucz\n";
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
        log.write(LogWriter::log_type::ASK, "Asking user for menu operation\n", false, std::string(ansi::red)+"Wybierz operacje: ");
        std::cin >> operation;
        // sprawdź błędne wejście - w razie potrzeby, wejdź w stan błędu
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            err = error_state::CIN;
        }
        else if(operation < 1 || operation > 3){
            err = error_state::NUM;
        }
        else{ // gdy nie ma błędu wejścia, zakończ pętle
            break;
        }
    }

    // wywołaj funkcje odpawiadającą do wyboru
    switch(operation){
        case 1:
            log.write(LogWriter::log_type::INFO, "User is encrypting message\n");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            encrypt_key();
            break;
        case 2:
            log.write(LogWriter::log_type::INFO, "User is decrypting message\n");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            decrypt_key();
            break;
        case 3:
            log.write(LogWriter::log_type::INFO, "User is generating random key\n");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            random_key();
            break;
    }
}

// Funkcja do losowania klucza i wyświetlania go 
void Menu::random_key(){
    int key = keys_manager.random_key();
    // ui
    ansi::clear_console();
    // ustaw potrzebne zmienne dla prawidłowego zapisania danych (pobiera ilość dotychczas zapisanych plików klucza, aby wygenerować unikatowy folder i nazwe)
    int next_key_index = file::get_key_count()+1;
    // ui
    std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄  ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                              "Twój unika-" << "  ▌ ▌▌▙▖▚ ▌▌▙▘\n" << 
                              "towy" << ansi::reset << " klucz: " << ansi::red << " ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
    // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
    log.write(LogWriter::log_type::KEY, "Succesfully created unique key\n", false, std::to_string(key)+"\n"); 
    // wywołuj moduły tworzenia klucza (zobacz definicje funkcji)
    create_key_modules(std::to_string(key), next_key_index);
}

// Funkcja do szyfrowania wiadomosci i wyswietlania zaszysfrowanego ciągu
void Menu::encrypt_key(){
    int type = get_message_mode(); // type: 1-szyfrowanie pliku .txt, 2-szyfrowanie ciagu znakow 
    std::string msg;
    int key;
    // pobierz ciąg
    switch(type){
        case 1: // szyfrowanie z pliku .txt
            get_message_from_file(msg); break;
        case 2: // szyfrowanie z ciągu znaków
            get_message_from_output(msg); break;
    }
    std::cin.get();
    // pobierz klucz i ustaw go w klasie szyfrowania
    get_key_from_user(key);
    encryption.set_key(key);
    std::cin.get();
    // rozpocznij generowanie
    std::string encrypted = encryption.ceaser_chip_encrypt(msg, animation);
    // wywołaj moduły po wygenerowaniu klucza
    encrypt_modules(encrypted, std::to_string(key));
}
// Funkcja do deszyfrowania wiadomości i wyświetlania deszyfrowanego ciągu
void Menu::decrypt_key(){
    int type = get_decoding_mode(); // type: 1-szyfrowanie pliku .txt, 2-szyfrowanie ciagu znakow 
    std::string encrypted_msg;
    int key;
    // pobierz ciag
    switch(type){
        case 1: // szyfrowanie z pliku .txt
            get_message_from_file(encrypted_msg); break;
        case 2: // szyfrowanie z ciągu znaków
            get_message_from_output(encrypted_msg); break;
    }
    get_key_to_decode(key, encrypted_msg);
    encryption.set_key(key);
    std::string decrypted = encryption.ceaser_chip_decrypt(encrypted_msg, animation);
    decrypt_modules(decrypted);
}