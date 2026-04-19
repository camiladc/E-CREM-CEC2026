#include <bits/stdc++.h>

using namespace std;
#include "global.h"


//Para escribir el individuo nececitamos describir como aplica los movimientos -> aplicar las heurísticas
void writeIndDecoded(vector<vector<int>> &yard, vector<int> &stack_position, individuo &ind , ofstream &f){

    //Copiamos variables iniciales para trackear los movimientos
    vector<vector<int>> actual_yard = yard;
    vector<int> actual_positions = stack_position;
    
    //Inicializamos variables
    int id_to_retrieve = 1;
    int n_containers_actual = n_initial_containers;
    int move_actual = 0;
    int n_moves = ind.moves.size();
    
    //Hasta que no queden containers
    while(n_containers_actual > 0){
        
        int origin_stack = actual_positions[id_to_retrieve];
        int h_origin = actual_yard[origin_stack].size();

        //Si está en el tope lo sacamos
        if(id_to_retrieve == actual_yard[origin_stack][h_origin-1]){
            //Escribimos el movimiento
            f << "\n0: " << id_to_retrieve << " " << origin_stack << " -1";

            actual_yard[origin_stack].pop_back();
            actual_positions[id_to_retrieve] = -1;
            id_to_retrieve++;
            n_containers_actual--;

        }

        //Si no es así entonces aplicamos heuristica que toca
        else{

            int choosed = ind.moves[move_actual];
            int h_origin = actual_yard[origin_stack].size();
            int c2relocate = actual_yard[origin_stack][h_origin-1];
            int destiny_stack;
            //Heurística space
            if (choosed ==  1){
                destiny_stack = myopic_space(actual_yard, origin_stack);
            }
            
            //Heurística RI
            else if(choosed == 2){
                destiny_stack = RI(actual_yard, origin_stack);
            }
            //Heurística RIL
            else if(choosed == 3){
                destiny_stack = RIL(actual_yard, origin_stack);
            }
            //Heurística RI right handed
            else if(choosed == 4){
                destiny_stack = RI_R(actual_yard, origin_stack);
            }
            //Heurística RIL right handed
            else if(choosed == 5){
                destiny_stack = RIL_R(actual_yard, origin_stack);
            }

            actual_yard[origin_stack].pop_back();
            actual_yard[destiny_stack].push_back(c2relocate);
            actual_positions[c2relocate] = destiny_stack;

            //Escribimos el movimiento
            f << "\n" << choosed << ": " << c2relocate << " " << origin_stack << " " << destiny_stack; 

            //Si se acaban los movimentos de la lista y aun quedan container, repetirlos desde el principio
            //move_actual == n_moves-1? move_actual = 0 : move_actual++;
            move_actual++;
        }   
        
    }

}

void writeInd(individuo &ind, ofstream &f){
    for (int i = 0; i < ind.moves.size(); i++){
        f << ind.moves[i] << " ";
    }
    f << " F[0]=" << ind.fobj[0];
    f << " F[1]=" << ind.fobj[1];
    f << " F[2]=" << ind.fobj[2];
}   

void writePob(int n_gen_actual, vector<individuo>& pop, ofstream &f){
    f << "\nGen " << n_gen_actual;
    for(int i = 0; i < params.popsize; i++){
        f << "\nIndividuo " << i+1 << "\n";
        writeInd(pop[i], f);
    }
    f << "\n";
}

