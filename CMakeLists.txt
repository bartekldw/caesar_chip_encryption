cmake_minimum_required(VERSION 3.10)
set(project_name caesar_chiper)
project(${project_name})
message(STATUS "Konfiguracja rozpoczęta...")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
message(STATUS "Standard c++17 ustawiony")
set(src
    main.cpp
    main/log/logWriter.cpp
    encryption/encryption/encryption.cpp
    encryption/io/file.cpp
    encryption/encryption/keygenerator.cpp
    encryption/io/userio.cpp
    main/menu/menu.cpp
    main/menu/menu_encrypt.cpp
    main/menu/menu_decrypt.cpp
    main/menu/menu_key.cpp
)
message(STATUS "Ustawiono docelowe pliki")
add_executable(${project_name} ${src})
target_include_directories(${project_name} PRIVATE encryption)
# Usuń linijke poniżej jeżeli nie chcesz animacji szyfrowania i deszyfrowania (JEŻELI JUŻ SKOMPILOWANO, NALEŻY PRZEBUDOWAĆ FOLDER build)
add_compile_definitions(ANIMATION_MODE)
#######################################
message(STATUS "Kompilacja gotowa!")