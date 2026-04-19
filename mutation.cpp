#include<bits/stdc++.h>

#include "global.h"

using namespace std;

void swap(individuo &ind){// De toda la vida

    //Swap multiple
    
    int moves_size = ind.moves.size();
    for (int i = 0; i < moves_size; i++)
    {
        double p = getRandomProb();
        if(p <= params.p_swap){
            int a = i;
            int b = getRandomInt(0,moves_size-1);
            int aux = ind.moves[a];
            ind.moves[a] = ind.moves[b];
            ind.moves[b] = aux;
        }
    }

}
 
void inversion(individuo &ind){// Entre dos puntos random

    int moves_size = ind.moves.size();
  //  double p = getRandomProb();

    //if(p <= params.pmut_inversion*moves_size){ -> creo que esto no debería estar

        int a = getRandomInt(0,moves_size-1);
        int b = getRandomInt(0,moves_size-1);

        //"a" debe ser menor o igual a b
        if(a>b){int aux = a; a = b; b = aux;}

        //Invertimos sección entre a y b incluidos
        vector<int> section;
        for (int i = a; i < b+1; i++)
        {
            section.push_back(ind.moves[i]);
        }
        for (int i = a; i < b+1; i++)
        {
            ind.moves[i] = section[b-i];
        }
   // }
      
}

void intFlip(individuo &ind){// De toda la vida

    int moves_size = ind.moves.size();

    for (int i = 0; i < moves_size; i++)
    {
        double p = getRandomProb();
        if(p <= params.p_intFlip){
            int j = getRandomInt(0,moves_size-1);
            ind.moves[i] = getRandomInt(1,params.n_heu);
        }
    }
}
