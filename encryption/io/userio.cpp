#ifdef _WIN32 // dla obsługi ANSI w windows
#include <windows.h>
#endif // _WIN32
#include <iostream>
#include "../external/utfcpp/utf8.h"
#include <codecvt>
#include <locale>
#include "io/userio.hpp"
#include "../main/log/logWriter.hpp"
// Główna funkcja struktury ansiCol: ansi_ok() - sprawdza czy konsola obsługuje wypisywanie znaków ANSI (kolorwanie tekstu)
// Windows: włącza tryb ENABLE_VIRTUAL_TERMINAL_PROCESSING, jeśli to możliwe
// Linux/Unix/MacOS: w większości przypadków obsługa jest domyślna
bool ansi::ansi_ok(){
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return false; // brak dostępu do konsoli
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return false; // nie udało się pobrać trybu
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // włącz obsługę ANSI
        return SetConsoleMode(hOut, dwMode) != 0; // true, jeśli ustawienie się powiodło
    #else  
        return true; // dla Linux/Unix/MacOs obsługuje ANSI w ~99% przypadkach
    #endif
}
// Funkcja czyscząca konsole w zaleznosci od systemu
void ansi::clear_console(){
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear"); // Linux / macOS / inne UNIX
    #endif
}
// Główna funkcja con: copy_to_clipboard() - kopiuje podany tekst do systemowego schowka
// Windows: korzysta z natywnego WinAPI (OpenClipboard, EmptyClipboard, GlobalAlloc, SetClipboardData)
// Linux/Unix: używa zewnętrznego narzędzia `xclip` z opcją `-selection clipboard` (wymaga instalacji xclip - sprawdzane)
void con::copy_to_clipboard(LogWriter& log, const std::string& msg, int& err_stat) {
#ifdef _WIN32
    if (!OpenClipboard(nullptr)) {
        log.write(LogWriter::log_type::ERROR, "Can't open clipboard!", false,
                  "Nie mozna otworzyc schowka!\n");
        err_stat = 1;
        return;
    }
    EmptyClipboard();
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, msg.size() + 1);
    if (!hGlob) {
        CloseClipboard();
        log.write(LogWriter::log_type::ERROR, "Bad alloc", false,
                  "Blad alokacji pamieci\n");
        err_stat = 1;
        return;
    }
    memcpy(GlobalLock(hGlob), msg.c_str(), msg.size() + 1);
    GlobalUnlock(hGlob);
    SetClipboardData(CF_TEXT, hGlob);
    CloseClipboard();
#else
    FILE* pipe = popen("xclip -selection clipboard 2>/dev/null", "w");
    if (!pipe) {
        log.write(LogWriter::log_type::WARNING, "Clipboard copy failed. Make sure you have xclip installed", false, "Blad podczas kopiowania do schowka. Upewnij sie, ze masz pobrane xclip\n");
        err_stat = 1;
        return;
    }
    fwrite(msg.data(), 1, msg.size(), pipe);
    int status = pclose(pipe);
    if (status != 0) {
        log.write(LogWriter::log_type::WARNING, "Clipboard copy failed. Make sure you have xclip installed", false,"Blad podczas kopiowania do schowka. Upewnij sie, ze masz pobrane xclip\n");
        err_stat = 1;
    }
#endif
}

// Funkcja wyświetlająca nagłówek podczasz szyfrowania
void con::print_encrypt_header(bool decrypting){
     std::cout << ansi::red << "▄▄▄▄▄▄▄▄▄▄▄        ▄▖▄▖▄▖▄▖▄▖▄▖\n" << 
                               (decrypting ? "De" : "S") << "zyfrowanie" << (decrypting ? "" : " ") << "       ▌ ▌▌▙▖▚ ▌▌▙▘\n" << ansi::reset << 
                               "twojej wiadomości" << ansi::red << "  ▙▖▛▌▙▖▄▌▛▌▌▌\n" << ansi::reset;
}


// Funkcja zamieniajaca U+XXXX na UTF-8
std::string con::u32char_to_utf8(char32_t cp) {
    std::string out;
    utf8::unchecked::append(cp, std::back_inserter(out));
    return out;
}

// Funkcja zamieniająca u32string na UTF-8
// u32string -> UTF-8
std::string con::u32str_to_utf8(const std::u32string& s32) {
    std::string out;
    utf8::unchecked::utf32to8(s32.begin(), s32.end(), std::back_inserter(out));
    return out;
}
