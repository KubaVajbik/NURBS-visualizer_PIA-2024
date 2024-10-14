//soubor s hlavickami funkci definovanch v projection.cpp
// Prikazy pro preprocesor zabranujici vicenasobnemu vlozeni
#ifndef PROJECTION_H
#define PROJECTION_H

#include <vector>   //knihovna pro dynamicka pole

std::vector<std::vector<float>> Rx(float gamma);    //hlavicka transformacni matice rotace okolo osy x pro sloupcove vektory

std::vector<std::vector<float>> Ry(float gamma);    //hlavicka transformacni matice rotace okolo osy y pro sloupcove vektory

std::vector<std::vector<float>> Rz(float gamma);    //hlavicka transformacni matice rotace okolo osy z pro sloupcove vektory

std::vector<std::vector<float>> Tx(float dist);    //transformacni matice posunu podel osy x pro sloupcove vektory

std::vector<std::vector<float>> Ty(float dist);    //transformacni matice posunu podel osy y pro sloupcove vektory

std::vector<std::vector<float>> Tz(float dist);    //transformacni matice posunu podel osy z pro sloupcove vektory

std::vector<std::vector<float>> matMult(std::vector<std::vector<float>> A, std::vector<std::vector<float>> B);  //hlavicka funkce pro nasobeni dvou matic

std::vector<float>  vecMatMult(std::vector<std::vector<float>> A, std::vector<float> b);    //hlavicka funkce pro nasobeni matice a sloupcoveho vektoru

std::vector<std::vector<float>> maticeWorldToCamera(float x, float y, float z, float gammaY, float gammaZ);   //hlavicka funkce transformacni matice do souradnic kamery

std::vector<std::vector<float>> maticeStredovehoPromitani(float zornyUhel, float farPlane, float nearPlane);   //hlavicka funkce transformacni matice stredove projekce

std::vector<float>  promitnuti(std::vector<std::vector<float>> maticeTransformaceDoKamery, std::vector<std::vector<float>> maticePromitani, std::vector<float> promitanyBod);    //hlavicka funkce promitajici bod

#endif