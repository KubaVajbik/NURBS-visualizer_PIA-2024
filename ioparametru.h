//soubor s hlavickami funkci definovanch v ioParametru.cpp
// Prikazy pro preprocesor zabranujici vicenasobnemu vlozeni
#ifndef IOPARAMETRU_H
#define IOPARAMETRU_H

#include <cmath>    //matika a tak
#include <vector>   //knihovna pro dynamicka pole
#include <string>   //knihovna pro praci s retezci znaku

void kontrolaVlastnostiVektoruU(bool& chybaSNacitanimVektoruU, std::vector<int> aktivniUzloveRoztece);  //hlavicka funkce pro kontrolu vlastnosti vektoru U

void kontrolaVlastnostiStupneP(bool& chybaSeStupnemKrivkyP, float& pPom, int pocetUzlu, int pocetRidicichBodu);  //hlavicka funkce pro pozadavku stupne p

int zadaniPoctuUseku();  //funkce pro kontrolu pozadavku na pocet useku

float inputFloat(std::string promptMessage);    //hlavicka funkce pro nacitani a kontrolu hodnoty promenne typu float

void preskalovaniKameraObrazovkaSeznam(std::vector<std::vector<float>>& seznamPromitnutychBoduKPreskalovani, int WIDTH, int HEIGHT);  //hlavicka funkce pro preskalovani ze souradnic senzoru do souradnic obrazovky

void preskalovaniKameraObrazovkaVektor(std::vector<float>& seznamPromitnutychBoduKPreskalovani, int WIDTH, int HEIGHT);  //funkce pro preskalovani ze souradnic senzoru do souradnic obrazovky

#endif