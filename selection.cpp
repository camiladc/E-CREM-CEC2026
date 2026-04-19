#include<bits/stdc++.h>

#include "global.h"

using namespace std;

//Función de comparación para ordenar en orden ascendente por fobj[2] (función objetivo combinada)
bool compararPorFobjAsc(const individuo &a, individuo &b) {
    return a.fobj[2] < b.fobj[2];
}

//Pa un objetivo q me da lata
individuo tournament(individuo &ind1, individuo &ind2){

    int fo1; //Ganador para fo1 
    if(ind1.fobj[2] < ind2.fobj[2]) return ind1;
    else if(ind2.fobj[2] < ind1.fobj[2]) return ind2;
    else{
        double p = getRandomProb();
        if(p <= 0.5) return ind1;
        else return ind2;
    }

}

// Esta es una versión usando el a1 y a2 para que se repitan menos los individuos que van a torneo.
void generateNewPop(vector<individuo> &old_pop){

    vector<int> a1(params.popsize+1), a2(params.popsize+1);
    int temp;
    int rand;
    individuo parent1, parent2;
    vector<individuo> new_pop(params.popsize), children;

    double p;
    sort(old_pop.begin(), old_pop.end(), compararPorFobjAsc);

    //Obtener elite de poblacion padre y pasarla directamente a la nueva poblacion
    for (int i = 0; i < params.elite; i++){
        new_pop[i] = old_pop[i];
    }

    //Preparar torneos
    for (int i=0; i<params.popsize; i++)
    {
        a1[i] = a2[i] = i;
    }
    for (int i=0; i<params.popsize; i++)
    {
        rand = getRandomInt(i, params.popsize-1);
        temp = a1[rand];
        a1[rand] = a1[i];
        a1[i] = temp;
        rand = getRandomInt(i, params.popsize-1);
        temp = a2[rand];
        a2[rand] = a2[i];
        a2[i] = temp;
    }
    a1[params.popsize] = a1[0]; //Esto es para el caso cuando c=19 y se hace cruzamiento.
    a2[params.popsize] = a2[0]; //Realmente no se usa el a1[0] a menos que no haya elitismo, quizas se podría hacer un numero al azar en ese caso.

    for (int c = params.elite; c < params.popsize; c++){
        parent1 = tournament(old_pop[a1[c]], old_pop[a1[c+1]]);

        p = getRandomProb();
        if(p<=params.pcross){
            parent2 = tournament(old_pop[a2[c]], old_pop[a2[c+1]]);

            float cross = getRandomProb();
            if(cross <= params.pcross_one){
                children = opc(parent1, parent2);
            }
            else{
                children = tpc(parent1, parent2);
            }

            //En caso que no alcancen alguno de los dos hijos, se elige uno aleatoreamente
            if(c == params.popsize-1){
                float cain = getRandomProb();
                if(cain <= 0.5){
                    new_pop[c] = children[0];
                }
                else{
                    new_pop[c] = children[1];
                }
            }
            else{
                new_pop[c] = children[0]; new_pop[c+1] = children[1];
            }
            c++;
        }
        else if(p<=(params.pcross+params.pmut)){

            //Asignemos probabilidades para cada mutación
            float mut = getRandomProb();
            if(mut < params.pmut_swap){
                swap(parent1);
            }
            else if(mut < params.pmut_swap + params.pmut_inversion){
                inversion(parent1);
            }
            else{
                intFlip(parent1);
            }
        }
        else
            new_pop[c] = parent1;
    }
    old_pop = new_pop;
}