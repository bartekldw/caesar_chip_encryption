#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "encryption/keygenerator.hpp"
#include "../main/log/logWriter.hpp"
Key::Key(LogWriter& log) : log(log), gen(rd()), char_dist(33,126), //33 - 126 = drukowalne znaki ASCII
                                                int_dist(32,128){ 

}
// Funkcja do generowania losowego klucza 
std::string Key::random_key(int& size, size_t key_len){
    log.write(LogWriter::log_type::INFO, "Key generation start\n");
    std::string key;
    size = (key_len == 0 ? int_dist(gen) : key_len);
    for(int i = 0; i < size; ++i){
        char c = static_cast<char>(char_dist(gen));
        log.write(LogWriter::log_type::INFO, "Generating key["+std::to_string(i)+"] = "+c+"\n");
        key += c;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return key;
}
// Statyczna funkcja do losowania liczb (używana bez obiektu)
int Key::randint(int beg, int end){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist (beg, end);
    return dist(gen);
}