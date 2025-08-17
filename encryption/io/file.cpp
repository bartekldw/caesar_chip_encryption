#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem> // dla utworzenia folderu
#include "../external/json/json.hpp"
#include "io/file.hpp"
// Funkcja tworząca folder do zapisywania danych
void file::init(){
    if (!std::filesystem::exists("../saved")) { 
        std::filesystem::create_directories("../saved");
    }
}
// Funkcja zapisująca dane do .bin
void file::save_bin(const std::string& file_name, std::vector<unsigned char>& bytes){
    std::ofstream out(file_name, std::ios::binary);
    out.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
    out.close();
}
// Funkcja zapisująca dane do .txt
void file::save_txt(const std::string& file_name, const std::string& key){
    std::ofstream out(file_name);
    out << key;
    out.close();
}
// Funkcja zapisująca key-value dane do JSON
void file::save_json(const std::string& key, int val){
    using json = nlohmann::json;
    json data;
    // wczytaj istniejące dane, jeśli plik istnieje
    std::ifstream in("../config.json");
    if (in.is_open() && in.peek() != std::ifstream::traits_type::eof()) {
        in >> data;
    }
    in.close();
    // załaduj nowe dane do pliku
    std::ofstream out("../config.json");
    data[key] = val;
    out << data.dump(4);
    out.close();
}

// Funkcja czytająca plik tekstowy na podstawie jego ścieżki
std::string file::read_text(const std::string& path, bool& err){
    std::ifstream in(path);
    if(!in.is_open()){ 
        err = true;
        return "";
    }
    std::ostringstream buf;
    buf << in.rdbuf();
    in.close();
    err = false;
    return buf.str();
}
// Funkcja zwracająca ilość plików z kluczem (za pomocą configu jsona)
int file::get_key_count(){
    using json = nlohmann::json;
    json data;
    // wczytaj json
    std::ifstream in("../config.json");
    if (in.is_open() && in.peek() != std::ifstream::traits_type::eof()) {
        in >> data;
        if (data.contains("keyTextFilesCount") && !data.empty()) {
            return data["keyTextFilesCount"];
        }
    }
    return 0; // jeśli nadal brak klucza, zwracamy 0
}
// Funkcja zwracająca ilość plików z zaszyfrowanymi wiadomościami (za pomocą configu jsona)
int file::get_encrypted_count(){
    using json = nlohmann::json;
    json data;
    // wczytaj json
    std::ifstream in("../config.json");
    if (in.is_open() && in.peek() != std::ifstream::traits_type::eof()) {
        in >> data;
        if (data.contains("encryptedFilesCount") && !data.empty()) {
            return data["encryptedFilesCount"];
        }
    }
    return 0; // jeśli nadal brak klucza, zwracamy 0
}
// Funkcja zwracająca ilość plików z odszyfrowanymi wiadomościami (za pomocą configu jsona)
int file::get_decrypted_count(){
    using json = nlohmann::json;
    json data;
    // wczytaj json
    std::ifstream in("../config.json");
    if (in.is_open() && in.peek() != std::ifstream::traits_type::eof()) {
        in >> data;
        if (data.contains("decryptedFilesCount") && !data.empty()) {
            return data["decryptedFilesCount"];
        }
    }
    return 0; // jeśli nadal brak klucza, zwracamy 0
}