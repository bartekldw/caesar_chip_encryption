#pragma once
#include <iostream>
#include "io/userio.hpp"
#include "../main/log/logWriter.hpp"
#include "encryption/keygenerator.hpp"
#include "encryption/encryption.hpp"
// Główna klasa zarządzająca menu, wykonująca akcje
class Menu{
private:
    // obiekt klasy do szyfrowania/deszyfrowania
    Encryption encryption;
    // obiekt logowania
    LogWriter log;
    // obiekt zarządzania kluczami
    Key keys_manager;
    // zmienna do przechowywania stanu aplikacji (zakonczona, nie zakonczona)
    bool completed = false;
    // enum class do przechwycenia błędu wejścia
    enum class error_state {NONE, CIN, NUM};
    // czy animacja?
    #ifdef ANIMATION_MODE
        bool animation = true;
    #else  
        bool animation = false; 
    #endif
    // pomocnicze funkcje:
    // dla klucza:
    void create_key_modules(const std::string& key, int key_files_count);
    // dla szyfrowania:
    int get_message_mode();
    void get_message_from_file(std::string& msg);
    void get_message_from_output(std::string& msg);
    void get_key_from_user(int& key);
    void encrypt_modules(const std::string& msg, const std::string& key);
    void encrypt_save_modules(const std::string& msg, const std::string& hex_msg);
    // dla deszyfrowania:
    int get_decoding_mode();
    int get_decoding_style_mode();
    void get_encoded_message_from_file(std::string& encoded);
    void get_encoded_message_from_output(std::string& encoded);
    void get_key_to_decode(int& key, const std::string& msg);
    void decrypt_modules(const std::string& decrypted);
    void decrypt_save_modules(const std::string& decrypted);
    std::string hex_to_dec(const std::string& hex);
public:
    Menu();
    void print();
    void random_key();
    void encrypt_key();
    void decrypt_key();
};