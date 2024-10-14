//soubor pro definici zdrojoveho kodu funkci projekci
#include <iostream> //knihovna pro hrani s io
#include <vector>   //knihovna pro dynamicka pole
#include <string>   //knihovna pro praci s retezci znaku
#include <cmath>    //matika a tak

std::vector<std::vector<float>> Rx(float gamma){    //transformacni matice rotace okolo osy x pro sloupcove vektory
    std::vector<std::vector<float>> maticeRx(4, std::vector<float>(4));
    float cGamma = std::cos(gamma*M_PI/180);
    float sGamma = std::sin(gamma*M_PI/180);
    maticeRx[0][0] = 1;
    maticeRx[3][3] = 1;
    maticeRx[1][1] = cGamma;
    maticeRx[2][2] = cGamma;
    maticeRx[1][2] = -sGamma;
    maticeRx[2][1] = sGamma;
    return maticeRx;
}

std::vector<std::vector<float>> Ry(float gamma){    //transformacni matice rotace okolo osy y pro sloupcove vektory
    std::vector<std::vector<float>> maticeRy(4, std::vector<float>(4));
    float cGamma = std::cos(gamma*M_PI/180);
    float sGamma = std::sin(gamma*M_PI/180);
    maticeRy[1][1] = 1;
    maticeRy[3][3] = 1;
    maticeRy[0][0] = cGamma;
    maticeRy[2][2] = cGamma;
    maticeRy[2][0] = -sGamma;
    maticeRy[0][2] = sGamma;
    return maticeRy;
}

std::vector<std::vector<float>> Rz(float gamma){    //transformacni matice rotace okolo osy z pro sloupcove vektory
    std::vector<std::vector<float>> maticeRz(4, std::vector<float>(4));
    float cGamma = std::cos(gamma*M_PI/180);
    float sGamma = std::sin(gamma*M_PI/180);
    maticeRz[2][2] = 1;
    maticeRz[3][3] = 1;
    maticeRz[0][0] = cGamma;
    maticeRz[1][1] = cGamma;
    maticeRz[0][1] = -sGamma;
    maticeRz[1][0] = sGamma;
    return maticeRz;
}

std::vector<std::vector<float>> Tx(float dist){    //transformacni matice posunu podel osy x pro sloupcove vektory
    std::vector<std::vector<float>> maticeTx(4, std::vector<float>(4));
    maticeTx[0][0] = 1;
    maticeTx[1][1] = 1;
    maticeTx[2][2] = 1;
    maticeTx[3][3] = 1;
    maticeTx[0][3] = dist;
    return maticeTx;
}

std::vector<std::vector<float>> Ty(float dist){    //transformacni matice posunu podel osy y pro sloupcove vektory
    std::vector<std::vector<float>> maticeTy(4, std::vector<float>(4));
    maticeTy[0][0] = 1;
    maticeTy[1][1] = 1;
    maticeTy[2][2] = 1;
    maticeTy[3][3] = 1;
    maticeTy[1][3] = dist;
    return maticeTy;
}

std::vector<std::vector<float>> Tz(float dist){    //transformacni matice posunu podel osy z pro sloupcove vektory
    std::vector<std::vector<float>> maticeTz(4, std::vector<float>(4));
    maticeTz[0][0] = 1;
    maticeTz[1][1] = 1;
    maticeTz[2][2] = 1;
    maticeTz[3][3] = 1;
    maticeTz[2][3] = dist;
    return maticeTz;
}

std::vector<std::vector<float>> matMult(std::vector<std::vector<float>> A, std::vector<std::vector<float>> B){  //funkce pro nasobeni dvou matic
    std::vector<std::vector<float>> C((int)A.size(), std::vector<float>((int)B[0].size()));
    for(int i = 0; i<(int)A.size(); i++){
        for(int j = 0; j<(int)B[0].size(); j++){
            C[i][j] = 0;
            for(int k = 0; k<(int)B.size(); k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }  
    }
    return C;
}

std::vector<float>  vecMatMult(std::vector<std::vector<float>> A, std::vector<float> b){    //funkce pro nasobeni matice a sloupcoveho vektoru
    int bRozmer = (int)b.size();
    std::vector<float> x(bRozmer,0);
    for(int i = 0; i<bRozmer; i++){
        for(int j = 0; j<bRozmer; j++){
            x[i] += A[i][j] * b[j];
        }
    }
    float w = x[bRozmer-1];
    if(w != 1){
        for(int i = 0; i<bRozmer; i++){
            x[i] = x[i]/w;
        }
    }
    return x;
}

std::vector<std::vector<float>> maticeWorldToCamera(float x, float y, float z, float gammaY, float gammaZ){   //transformacni matice do souradnic kamery
    std::vector<std::vector<float>> transformacniMatice;
    transformacniMatice = matMult(Rz(-90), Ry(-90));      //efektivne porhozeni os x = z a y = x a z = y
    transformacniMatice = matMult(transformacniMatice, Ry(-gammaY));
    transformacniMatice = matMult(transformacniMatice, Rz(-gammaZ));
    transformacniMatice = matMult(transformacniMatice, Tz(-z));
    transformacniMatice = matMult(transformacniMatice, Ty(-y));
    transformacniMatice = matMult(transformacniMatice, Tx(-x));
    return transformacniMatice;
}

std::vector<std::vector<float>> maticeStredovehoPromitani(float zornyUhel, float farPlane, float nearPlane){   //transformacni matice stredove projekce
    std::vector<std::vector<float>> projekcniMatice(4, std::vector<float>(4, 0));
    float skalovani = 1/std::tan(zornyUhel*0.5*M_PI/180);
    projekcniMatice[0][0] = skalovani;  //defakto zoom na x
    projekcniMatice[1][1] = skalovani;  //defakto zoom na y
    projekcniMatice[3][2] = -1;
    projekcniMatice[2][2] = -farPlane/(farPlane-nearPlane); //normovani vzdalenosti bodu do [0, 1]
    projekcniMatice[2][3] = -(farPlane*nearPlane)/(farPlane-nearPlane); //normovani vzdalenosti bodu do [0, 1]
    return projekcniMatice;
}

std::vector<float>  promitnuti(std::vector<std::vector<float>> maticeTransformaceDoKamery, std::vector<std::vector<float>> maticePromitani, std::vector<float> promitanyBod){    //funkce promitajici bod
    std::vector<float> bodKamera = vecMatMult(maticeTransformaceDoKamery, promitanyBod);   //transformace do souradneho systemu kamery
    std::vector<float> bodSenzor = vecMatMult(maticePromitani, bodKamera);   //promitnuti na senzor kamery
    return bodSenzor;
}