#include <bits/stdc++.h>
#include <fstream>

#include "global.h"

using namespace std;


void readInstance(ifstream &f, vector<vector<int>> &yard, vector<int> &stack_position){
    
    string line;
    getline(f, line);
    
    //Esto nos permite procesar toda la linea
    istringstream instance_info(line);
    instance_info >> instance_name >> n_bays >> n_rows >> max_h >> n_initial_containers;

    //Hacemos un resize del initial yard y del stack position;
    yard.resize(n_bays*n_rows);
    stack_position.resize(n_initial_containers+1);

    //Procesamos cada linea restante
    while(getline(f, line)){
        if(line.empty()) continue;

        istringstream info(line);
        int b, r;
        info >> b >> r;
        b--;r--;

        int index = (n_rows*b) + r;

        //Metemos cada container
        int n_container;   
        info >> n_container;

 

        int id;
        for(int i = 0; i < n_container; i++){

            //Agregamos el container actual
            info >> id;
            yard[index].push_back(id);

            //Guardamos el stack en el que está el container
            stack_position[id] = index;

            //Ahora nos saltamos el repetido
            info>>id;
        }
        

    }

}

