#pragma once
#include <iostream>
#include <fstream>
#include <vector>

// Klasa do zapisywania i odczytu plików (.txt, .bin oraz json)
struct file{
    static void init(); // funkcja tworząca folder (wrazie potrzeby, jeżeli nie istnieje)
    static void save_bin(const std::string& file_name, std::vector<unsigned char>& bytes);
    static void save_txt(const std::string& file_name, const std::string& key);
    static void save_json(const std::string& key, int val); // zwykłe argumenty dla key i val - dostosowane do naszego programu
    static std::string read_text(const std::string& path, bool& err);
    static int get_key_count(); // funkcja zwracająca ilość plików .bin i .txt kluczów - dla unikalnej nazwy
    static int get_encrypted_count(); // funkcja zwracająca ilosć plików .bin i .txt zaszyfrowanych wiadomości - dla unikalnej nazwy
    static int get_decrypted_count(); // funkcja zwracająca ilość plików .txt odszyfrowanych - dla unikalnej nazwy
};