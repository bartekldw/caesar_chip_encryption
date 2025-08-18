#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "encryption/keygenerator.hpp"
#include "../main/log/logWriter.hpp"
Key::Key(LogWriter& log) : log(log), gen(rd()), int_dist(32,128) //33 - 126 = drukowalne znaki ASCII
                                                { 

}
// Funkcja do generowania losowego klucza 
int Key::random_key(){
    std::uniform_int_distribution<int> alp_dist (0,63);
    return alp_dist(gen);
}
// Statyczna funkcja do losowania liczb (używana bez obiektu)
int Key::randint(int beg, int end){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist (beg, end);
    return dist(gen);
}