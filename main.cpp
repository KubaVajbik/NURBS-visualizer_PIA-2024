#include <iostream> //knihovna pro hrani s io
#include <fstream>  //knihovna pro praci se soubory
#include <string>   //knihovna pro praci s retezci znaku
#include <vector>   //knihovna pro dynamicka pole
#include <cstdio>   //pro zastaveni terminalu
#include <algorithm>    //pro praci s vektory a tak
#include <cmath>    //matika a tak
#include <limits> // This is important!
#include "nurbs.h"  //pripojeni knihovny s funkcemi nurbs
#include "projection.h" //pripojeni knihovny s funkcemi vykreslovani
#include "ioparametru.h"    //pripojeni nihovny s funkcemi pro input a output parametru
#include <SDL2/SDL.h>   //kinovna pro praci s grafickymi prostredimi

void drawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

int main( int argc, char *argv[]) { //hlavni funkce
    //////////////INPUT SEKCE//////////////
    bool chybaSNacitanimBodu = true;
    std::tuple<std::vector<std::vector<float>>, bool> outputRidiciBody;
    while(chybaSNacitanimBodu){
        std::string nazevSouboruSeZadanymiBody; //self explanatory
        std::cout << "Zadej nazev souboru se souradnicemi ridicich bodu P_i: ";
        std::cin >> nazevSouboruSeZadanymiBody;

        outputRidiciBody = poleRidicichBoduZTxt(nazevSouboruSeZadanymiBody);  //funkce pro nacteni hodnot z .txt souboru do pole
        chybaSNacitanimBodu = std::get<1>(outputRidiciBody);   //zprava o tom, zda se hodnoty nacetly spravne
    }
    std::vector<std::vector<float>> poleRidicichBodu = std::get<0>(outputRidiciBody);   //pole nactenych hodnot souradnic ridicich bodu
    int pocetRidicichBodu = (int)poleRidicichBodu.size();

    bool chybaSNacitanimVektoruU = true;
    std::tuple<std::vector<float>, bool> outputVektorU;
    while(chybaSNacitanimVektoruU){
        std::string nazevSouboruSUzlovymVektorem; //self explanatory
        std::cout << "Zadej nazev souboru s uzlovym vektorem U: ";
        std::cin >> nazevSouboruSUzlovymVektorem;

        outputVektorU = vektorZTxt(nazevSouboruSUzlovymVektorem); //funkce pro nacteni hodnot vektoru z .txt
        chybaSNacitanimVektoruU = std::get<1>(outputVektorU);   //zprava o tom, zda se hodnoty nacetly spravne
    }
    std::vector<float> vektorU = std::get<0>(outputVektorU);   //pole vektoru U
    int pocetUzlu = (int)vektorU.size();
    std::vector<int> aktivniUzloveRoztece = uzloveRoztece(vektorU);

    kontrolaVlastnostiVektoruU(chybaSNacitanimVektoruU, aktivniUzloveRoztece);

    bool chybaSNacitanimVektoruW = true;
    std::tuple<std::vector<float>, bool> outputVektorW;
    while(chybaSNacitanimVektoruW){
        std::string nazevSouboruSVektoremVah; //self explanatory
        std::cout << "Zadej nazev souboru s vektorem vah W: ";
        std::cin >> nazevSouboruSVektoremVah;

        outputVektorW = vektorZTxt(nazevSouboruSVektoremVah); //funkce pro nacteni hodnot vektoru z .txt
        chybaSNacitanimVektoruW = std::get<1>(outputVektorW);   //zprava o tom, zda se hodnoty nacetly spravne
    }
    std::vector<float> vektorW = std::get<0>(outputVektorW);   //pole vektoru W
    int pocetVah = (int)vektorW.size();

    if(chybaSNacitanimVektoruW == false){   //kontrola pozadavku na vektor V
        if(pocetRidicichBodu != pocetVah){
            std::cout << "-> Ve vektoru vah musi byt tolik prvku kolik je ridicich bodu (" << pocetRidicichBodu << ")! Zkontroluj obsah obou souboru!" << std::endl;
            chybaSNacitanimVektoruW = true;
        }
    }

    bool chybaSeStupnemKrivkyP = true;
    float pPom = 0;
    kontrolaVlastnostiStupneP(chybaSeStupnemKrivkyP, pPom, pocetUzlu, pocetRidicichBodu);
    int p = (int)pPom;

    //////////////HLAVNI TELO FUNKCE//////////////
    if(chybaSNacitanimBodu + chybaSNacitanimVektoruU + chybaSNacitanimVektoruW + chybaSeStupnemKrivkyP == 0){
        //pro vsechny dimenze
        int dimenzeProstoru = (int)poleRidicichBodu[0].size();

        std::cout << std::endl; //novy radek
        std::cout << "Pro jednoznacne zadani krivky toho vic nepotrebujeme." << std::endl;  //zprava o uspesnem nacteni dat
        std::cout << "Ted prejdeme k nastaveni aproximace zadane krivky..." << std::endl;
        std::cout << std::endl; //novy radek

        int useky = zadaniPoctuUseku();
        
        float maxU = *std::max_element(vektorU.begin(), vektorU.end()); //vypocitame rozmezi parametru u
        float minU = *std::min_element(vektorU.begin(), vektorU.end());

        //vypocitame souradnice diskretnich bodu krivky
        std::vector<std::vector<float>> diskretizaceKrivky; //hodnoty souradnic bodu krivky
        std::ofstream zapisDoSouboru("diskretizaceKrivky.txt"); //soubor pro zapis tech samych hodnot
        for(float u = minU; u <= maxU; u += (maxU-minU)/(useky-1)){
            int aktualniRoztec = naJakeUzloveRozteciZrovnaJsme(vektorU, aktivniUzloveRoztece, u);   //aktualni aktivni uzlova roztec
            std::vector<float> bazfun = bazoveFunkceBSpline(aktualniRoztec, vektorU, u, p); //napocitani B-spline bazovych funkci pro u
            std::vector<float> racfun = racionalniBazoveFunkce(bazfun, vektorW);    //napocitani racionalnich bazovych funkci z B-spline bazovych funkci
            std::vector<float> souradnice = rceNurbsKrivky(poleRidicichBodu, racfun, dimenzeProstoru);  //napocitani souradnic bodu krivky pro stavajici u
            for(int i = 0; i<dimenzeProstoru; i++){ //zapis hodnot do souboru
                zapisDoSouboru << souradnice[i] << " ";
            }
            diskretizaceKrivky.push_back(souradnice);  //zapis hodnot do pole
            zapisDoSouboru << std::endl;
        }

        if(dimenzeProstoru > 3){    //pro dimenzi vyssi jak 3 to nebudeme vykreslovat
            std::cout << "Pro dimenzi porstoru vetsi nez 3 uz ti vic nepomuzu." << std::endl;
            std::cout << "Uzij si textovy soubor diskretizaceKrivky.txt s vygenerovanymi souradnicemi bodu krivky." << std::endl;
        }
        if(dimenzeProstoru < 3){
            std::cout << "Pro dimenzi porstoru mensi nez 3 uz ti vic nepomuzu." << std::endl;
            std::cout << "Uzij si textovy soubor diskretizaceKrivky.txt s vygenerovanymi souradnicemi bodu krivky." << std::endl;
        }
        if(dimenzeProstoru == 3){    //pro dimenzi prostoru 3 to zobrazime v perspektive
            bool vykresleniZnovu = true;
            std::cout << "Pro dimenzi porstoru 3 ti vedle vygenerovaneho souboru diskretizaceKrivky.txt se rouradnicemi bodu krivky jeste umim vykreslit vyslednou krivku." << std::endl;
            std::cout << "Potrebuju, abys mi zadal souradnice a natoceni kamery:" << std::endl;
            for(int i = 0; i<pocetRidicichBodu; i++){
                poleRidicichBodu[i].push_back(1);   //priprava pro homogenni souradnice
            }
            for(int i = 0; i<(int)diskretizaceKrivky.size(); i++){
                diskretizaceKrivky[i].push_back(1);   //priprava pro homogenni souradnice
            }
            while(vykresleniZnovu == true){
                float xKamera = inputFloat("Zadej souradnici x kamery: ");    //hodnota souradnice x kamery

                float yKamera = inputFloat("Zadej souradnici y kamery: ");    //hodnota souradnice y kamery

                float zKamera = inputFloat("Zadej souradnici z kamery: ");    //hodnota souradnice z kamery

                float ryKamera = inputFloat("Zadej uhel kamery, o jaky je naklonena dolu: ");    //hodnota uhlu ry kamery
                
                float rzKamera = inputFloat("Zadej uhel kamery, o jaky je otocena od zaporne osy x: ");    //hodnota uhlu rz kamery

                std::vector<std::vector<float>> transformaceDoKamery = maticeWorldToCamera(xKamera, yKamera, zKamera, -ryKamera, rzKamera);  //preneseni sceny do souradnic kamery, ktera se momentalne diva ve smeru zaporne osy z
                std::vector<std::vector<float>> maticePromitani = maticeStredovehoPromitani(45, 100, 0.5);  //matice stredoveho promitani

                //promitneme vsechny ridici body
                std::vector<std::vector<float>> promitnuteRidiciBody(0);    //pole, kam je budeme ukadat
                //std::ofstream zapisDoSouboruRidiciBody("promitnuteRidiciBody.txt"); //soubor do ktereho je ulozime
                for(int i = 0; i<pocetRidicichBodu; i++){    //promitnuti vsech ridicich bodu
                    std::vector<float> promitnutyRidiciBod = promitnuti(transformaceDoKamery, maticePromitani, poleRidicichBodu[i]);
                    promitnuteRidiciBody.push_back(promitnutyRidiciBod);    //zapis hodnot do pole
                    //for(int j = 0; j<dimenzeProstoru+1; j++){ //zapis hodnot do souboru
                    //    zapisDoSouboruRidiciBody << promitnutyRidiciBod[j] << " ";
                    //}
                    //zapisDoSouboruRidiciBody << std::endl;
                }

                //promitneme vsechny diskretni body krivky
                std::vector<std::vector<float>> promitnuteBodyKrivky(0);    //pole, kam je budeme ukadat
                //std::ofstream zapisDoSouboruBodyKrivky("promitnuteBodyKrivky.txt"); //soubor do ktereho je ulozime
                for(int i = 0; i<(int)diskretizaceKrivky.size(); i++){  //promitnuti vsech bodu krivky
                    std::vector<float> promitnutyBodKrivky = promitnuti(transformaceDoKamery, maticePromitani, diskretizaceKrivky[i]);
                    promitnuteBodyKrivky.push_back(promitnutyBodKrivky);    //zapis hodnot do pole
                    //for(int j = 0; j<dimenzeProstoru+1; j++){ //zapis hodnot do souboru
                    //    zapisDoSouboruBodyKrivky << promitnutyBodKrivky[j] << " ";
                    //}
                    //zapisDoSouboruBodyKrivky << std::endl;
                }

                //promitnuti bodu definujicich osovy kriz
                std::vector<float> pocatek = {0, 0, 0, 1};  //pocatek
                std::vector<float> bodX1 = {-3, 0, 0, 1};   //bod definujici zapornou osu x
                std::vector<float> bodX2 = {3, 0, 0, 1};    //bod definujici kladnou osu x
                std::vector<float> bodY1 = {0, -3, 0, 1};   //bod definujici zapornou osu y
                std::vector<float> bodY2 = {0, 3, 0, 1};    //bod definujici kladnou osu y
                std::vector<float> promitnutyPocatek = promitnuti(transformaceDoKamery, maticePromitani, pocatek);
                std::vector<float> promitnutyX1 = promitnuti(transformaceDoKamery, maticePromitani, bodX1);
                std::vector<float> promitnutyX2 = promitnuti(transformaceDoKamery, maticePromitani, bodX2);
                std::vector<float> promitnutyY1 = promitnuti(transformaceDoKamery, maticePromitani, bodY1);
                std::vector<float> promitnutyY2 = promitnuti(transformaceDoKamery, maticePromitani, bodY2);

                //////////////NASTAVENI VYKRESLENI//////////////
                SDL_Init( SDL_INIT_EVERYTHING );    //zapne vsechno v knihovne SDL2

                SDL_DisplayMode DM;
                SDL_GetCurrentDisplayMode(0, &DM);
                int WIDTH = DM.h*0.9;   //sezene sirku a vysku pouzivaneho displeje
                int HEIGHT = DM.h*0.9;

                preskalovaniKameraObrazovkaSeznam(promitnuteRidiciBody, WIDTH, HEIGHT); //preskalovani souradnic z kamery na obrazovku pro ridici body
                preskalovaniKameraObrazovkaSeznam(promitnuteBodyKrivky, WIDTH, HEIGHT); //preskalovani souradnic z kamery na obrazovku pro promitnute body
                preskalovaniKameraObrazovkaVektor(promitnutyPocatek, WIDTH, HEIGHT);    //preskalovani souradnic z kamery na obrazovku pro promitnuty pocatek
                preskalovaniKameraObrazovkaVektor(promitnutyX1, WIDTH, HEIGHT); //preskalovani souradnic z kamery na obrazovku pro bod definujici zapornou osu x
                preskalovaniKameraObrazovkaVektor(promitnutyX2, WIDTH, HEIGHT); //preskalovani souradnic z kamery na obrazovku pro bod definujici kladnou osu x
                preskalovaniKameraObrazovkaVektor(promitnutyY1, WIDTH, HEIGHT); //preskalovani souradnic z kamery na obrazovku pro bod definujici zapornou osu y
                preskalovaniKameraObrazovkaVektor(promitnutyY2, WIDTH, HEIGHT); //preskalovani souradnic z kamery na obrazovku pro bod definujici kladnou osu y

                SDL_Window *windowPerspektiva = SDL_CreateWindow( "NURBS - visualizer: Pohled v perspektivě", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0); //nastaveni okna vcetne rozmeru a tak

                SDL_Renderer *rendererPerspektiva = SDL_CreateRenderer(windowPerspektiva, -1, SDL_RENDERER_ACCELERATED);    //vytvoreni objektu ktery do okna bude kreslit

                if( NULL == windowPerspektiva){    //kdyby se okno neinicializovalo
                    std::cout << "Could not create window: " << SDL_GetError() << std::endl;
                    return 1;
                }

                std::cout << "Pro pokracovani zavri okno s pohledem v perspektive" << std::endl;

                SDL_Event windowEvent;  //promenna pro tracking udalosti

                bool running = true;
                while(running){    //tohle proste bezi furt

                    //event management
                    if(SDL_PollEvent(&windowEvent)){    //pokud teda existuje nejaka udalost, tak budeme kontrolovat co znamena
                        switch(windowEvent.type){
                            case SDL_QUIT:
                                running = false;
                                break;
                        }
                    }
                    SDL_SetRenderDrawColor(rendererPerspektiva, 0, 0, 0, SDL_ALPHA_OPAQUE);    //barva pozadi
                    SDL_RenderClear(rendererPerspektiva);  //vycisteni rendereru

                    SDL_SetRenderDrawColor(rendererPerspektiva, 150, 0, 0, SDL_ALPHA_OPAQUE);
                    drawCircle(rendererPerspektiva, promitnutyPocatek[0], promitnutyPocatek[1], HEIGHT*0.007);
                    SDL_RenderDrawLine(rendererPerspektiva, promitnutyX1[0], promitnutyX1[1], promitnutyPocatek[0], promitnutyPocatek[1]);  //zaporna osa x
                    SDL_RenderDrawLine(rendererPerspektiva, promitnutyX2[0], promitnutyX2[1], promitnutyPocatek[0], promitnutyPocatek[1]);  //kladna osa x
                    SDL_RenderDrawLine(rendererPerspektiva, promitnutyY1[0], promitnutyY1[1], promitnutyPocatek[0], promitnutyPocatek[1]);  //zaporna osa y
                    SDL_RenderDrawLine(rendererPerspektiva, promitnutyY2[0], promitnutyY2[1], promitnutyPocatek[0], promitnutyPocatek[1]);  //kladna osa y

                    SDL_SetRenderDrawColor(rendererPerspektiva, 150, 150, 150, SDL_ALPHA_OPAQUE);
                    for(int i = 1; i<pocetRidicichBodu; i++){   //vykresleni ridiciho polygonu
                        SDL_RenderDrawLine(rendererPerspektiva, promitnuteRidiciBody[i-1][0], promitnuteRidiciBody[i-1][1], promitnuteRidiciBody[i][0], promitnuteRidiciBody[i][1]);
                    }

                    for(int i = 0; i<pocetRidicichBodu; i++){   //vykresleni ridicich bodu
                        drawCircle(rendererPerspektiva, promitnuteRidiciBody[i][0], promitnuteRidiciBody[i][1], HEIGHT*0.007);
                    }

                    SDL_SetRenderDrawColor(rendererPerspektiva, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    for(int i = 1; i<(int)diskretizaceKrivky.size(); i++){   //vykresleni bodu krivky
                        SDL_RenderDrawLine(rendererPerspektiva, promitnuteBodyKrivky[i-1][0], promitnuteBodyKrivky[i-1][1], promitnuteBodyKrivky[i][0], promitnuteBodyKrivky[i][1]);
                    }

                    SDL_RenderPresent(rendererPerspektiva);    //zobrazi co jsme nakreslili
                    SDL_Delay(100);  //okno bezi na +- 10 fps
                }

                SDL_DestroyWindow(windowPerspektiva);  //cleanup
                SDL_DestroyRenderer(rendererPerspektiva);
                SDL_Quit();

                char odpoved;   
                std::cout << "Chces krivku vykreslit znovu s jinou pozici kamery? [y/n]: "; //moznost zadat znova souradnice kamery a vykreslit krivku
                std::cin >> odpoved;

                while(!std::cin.good() || (odpoved != 'n' && odpoved != 'y' && odpoved != 'N' && odpoved != 'Y')){  //dokud nesplnuji podminku
                    std::cout << "-> Prosim vyber ze dvou v nabidce uvedenych znaku." << std::endl;
                    std::cout << "Chces krivku vykreslit znovu s jinou pozici kamery? [y/n]: ";
                    std::cin >> odpoved;
                }
                if(std::cin.good()){    //kdyz splnuji podminku
                    if(odpoved == 'n' || odpoved == 'N'){   //pokud nechceme dalsi vykresleni
                        vykresleniZnovu = false;    //zastavime while cyklus
                    }
                }

            }
            std::cout << "OK, to je ode me vse. Cau!" << std::endl; //zprava pred koncem programu
        }
    }

    //////////////PAUZA PRED KONCEM PORGRAMU//////////////
    std::cout << "Press Enter to close the program . . .";  //zastaveni terminalu pro zobrazeni a cteni vysledku
    std::cin.ignore(INT_MAX, '\n'); //ignoruje vsechno ve streambufferu az do konce radky (tu my zadame)
    std::getchar(); //zmacknuti klavesy enter vejde do std::cin a tim to projede az na koncec funkce main

    return 0;
}