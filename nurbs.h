//soubor s hlavickami funkci definovanch v nurbs.cpp
// Prikazy pro preprocesor zabranujici vicenasobnemu vlozeni
#ifndef NURBS_H
#define NURBS_H

#include <string>   //knihovna pro praci s retezci znaku
#include <vector>   //knihovna pro dynamicka pole

std::tuple<std::vector<std::vector<float>>, bool> poleRidicichBoduZTxt(std::string nazevSouboruSeZadanymiBody); //hlavicka funkce pro vytvoreni pole souradnic ridicich bodu

std::tuple<std::vector<float>, bool> vektorZTxt(std::string nazevSouboruVektoru); //hlavicka funkce pro nacitani vektoru z textoveho souboru

void spatneBodyMsg(std::vector<int> seznamSpatnychBodu);    //hlavicka funkce pro zobrazeni informaci o spatne zadanch bodech

std::vector<int> uzloveRoztece(std::vector<float> uzlovyVektor);  //hlavicka funkce pro urcovani aktivnich a pasivnich uzlovych rozteci

int naJakeUzloveRozteciZrovnaJsme(std::vector<float> vektorU, std::vector<int> aktivniUzloveRoztece, float u); //hlavicka funkce urcujici aktualni roztec

float bazoveFunkceBSplineIter(int i, int k, std::vector<float> U, float u, float NiPredchozi, float NiPlusPredchozi);   //hlavicka iteracne funkce pocitajici hodnoty B-spline bazovych funkci

std::vector<float> bazoveFunkceBSpline(int aktualniRoztec, std::vector<float> U, float u, int p); //hlavicka funkce pocitajici aktualni hodnoty B-spline bazovych funkci

std::vector<float> racionalniBazoveFunkce(std::vector<float> bazoveFunkceBSpline, std::vector<float> W);    //hlavicka funkce pocitajici aktualni hodnoty racionalnich bazovych funkci

std::vector<float> rceNurbsKrivky(std::vector<std::vector<float>> poleRidicichBodu,  std::vector<float> poleRacionalnichBazovychFunkci, int dimenzeProstoru);   //hlavicka funkce ktera dopocita souradnice aktualniho bodu krivky

#endif