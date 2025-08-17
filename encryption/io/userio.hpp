#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif
#include <iostream>
#include <string_view> // dla wyświetlania kolorów, alternatywa dla std::string - szybsza
#include "../main/log/logWriter.hpp"
// Struktura przechowująca kolory ANSI oraz funkcja sparwdzająca kompatybilność konsoli z ANSI
struct ansi{
    inline static constexpr const char* reset       = "\033[0m";
    inline static constexpr const char* black       = "\033[1;30m";
    inline static constexpr const char* red         = "\033[1;31m";
    inline static constexpr const char* green       = "\033[1;32m";
    inline static constexpr const char* yellow      = "\033[1;33m";
    inline static constexpr const char* blue        = "\033[1;34m";
    inline static constexpr const char* purple      = "\033[1;35m";
    inline static constexpr const char* cyan        = "\033[1;36m";
    inline static constexpr const char* gray        = "\033[1;37m";
    inline static constexpr const char* light_red   = "\033[1;31m";
    inline static constexpr const char* light_green = "\033[1;32m";
    inline static constexpr const char* light_yellow= "\033[1;33m";
    inline static constexpr const char* light_blue  = "\033[1;34m";
    inline static constexpr const char* light_purple= "\033[1;35m";
    inline static constexpr const char* light_cyan  = "\033[1;36m";
    inline static constexpr const char* white       = "\033[1;37m";
    static bool ansi_ok();
    static void clear_console();
};
struct con{
    static void copyToClipboard(LogWriter& log, const std::string& msg, int& err_stat);
    static void print_encrypt_header(bool decrypting = false);
};