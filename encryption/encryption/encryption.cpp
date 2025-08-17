#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "../main/log/logWriter.hpp"
#include "io/userio.hpp"
#include "encryption/encryption.hpp"
Encryption::Encryption(LogWriter& log) : log(log){};
// Główna funkcja do szyfrowania
std::string Encryption::xor_encrypt(const std::string& msg, bool decrypting){
    std::string result;
    std::cout << std::setw(2) << std::setfill('0');
    for(size_t i = 0; i < msg.size(); ++i){
        char target = msg[i] ^ key[i % key.size()];
        for(int c = 32; c < 127; ++c){
            ansi::clear_console();
            con::print_encrypt_header(decrypting);
            std::cout << std::hex;
            for(const auto& element : result){
                std::cout << (int)element << " ";
            }
            if(i == msg.size() - 1){
                std::cout << (int)result.back() << "\n";
            }else{
                std::cout << std::dec;
                std::cout << ansi::green << (char)c << ansi::reset << "\n";
            }
            std::cout << std::dec;
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            if(char(c) == target){
                break;
            }
        }
        result += target;
    }
    ansi::clear_console();
    return result;
}
// Statyczna funkcja do zwracania bajtowej reprezentacji znaków
std::string Encryption::return_hex(const std::string& msg){
    std::ostringstream oss_hex;
    oss_hex << std::hex << std::setw(2) << std::setfill('0');
    for(const auto& element : msg){
        oss_hex << static_cast<int>(element);
    }
    return oss_hex.str();
}