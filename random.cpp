#include<bits/stdc++.h>
#include<random>

#include "global.h"

using namespace std;

//Función que asigna semilla al generador aleatoreos
void randomize(int seed){
    rng.seed(seed);
}

//Función que retorna un numero entero entre a y b incluyentes
int getRandomInt(int a, int b) {
 //   cout << "VOY A LLORAR " << endl;
    uniform_int_distribution<int> dist(a, b);
//    cout << "NO llore" << endl;
    return dist(rng);
}

//Funcion que retorna un numero real entre 0 y 1
float getRandomProb(){
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
}

//Funcion que retorna un numero real entre 0 y lim
float getRandomProb_lim(float lim){
    uniform_real_distribution<float> dist(0.0f, lim);
    return dist(rng);
}