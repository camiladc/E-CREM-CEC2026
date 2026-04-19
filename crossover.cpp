#include<bits/stdc++.h>

#include "global.h"

using namespace std;

//Cruzamiento en un punto de toda la vida
vector<individuo> one_point_crossover(individuo padre1, individuo padre2){

    //Obtenemos punto de corte
    int p1_size = padre1.moves.size();
    int p2_size = padre2.moves.size();
    int max_size = max(p1_size,p2_size);
    int cut = getRandomInt(0,min(p1_size,p2_size)-1);

    //Inicializamos hijos
    individuo hijo1, hijo2;
    hijo1.moves = {}; hijo2.moves = {};
    hijo1.fobj[0] = 0.0; hijo2.fobj[0] = 0.0;
    hijo1.fobj[1] = 0.0; hijo2.fobj[1] = 0.0;
    hijo1.fobj[2] = 0.0; hijo2.fobj[2] = 0.0;

    //Copiamos primera mitad
    for (int i = 0; i < cut; i++)
    {
        hijo1.moves.push_back(padre1.moves[i]);
        hijo2.moves.push_back(padre2.moves[i]);
    }
    //Copiamos segunda mitad
    for (int i = cut; i < max_size; i++)
    {
        if(i < p2_size){
            hijo1.moves.push_back(padre2.moves[i]);
        }
        if(i < p1_size){
            hijo2.moves.push_back(padre1.moves[i]);
        }
    }
    
    return {hijo1, hijo2};
    
}

//Cruzamiento en un punto optimizado
vector<individuo> opc(individuo &padre1, individuo &padre2){

    //Obtenemos punto de corte
    int p1_size = padre1.moves.size();
    int p2_size = padre2.moves.size();
    int max_size = max(p1_size,p2_size);
    int cut = getRandomInt(0,min(p1_size,p2_size)-1);

    //Copiamos primera mitad
    for (int i = 0; i < cut; i++)
    {
        swap(padre1.moves[i], padre2.moves[i]);
    }

    return {padre1, padre2};
}

//Cruzamiento en dos puntos de toda la vida
vector<individuo> two_point_crossover(individuo padre1, individuo padre2){

    //Obtenemos puntos de corte
    int p1_size = padre1.moves.size();
    int p2_size = padre2.moves.size();
    int max_size = max(p1_size,p2_size);
    int cut1 = getRandomInt(0,min(p1_size,p2_size)-1);
    int cut2 = getRandomInt(0,min(p1_size,p2_size)-1);

    if (cut1 > cut2){
        int temp = cut1;
        cut1 = cut2;
        cut2 = temp;
    }

    //Inicializamos hijos
    individuo hijo1, hijo2;
    hijo1.moves = {}; hijo2.moves = {};
    hijo1.fobj[0] = 0.0; hijo2.fobj[0] = 0.0;
    hijo1.fobj[1] = 0.0; hijo2.fobj[1] = 0.0;
    hijo1.fobj[2] = 0.0; hijo2.fobj[2] = 0.0;

    //Copiamos parte inicial
    for (int i = 0; i < cut1; i++)
    {
        hijo1.moves.push_back(padre1.moves[i]);
        hijo2.moves.push_back(padre2.moves[i]);
    }
    //Copiamos parte del medio
    for (int i = cut1; i < cut2; i++)
    {
        hijo1.moves.push_back(padre2.moves[i]);
        hijo2.moves.push_back(padre1.moves[i]);
    }
    //Copiamos parte final
    for (int i = cut2; i < max_size; i++)
    {
        
        if(i < p1_size){
            hijo1.moves.push_back(padre1.moves[i]);
        }
        if(i < p2_size){
            hijo2.moves.push_back(padre2.moves[i]);
        }
    }

    return {hijo1, hijo2};
}

//Cruzamiento en dos puntos optimizado
vector<individuo> tpc(individuo &padre1, individuo &padre2){

    //Obtenemos puntos de corte
    int p1_size = padre1.moves.size();
    int p2_size = padre2.moves.size();
    int max_size = max(p1_size,p2_size);
    int cut1 = getRandomInt(0,min(p1_size,p2_size)-1);
    int cut2 = getRandomInt(0,min(p1_size,p2_size)-1);

    if (cut1 > cut2){
        int temp = cut1;
        cut1 = cut2;
        cut2 = temp;
    }

    //Copiamos parte del medio
    for (int i = cut1; i < cut2; i++)
    {
        swap(padre1.moves[i], padre2.moves[i]);
    }

    return {padre1, padre2};
}