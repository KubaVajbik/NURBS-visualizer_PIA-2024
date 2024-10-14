//soubor pro definici zdrojoveho kodu funkci NURBS
#include "nurbs.h"  //pripojeni knihovny s funkcemi nurbs
#include <iostream> //knihovna pro hrani s io
#include <fstream>  //knihovna pro praci se soubory
#include <string>   //knihovna pro praci s retezci znaku
#include <vector>   //knihovna pro dynamicka pole
#include <cctype>   //knihovna pro kontrolu jestli je znak cislo nebo ne

std::tuple<std::vector<std::vector<float>>, bool> poleRidicichBoduZTxt(std::string nazevSouboruSeZadanymiBody) {//funkce pro vytvoreni pole souradnic ridicich bodu
    std::tuple<std::vector<std::vector<float>>, bool> outputPoleRidicichBoduZTxt;   //definice vystupu funkce. pole a boolean

    std::ifstream zadaneBody(nazevSouboruSeZadanymiBody); //nacteni souboru se zadanymi souradnicemi ridicich bodu

    std::string line;   //pomocna promenna pro ulozeni radku ze souboru

    int pocetRidicichBodu = 0;  //pocet ridicich bodu (pocet radku v textovem souboru)
    int rozmerProstoru = 0; //promenna ukladajici pocet slov/souradnic v nactenem radku, pres kterou se nastavi druhy rozmer pole
    int rozmerProstoruPredchozi = 0;    //promenna ukladajici pocet slov/souradnic v predchozim nactenem radku
    bool wasChar = false;    //byl predchozi znak jiny nez mezera?
    std::vector<int> bodySpatnehoProstoru;    //pole pro zachyceni radku s body s jinym poctem souradnic
    bool existujiSpatneBody = false;
    bool spatnyZnak = false;

    if(zadaneBody.fail()){  //pokud takovy soubor neexistuje
        std::cout << "-> Takovy soubor jsme nenasli." << std::endl;
        existujiSpatneBody = true;
    }
    else{   //pokud se soubor nasel
        while(std::getline(zadaneBody, line)){  //zjistime kolik radku v souboru se souradnicemi je a taky kolik souradnic ma kazdy ze zadanych bodu
            pocetRidicichBodu++;
            wasChar = false;    //vynulujeme aktualni hodnotu pro dalsi pocitani
            rozmerProstoru = 0;     //vynulujeme aktualni hodnotu pro dalsi pocitani
            for(int znak = 0; znak < (int)line.length(); znak++){    //projedeme radek pres vsechny znaky
                    if(line[znak] != ' '){     //pokud se jedna o jiny znak nez mezera
                        if(std::isdigit(line[znak])==0 && line[znak] != '.' && line[znak] != '-'){   //kontrola jestli jde o znak jiny nez cislo ktery zaroven neni desetinna tecka nebo minus
                                spatnyZnak = true;
                        }
                        if(wasChar == false){   //pokud byl predchozi znak mezera
                            rozmerProstoru++;
                        }
                        wasChar = true;
                    }
                    else{       //pokud je aktualni znak mezera
                        wasChar = false;    
                    }
            }
            if(pocetRidicichBodu == 1){ //na prvnim radku zapopoulujeme promennou rozmerProstoruPredchozi
                rozmerProstoruPredchozi = rozmerProstoru;
            }
            else {  //pro kazdy dalsi radek kontrolujeme
                if(rozmerProstoru != rozmerProstoruPredchozi){  //pokud nema stejny pocet souradnic jako radek predchozi
                    bodySpatnehoProstoru.push_back(pocetRidicichBodu-1); //-1 protoze momentalne jsme na bode "dalsim", a chybne byl ten predchozi (pocetBodu pricitame nez je kontrolujeme)
                    existujiSpatneBody = true;
                    //std::cout << "Lisi se pocty souradnic bodu zadaneho okolo radku "<< pocetRidicichBodu <<", zkontroluj soubor s jejich souradnicemi" << std::endl;
                }
                //pokud ma
                rozmerProstoruPredchozi = rozmerProstoru;   //zase predame hodnotu do pomocne promenne
            }
        }
    
        rozmerProstoru = rozmerProstoruPredchozi;   //pokud je v kazdem radku stejny pocet souradnic, musi platit tohle a obejdeme reset rozmerProstoru v cyklu while

        std::string frazeBody;
        if(spatnyZnak == true){
            std::cout << "-> Textovy soubor se souradnicemi ridicich bodu obsahuje nepodporovane znaky." << std::endl;
            existujiSpatneBody = true;
        }
        else{
            if(pocetRidicichBodu == 0){ //kdyz je soubor prazdny
                std::cout << "-> Textovy soubor se souradnicemi ridicich bodu je prazdny." << std::endl;
                existujiSpatneBody = true;
            }
            else if(pocetRidicichBodu == 1){    //kdyz je v souboru jen jeden bod
                std::cout << "-> V textovem souboru se souradnicemi ridicich bodu musi byt zadany alespon 2 ridici body." << std::endl;
                existujiSpatneBody = true;
            }
            else{   //pokud je dostatecny pocet ridicich bodu, program pokracuje
                if(existujiSpatneBody == true){ //pokud byly nalezeny spatne body, tak budou informace o nich zobrazeny
                    spatneBodyMsg(bodySpatnehoProstoru);
                }
                else{   //pokud nebyly nalezeny spatne body, zobrazi se lehka zprava utechy
                    if(2 < pocetRidicichBodu && pocetRidicichBodu <= 4){    //snaha o trochu cestiny (mnozny a jednotny tvar zpravy)
                        frazeBody = "-> Vsechny " + std::to_string(pocetRidicichBodu) + " ridici body maji ";
                    }
                    if(pocetRidicichBodu > 4){
                        frazeBody = "-> Vsech " + std::to_string(pocetRidicichBodu) + " ridicich bodu ma ";
                    }
                    std::cout << frazeBody <<"stejny pocet souradnic a byly nacteny. Detekovana dimenze prostoru je " << rozmerProstoru << "."<< std::endl;
                
                    zadaneBody.clear(); //vymaze zaznam o tom ze uz jsme ze souboru cetli (https://stackoverflow.com/questions/7681555/resetting-the-end-of-file-state-of-a-ifstream-object-in-c)
                    zadaneBody.seekg(0, std::ios::beg);  //vrati nas zpet na zacatek souboru

                    std::vector<std::vector<float>> ridiciBody(pocetRidicichBodu, std::vector<float>(rozmerProstoru));  //inicializace pole na zaklade nactenych dat

                    float souradnice;   //pomocna pormenna pro hodnotu souradnice ze souboru
                    int cisloRidicihoBodu = 0, cisloSouradnice = 0;   //indexy pole

                    while(zadaneBody >> souradnice){
                        ridiciBody[cisloRidicihoBodu][cisloSouradnice] = souradnice;  //prirazeni hodnoty souradnice do odpovidajici bunky pole
                        cisloSouradnice++;    //inkrementace pres sloupce
                        if(cisloSouradnice==rozmerProstoru){      //inkrementace pres radky
                            cisloRidicihoBodu++;
                            cisloSouradnice = 0;
                        }
                    }
                    std::get<0>(outputPoleRidicichBoduZTxt) = ridiciBody;   //naskladani dat do vystupu funkce
                }
            }
        }
    }
    std::get<1>(outputPoleRidicichBoduZTxt) = existujiSpatneBody;   //naskladani dat do vystupu funkce
    return outputPoleRidicichBoduZTxt;
}

std::tuple<std::vector<float>, bool> vektorZTxt(std::string nazevSouboruVektoru){   //funkce pro nacteni hodnot vektoru z .txt souboru
    std::tuple<std::vector<float>, bool> outputVektorZTxt;  //definice vystupu funkce. pole

    std::ifstream zadanyVektor(nazevSouboruVektoru); //nacteni souboru s vektorem

    bool wasChar = false;
    std::string line;
    int pocetRadku = 0;
    int pocetHodnot = 0;
    bool spatneHodnotyVektoru = false;
    bool spatnyZnak;

    if(zadanyVektor.fail()){  //pokud takovy soubor neexistuje
        std::cout << "-> Takovy soubor jsme nenasli." << std::endl;
        spatneHodnotyVektoru = true;
    }
    else{
        while(std::getline(zadanyVektor, line)){
            pocetRadku++;
            for(int znak = 0; znak < (int)line.length(); znak++){    //projedeme radek pres vsechny znaky
                if(line[znak] != ' '){     //pokud se jedna o jiny znak nez mezera
                    if(std::isdigit(line[znak])==0 && line[znak] != '.' && line[znak] != '-'){   //kontrola jestli jde o znak jiny nez cislo ktery zaroven neni desetinna tecka
                        spatnyZnak = true;
                    }
                    if(wasChar == false){   //pokud byl predchozi znak mezera
                        pocetHodnot++;
                    }
                    wasChar = true;
                }
                else{       //pokud je aktualni znak mezera
                    wasChar = false;    
                }
            }
        }
        if(spatnyZnak == true){
            std::cout << "-> Textovy soubor s vektorem obsahuje nepodporovane znaky." << std::endl;
            spatneHodnotyVektoru = true;
        }
        else{
            if(pocetRadku == 0){ //kdyz je soubor prazdny
                std::cout << "-> Textovy soubor s vektorem je prazdny." << std::endl;
                spatneHodnotyVektoru = true;
            }
            else if(pocetRadku > 1){    //kdyz je v souboru jen jeden bod
                std::cout << "-> V textovem souboru s vektorem je vic nez jeden vektor" << std::endl;
                spatneHodnotyVektoru = true;
            }
            else{
                std::string frazeHodnoty;
                if(2 < pocetHodnot && pocetHodnot <= 4){    //snaha o trochu cestiny (mnozny a jednotny tvar zpravy)
                    frazeHodnoty = "-> Vsechny " + std::to_string(pocetHodnot) + " hodnoty byly nacteny.";
                }
                if(pocetHodnot > 4){
                    frazeHodnoty = "-> Vsech " + std::to_string(pocetHodnot) + " hodnot bylo nacteno.";
                }
                std::cout << frazeHodnoty << std::endl;

                std::vector<float> vektor;  //definice vystupu vektor

                zadanyVektor.clear(); //vymaze zaznam o tom ze uz jsme ze souboru cetli (https://stackoverflow.com/questions/7681555/resetting-the-end-of-file-state-of-a-ifstream-object-in-c)
                zadanyVektor.seekg(0, std::ios::beg);  //vrati nas zpet na zacatek souboru

                float aktualniHodnota;
                while(zadanyVektor >> aktualniHodnota){
                    vektor.push_back(aktualniHodnota);
                }
                std::get<0>(outputVektorZTxt) = vektor;   //naskladani dat do vystupu funkce
            }
        }
    }
    std::get<1>(outputVektorZTxt) = spatneHodnotyVektoru;
    return outputVektorZTxt;
}

void spatneBodyMsg(std::vector<int> seznamPredbeznychSpatnychBodu){   //funkce pro vytisteni informace o spatne zadanych bodech v textovem souboru
    std::vector<int> seznamSpatnychBodu;
    for(int poradiPredbeznehoSpatnehoBodu = 0; poradiPredbeznehoSpatnehoBodu < (int)seznamPredbeznychSpatnychBodu.size(); poradiPredbeznehoSpatnehoBodu++){ //filtr por chovani set reset
        if(poradiPredbeznehoSpatnehoBodu != (int)seznamPredbeznychSpatnychBodu.size()-1 && seznamPredbeznychSpatnychBodu[poradiPredbeznehoSpatnehoBodu+1]-seznamPredbeznychSpatnychBodu[poradiPredbeznehoSpatnehoBodu]!=0){ //pokud je dalsi bod s jinym poctem souradnic, pravdepodobne je v pohode, kdyz ten aktualni mel chybu
            seznamSpatnychBodu.push_back(seznamPredbeznychSpatnychBodu[poradiPredbeznehoSpatnehoBodu]); //pridani spatneho bodu do seznamu
            poradiPredbeznehoSpatnehoBodu++;
        }
        else{   //kdyz je chybne posledni bod ze seznamu, tak ho berem automaticky
            seznamSpatnychBodu.push_back(seznamPredbeznychSpatnychBodu[poradiPredbeznehoSpatnehoBodu]);
        }
    }
    std::string vsechnySpatneBody;
    std::string stringCarkaMezera = ", ";
    for(int poradiSpatnehoBodu = 0; poradiSpatnehoBodu < (int)seznamSpatnychBodu.size(); poradiSpatnehoBodu++){ //sestaveni stringu pro vypsani chybove zpravy
        vsechnySpatneBody += std::to_string(seznamSpatnychBodu[poradiSpatnehoBodu]);  //pripsani poradi zadaneho bodu do stringu
        if(poradiSpatnehoBodu < (int)seznamSpatnychBodu.size()-1){    //pokud se nejedna o posledni spatne zadany bod
            vsechnySpatneBody += stringCarkaMezera; //tak za nej dame carku a mezeru
        }
    }
    std::string frazeBody;
    if((int)seznamSpatnychBodu.size() > 1){ //snaha o trochu cestiny (mnozny a jednotny tvar zpravy)
        frazeBody = "ridici body " + vsechnySpatneBody + " jsou zadane ";
    }
    else {
        frazeBody = "ridici bod " + vsechnySpatneBody + " je zadan ";
    }
    std::cout << "-> Alespon " << frazeBody << "se spatnym poctem souradnic. Zkontroluj zdrojovy textovy soubor!" << std::endl;
}

std::vector<int> uzloveRoztece(std::vector<float> uzlovyVektor){  //funkce ro urceni aktivnich a pasivnich uzlovych rozteci z uzloveho vektoru
    std::vector<int> aktivniUzloveRoztece;    //definice vystupu funkce
    for(int prvek = 0; prvek < (int)uzlovyVektor.size()-1; prvek++){ //uzlovy vektor projedeme pres vsechny prvky
        if(uzlovyVektor[prvek] != uzlovyVektor[prvek+1]){ //kdyz je roztec nenulova
            aktivniUzloveRoztece.push_back(prvek);  //pridame ji do seznamu
        }
    }
    return aktivniUzloveRoztece;
}

int naJakeUzloveRozteciZrovnaJsme(std::vector<float> vektorU, std::vector<int> aktivniUzloveRoztece, float u){    //funkce najde na jeke aktivni rozteci se zrovna s parametrem u nachazime
    int aktualniRoztec = 0; //roztec na ktere se nachazi aktualni hodnota parametru u
    bool roztecNalezena = false;
    for(int roztec = 0; roztec < (int)aktivniUzloveRoztece.size(); roztec++){   //projedeme pres vsechny aktivni roztece
        if(vektorU[aktivniUzloveRoztece[roztec]] <= u && u < vektorU[aktivniUzloveRoztece[roztec]+1]){  //zkontrolujeme jestli je u na nektere z nich
            aktualniRoztec = aktivniUzloveRoztece[roztec];  //pokud jo, tak je to nase roztec
            roztecNalezena = true;
        }
        else if(roztec == (int)aktivniUzloveRoztece.size()-1 && roztecNalezena == false){    //pokud neni ani na jedny, tak to musi bejt posledni hodnota na posledni aktivni rozteci (protoze to jsme zatim netestovali)
            aktualniRoztec = aktivniUzloveRoztece[(int)aktivniUzloveRoztece.size()-1];   //tim padem vratime cislo posledni roztece
        }
    }

    return aktualniRoztec;
}

float bazoveFunkceBSplineIter(int i, int k, std::vector<float> U, float u, float NiPredchozi, float NiPlusPredchozi){   //funkce iteracne pocitajici hodnoty B-spline bazovych funkci
    const float epsilon = 1.0e-5;
    float Nik;
    float prvniZlomek;
    if(NiPredchozi + NiPlusPredchozi < epsilon){
        Nik = 0;
    }
    else{
    float prvniJmenovatel = U[i+k]-U[i];
        if(std::abs(prvniJmenovatel)<epsilon){
            prvniZlomek = 0;
        }
        else{
            prvniZlomek = (u-U[i])/prvniJmenovatel;
        }
    float druhyZlomek;
    float druhyJmenovatel = U[i+k+1]-U[i+1];
        if(std::abs(druhyJmenovatel)<epsilon){
            druhyZlomek = 0;
        }
        else{
            druhyZlomek = (U[i+k+1]-u)/druhyJmenovatel;
        }
    Nik = prvniZlomek * NiPredchozi + druhyZlomek * NiPlusPredchozi;
    }
    return Nik;
}

std::vector<float> bazoveFunkceBSpline(int aktualniRoztec, std::vector<float> U, float u, int p){  //funkce vypocitavajici hodnoty B-spline bazovych funkci
    int pocetRozteci = (int)U.size()-1;   //pocet rozteci pro aktualne nactene soubory
    std::vector<float> poleHodnotBazovychFunkci(pocetRozteci);  //pomocne pole hodnot bazovych funkci. v druhem radku stavajici iterace, nasledne prepsana do prvniho
    for(int i = 0; i < pocetRozteci; i++){  //populace pole hodnotami B-spline bazovych funkci nulte (k) iterace
        if(i == aktualniRoztec){
            poleHodnotBazovychFunkci[aktualniRoztec] = 1;
        }
        else{
            poleHodnotBazovychFunkci[i] = 0;
        }
    }
    int vsechnyRozteceIteracni = pocetRozteci-2;    //pocet rozteci = pocet uzlu-1, iterujeme pres roztece, tedy od 0 do pocet rozteci -1, a krom posledni, tedy -2
    for(int k = 1; k <= p; k++){    //dokud se nedostaneme na pozadovany stupen krivky
        for(int i = 0; i < vsechnyRozteceIteracni; i++){
            poleHodnotBazovychFunkci[i] = bazoveFunkceBSplineIter(i, k, U, u, poleHodnotBazovychFunkci[i], poleHodnotBazovychFunkci[i+1]);
        }
    }
    return poleHodnotBazovychFunkci;
}

std::vector<float> racionalniBazoveFunkce(std::vector<float> bazoveFunkceBSpline, std::vector<float> W){    //funkce pocitajici aktualni hodnoty racionalnich bazovych funkci
    int konecIteraci = W.size();
    std::vector<float> poleHodnotRacionalnichBazovychFunkci(konecIteraci);
    float N = 0;
    for(int j = 0; j < konecIteraci; j++){
        N += (bazoveFunkceBSpline[j]*W[j]);
    }
    for(int i = 0; i < konecIteraci; i++ ){
        poleHodnotRacionalnichBazovychFunkci[i] = (bazoveFunkceBSpline[i]*W[i])/N;
    }
    return poleHodnotRacionalnichBazovychFunkci;
}

std::vector<float> rceNurbsKrivky(std::vector<std::vector<float>> poleRidicichBodu,  std::vector<float> poleRacionalnichBazovychFunkci, int dimenzeProstoru){   //hlavicka funkce ktera dopocita souradnice aktualniho bodu krivky
    std::vector<float> souradniceBoduKrivky(dimenzeProstoru);
    for(int xi = 0; xi < dimenzeProstoru; xi++){
        souradniceBoduKrivky[xi] = 0;
        for(int funkce = 0; funkce < (int)poleRacionalnichBazovychFunkci.size(); funkce++){
            souradniceBoduKrivky[xi] += (poleRacionalnichBazovychFunkci[funkce]*poleRidicichBodu[funkce][xi]);
        }
    }
    return souradniceBoduKrivky;
}