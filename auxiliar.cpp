#include <bits/stdc++.h>

using namespace std;
#include "global.h"


void printYard(vector<vector<int>> &yard){

    int bay = 0;
    for(int i = 0; i < n_bays*n_rows; i++){

        //Verificamos si cambiamos de bahía 
        if(i%n_rows == 0){
            cout << "\nBay " << bay << "\n";
            bay++;
        }

        //Mostramos el stack actual 
        int size = yard[i].size();
        for(int j = 0; j < size; j++){
            cout << yard[i][j] << " ";
        }
        cout << "\n";
    }

}

void printInd(individuo &ind){

    for (int i = 0; i < ind.moves.size(); i++){
        printf("%d ", ind.moves[i]);
    }
    printf("\nF[0]= %f ", ind.fobj[0]);
    printf("\nF[1]= %f ", ind.fobj[1]);
    printf("\nF[2]= %f ", ind.fobj[2]);
    
}

bool desc_sort(pair<int, int>& a,  pair<int, int>& b) {
    return a.first > b.first;  
}

