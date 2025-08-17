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
Menu::Menu() : log(), keys_manager(log), encryption(log){
    log.write(LogWriter::log_type::INFO, "Log module loaded\n");
    file::init();
    log.write(LogWriter::log_type::INFO, "File system init succesfull\n");
}
// ----- Pomocnicze funkcje: -----
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
// Moduły zapisywania klucza
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
    // zapisz do .txt (zamieniając kodowanie na hex)
    std::string hex_msg = Encryption::return_hex(key);
    file::save_txt(new_path+"unique_key_"+std::to_string(file::get_key_count())+".txt", hex_msg);
    log.write(LogWriter::log_type::INFO, "Saved key to txt file ("+new_path+"unique_key_"+std::to_string(key_files_count)+".txt)\n", false, "Zapisano klucz do pliku tekstowego ("+new_path+"unique_key_"+std::to_string(key_files_count)+".txt)"); std::cout << "\n";
    // aktualizuj config json
    file::save_json("keyTextFilesCount",(file::get_key_count()+1));
    log.write(LogWriter::log_type::INFO, "Updated json config state\n", false, "Zaktualizowano stan konfiguracji (config.json)"); std::cout << "\n";
}
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
        key = keys_manager.random_key(size, msg_size);
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
// Funkcja do urochomienia modułów po wygenerowaniu klucz
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
// Główna funkcja do wyświetlania menu
void Menu::print(){
    // przygotuj zmienne
    int operation = 0;
    error_state err = error_state::NONE; // na początku ustaw na domyślne - brak błędu
    while(true){
        // ui
        ansi::clear_console();
        std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                "Szyfrowanie" << "  ▚▘▌▌▙▘\n" << ansi::reset << 
                                "1. Szyfruj" << ansi::green << "   ▌▌▙▌▌▌\n" << ansi::reset;
        std::cout << "2. Deszyfruj\n3. Wygeneruj klucz\n";
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
        log.write(LogWriter::log_type::ASK, "Asking user for menu operation\n", false, std::string(ansi::green)+"Wybierz operacje: ");
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
    std::string str_len;
    int len;
    get_key_from_user(len, str_len);
    // końcowy rozmiar klucza, zmieniany przez referencje w keys_manager.random_key()
    int add_size = 0;
    std::string key = keys_manager.random_key(add_size, (str_len.empty() ? 0 : len)); // 0 dla przypadku bazowego gdy wczytano pustego stringa - w tym przypadku losuj (zobacz keygenerator.cpp)\\
    // ui
    ansi::clear_console();
    // ustaw potrzebne zmienne dla prawidłowego zapisania danych (pobiera ilość dotychczas zapisanych plików klucza, aby wygenerować unikatowy folder i nazwe)
    int next_key_index = file::get_key_count()+1;
    // ui
    std::cout << ansi::green << "▄▄▄▄▄▄▄▄▄▄▄  ▖▖▄▖▄▖\n" << 
                                "Twoj losowy" << "  ▚▘▌▌▙▘\n"
                                "klucz: (" << next_key_index << ")"<< (next_key_index < 100 ? " " : "") << ansi::green << " ▌▌▙▌▌▌\n" << ansi::reset;
    // informuj o utworzonym kluczu i utwórz wektor odpowiadający bajtom klucza, użyte w pliku .bin
    log.write(LogWriter::log_type::KEY, "Succesfully created unique key\n", false, key); 
    std::cout << ansi::green << " (Dlugosc: " << add_size << ")" << ansi::reset << "\n";
    // wywołuj moduły tworzenia klucza (zobacz definicje funkcji)
    create_key_modules(key, next_key_index);
}

// Funkcja do szyfrowania wiadomosci i wyswietlania zaszysfrowanego ciągu
void Menu::encrypt_key(){
    int type = get_message_mode(); // type: 1-szyfrowanie pliku .txt, 2-szyfrowanie ciagu znakow 
    std::string msg;
    std::string key;
    // pobierz ciąg
    switch(type){
        case 1: // szyfrowanie z pliku .txt
            get_message_from_file(msg); break;
        case 2: // szyfrowanie z ciągu znaków
            get_message_from_output(msg); break;
    }
    // pobierz klucz i ustaw go w klasie szyfrowania
    get_key_from_user(key, msg.size());
    encryption.set_key(key);
    // rozpocznij generowanie
    std::string encrypted = encryption.xor_encrypt(msg);
    // wywołaj moduły po wygenerowaniu klucza
    encrypt_modules(encrypted, key);
}
// Funkcja do deszyfrowania wiadomości i wyświetlania deszyfrowanego ciągu
void Menu::decrypt_key(){
    int type = get_decoding_mode(); // type: 1-szyfrowanie pliku .txt, 2-szyfrowanie ciagu znakow 
    int style = get_decoding_style_mode(); // msg style: 1 - HEX, 2 - Char (DEC)
    std::string encrypted_msg;
    std::string key;
    // pobierz ciag
    switch(type){
        case 1: // szyfrowanie z pliku .txt
            get_message_from_file(encrypted_msg); break;
        case 2: // szyfrowanie z ciągu znaków
            get_message_from_output(encrypted_msg); break;
    }
    get_key_to_decode(key, encrypted_msg, (style == 1));
    if(style == 1){
        encrypted_msg = hex_to_dec(encrypted_msg);
    }
    encryption.set_key(key);
    std::string decrypted = encryption.xor_encrypt(encrypted_msg, true);
    decrypt_modules(decrypted);
}