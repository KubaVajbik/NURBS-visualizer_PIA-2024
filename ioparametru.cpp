//soubor pro definici zdrojoveho kodu funkci vstupu a vystupu
#include <iostream> //knihovna pro hrani s io
#include <cmath>    //matika a tak
#include <vector>   //knihovna pro dynamicka pole
#include <string>   //knihovna pro praci s retezci znaku

void kontrolaVlastnostiVektoruU(bool& chybaSNacitanimVektoruU, std::vector<int> aktivniUzloveRoztece){  //funkce pro kontrolu vlastnosti vektoru U
    if(chybaSNacitanimVektoruU == false){   //kontrola vlastnosti vektoru U
        if((int)aktivniUzloveRoztece.size() < 1){   //nepouzitelna data
            std::cout << "-> V uzlovem vektoru nebyla rozpoznana zadna aktivni uzlova roztec" << std::endl;
            chybaSNacitanimVektoruU = true;
        }
        else{   //pouzitelna data
            if(1 == (int)aktivniUzloveRoztece.size()){  //snaha o trochu cestiny
                std::cout << "-> V uzlovem vektoru byla rozpoznana jedna aktivni uzlova roztec." << std::endl;
            }
            else if(1 < (int)aktivniUzloveRoztece.size() && (int)aktivniUzloveRoztece.size() < 4){
                std::cout << "-> V uzlovem vektoru byly rozpoznany " + std::to_string(aktivniUzloveRoztece.size()) + " aktivni uzlove roztece." << std::endl;
            }
            else{
                std::cout << "-> V uzlovem vektoru bylo rozpoznano " + std::to_string(aktivniUzloveRoztece.size()) + " aktivnich uzlovych rozteci." << std::endl;
            }
        }
    }
}

void kontrolaVlastnostiStupneP(bool& chybaSeStupnemKrivkyP, float& pPom, int pocetUzlu, int pocetRidicichBodu){  //funkce pro kontrolu pozadavku na stupen p
    while(chybaSeStupnemKrivkyP){   //kontrola pozadavku na stupen krivky p
        std::cout << "Pro kontrolu zadej stupen p NURBS krivky: ";
        std::cin >> pPom;

        if(std::cin.good()){
            if(std::floor(pPom) != pPom){
                    std::cout << "-> Stupen p krivky musi byt cele cislo!" << std::endl;
                }
            else if(pocetUzlu - pocetRidicichBodu - 1 != pPom){
                std::cout << "-> Tento stupen krivky neodpovida drive zadanym vektorum a bodum. Nezapomen, ze pro aproximacni NURBS krivku musi platit vztah:" << std::endl;
                std::cout << "m = n + p + 1; kde (m+1) je pocet uzlu, (n+1) je pocet ridicich bodu a p je stupen krivky." << std::endl;
            }
            else {
                std::cout << "-> Stupen p krivky mi vysel stejny. Jedeme dal." << std::endl;
                chybaSeStupnemKrivkyP = false;
            }
        }
        else{
            std::cin.clear(); //(https://stackoverflow.com/questions/27003967/how-to-check-if-cin-is-int-in-c)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "-> Stupen p krivky musi byt cele cislo!" << std::endl;
        }
    }
}

int zadaniPoctuUseku(){  //funkce pro input a kontrolu pozadavku na pocet useku
    float useky;
    bool chybaSeZadanimUseku = true;
    while(chybaSeZadanimUseku){ //kontrola pozadavku na pocet useky
        std::cout << "Zadej pocet useku na ktere chces krivku aproximovat: ";
        std::cin >> useky;

        if(std::cin.good()){
            if(std::floor(useky) != useky){
                std::cout << "-> Pocet useku musi byt cele cislo!" << std::endl;
            }
            else {
                chybaSeZadanimUseku = false;
            }
        }
        else{
            std::cin.clear(); //(https://stackoverflow.com/questions/27003967/how-to-check-if-cin-is-int-in-c)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "-> Pocet useku musi byt cele cislo!" << std::endl;
        }
    }
    return (int)useky;
}

float inputFloat(std::string promptMessage){    //funkce pro nacitani a kontrolu hodnoty promenne typu float
    float promenna;    //hodnota kterou chceme ziskat
    bool chybnaHodnota = true;
    while(chybnaHodnota){
        std::cout << promptMessage;
        std::cin >> promenna;

        if(std::cin.good()){
            chybnaHodnota = false;
        }
        else{
            std::cin.clear(); //(https://stackoverflow.com/questions/27003967/how-to-check-if-cin-is-int-in-c)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "-> Zadavana ohdnota musi byt cislo!" << std::endl;
        }
    }
    return promenna;
}

void preskalovaniKameraObrazovkaSeznam(std::vector<std::vector<float>>& seznamPromitnutychBoduKPreskalovani, int WIDTH, int HEIGHT){  //funkce pro preskalovani ze souradnic senzoru do souradnic obrazovky
    float pom;
    for(int i = 0; i<(int)seznamPromitnutychBoduKPreskalovani.size(); i++){ //pro kazdy bod ze seznamu
        if(std::isfinite(seznamPromitnutychBoduKPreskalovani[i][0]) && std::isfinite(seznamPromitnutychBoduKPreskalovani[i][1])){   //pokud njsou jeho souradnice +-nekonecno
                pom = seznamPromitnutychBoduKPreskalovani[i][0];
                seznamPromitnutychBoduKPreskalovani[i][0] = (1+pom)*WIDTH/2;    //hodnotu jeho x souradnice prepocitame z intervalu [-1; 1] na interval [0; sirka displeje]
                pom = -seznamPromitnutychBoduKPreskalovani[i][1];
                seznamPromitnutychBoduKPreskalovani[i][1] = (1+pom)*HEIGHT/2;   //hodnotu jeho y souradnice prepocitame z intervalu [-1; 1] na interval [vyska displeje; 0]
        }
    }
}
//tyhle funkce strasne kravly kdyz byly skombinovany, proto jsou zvlast//
void preskalovaniKameraObrazovkaVektor(std::vector<float>& seznamPromitnutychBoduKPreskalovani, int WIDTH, int HEIGHT){  //funkce pro preskalovani ze souradnic senzoru do souradnic obrazovky
    float pom;
    if(std::isfinite(seznamPromitnutychBoduKPreskalovani[0]) && std::isfinite(seznamPromitnutychBoduKPreskalovani[1])){ //pokud njsou jeho souradnice +-nekonecno
                pom = seznamPromitnutychBoduKPreskalovani[0];
                seznamPromitnutychBoduKPreskalovani[0] = (1+pom)*WIDTH/2;   //hodnotu jeho x souradnice prepocitame z intervalu [-1; 1] na interval [0; sirka displeje]
                pom = -seznamPromitnutychBoduKPreskalovani[1];
                seznamPromitnutychBoduKPreskalovani[1] = (1+pom)*HEIGHT/2;  //hodnotu jeho y souradnice prepocitame z intervalu [-1; 1] na interval [vyska displeje; 0]
    }
}