#include<bits/stdc++.h>

#include "global.h"

using namespace std;


individuo initialize_ind(vector<vector<int>> &initial_yard, vector<int>& stack_position){


    individuo new_ind;
    new_ind.moves = {};
    new_ind.fobj[0] = 0.0;
    new_ind.fobj[1] = 0.0;
    new_ind.fobj[2] = 0.0;
        
    //Copiamos variables iniciales para trackear los movimientos
    vector<vector<int>> actual_yard = initial_yard;
    vector<int> actual_positions = stack_position; 

    //Creamos movimientos hasta que ya no queden containers
    int id_to_retrieve = 1;
    int n_containers_actual = n_initial_containers;
    while(n_containers_actual > 0){

        new_ind.fobj[0]++;

        int origin_stack = actual_positions[id_to_retrieve];
        int h_origin = actual_yard[origin_stack].size();

        //Si está en el tope lo sacamos y no guardamos ninguna heurística
        if(id_to_retrieve == actual_yard[origin_stack][h_origin-1]){
            actual_yard[origin_stack].pop_back();
            actual_positions[id_to_retrieve] = -1;
            id_to_retrieve++;
            n_containers_actual--;
        }

        //Si no es así entonces tenemos que decidir que heurística usar y guardarla en la lista
        else{
            int coded_heuristic = apply_random_heuristic(actual_yard, actual_positions, origin_stack);
            new_ind.moves.push_back(coded_heuristic);
        }

    }
    
    //Retornamos el individuo creado
    return new_ind;

}

//Need to ad a initialize_pop
vector<individuo> initialize_pop(vector<vector<int>> &initial_yard, vector<int> &stack_positions){
    
    vector<individuo> pop = {};
    for(int i = 0; i < params.popsize; i++){
        //Copiamos el yard para inicializarl a cada individuo
        individuo new_ind = initialize_ind(initial_yard, stack_positions);
        pop.push_back(new_ind);

    }

    return pop;
}