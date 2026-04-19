#include<bits/stdc++.h>

#include "global.h"

using namespace std;

/* -------------- Estas funciones retornan directamente el mejor valor ----------------*/

//Función miope de espacio que cambia al stack con más espacio (a la izquierda)
int myopic_space(vector<vector<int>> &yard, int origin_stack){
    //Recorremos todo el yard y vamos calculando
    int max_space = 0;
    int choosed_stack = 0;

    for(int i = 0; i < n_bays*n_rows; i++){

        if(i == origin_stack){
            continue;
        }

        //Verificamos si es mejor que lo que ya hemos encontrado
        int value = max_h - yard[i].size();
        if(value > max_space){
            max_space = value;
            choosed_stack = i;
        }
    }

    //Retornamos el mejor stack para moverse
    return choosed_stack;
}


int RI(vector<vector<int>> &yard, int origin_stack){

    //Recorremos todo el yard y vamos calculando
    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    int choosed_stack = 0;
    int min_ri = yard[0].size(); // Esta es la única diferencia con myopic_blocked, no debería importar (creo)

    for(int i = 0; i < n_bays*n_rows; i++){
        int ri_stack = 0;

        if(i == origin_stack){
            continue;
        }

        if(max_h == yard[i].size()) //Stack lleno
            continue;

        int size = yard[i].size();
        for(int j = 0; j < size; j++){
            if(yard[i][j] < c2relocate)
                ri_stack++;
        }

        //Verificamos si es mejor que lo que ya hemos encontrado
        if(min_ri > ri_stack){
            min_ri = ri_stack;
            choosed_stack = i;
        }
    }

    //Retornamos el mejor stack para moverse
    return choosed_stack;
}

int RIL(vector<vector<int>> &yard, int origin_stack){

    //Recorremos todo el yard y vamos calculando
    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    int choosed_stack = 0;
    int choosed_container = 0;
    int min_ri = yard[0].size();

    for(int i = 0; i < n_bays*n_rows; i++){
        int ri_stack = 0;
        int highest_c = 0;


        if(i == origin_stack){
            continue;
        }

        if(max_h == yard[i].size()) //Stack lleno
            continue;

        //if(yard[i].size() == 0) //Forzar elegir stack vacio
        //    return i;

        int size = yard[i].size();
        for(int j = 0; j < size; j++){
            if(yard[i][j] < c2relocate){
                ri_stack++;
                if(yard[i][j] > highest_c)
                    highest_c = yard[i][j];
            }
        }

        //Verificamos si es mejor que lo que ya hemos encontrado
        if(min_ri > ri_stack){
            min_ri = ri_stack;
            choosed_stack = i;
            choosed_container = highest_c;
        }

        else if(min_ri == ri_stack){
            if(highest_c > choosed_container){
                choosed_stack = i;
                choosed_container = highest_c;
            }
        }
    }

    //Retornamos el mejor stack para moverse
    return choosed_stack;
}

/*---------------- Right handed heuristics ---------------- */

int RI_R(vector<vector<int>> &yard, int origin_stack){


    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    int choosed_stack = 0;
    int min_ri = yard[0].size();

    //Recorremos de derecha a izquierda
    for(int i = n_bays*n_rows-1; i >= 0; i--){
        int ri_stack = 0;

        if(i == origin_stack){
            continue;
        }

        if(max_h == yard[i].size()) 
            continue;

        int size = yard[i].size();
        for(int j = 0; j < size; j++){
            if(yard[i][j] < c2relocate)
                ri_stack++;
        }

   
        if(min_ri > ri_stack){
            min_ri = ri_stack;
            choosed_stack = i;
        }
    }

    //Retornamos el mejor stack para moverse
    return choosed_stack;
}


int RIL_R(vector<vector<int>> &yard, int origin_stack){

    //Recorremos todo el yard y vamos calculando
    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    int choosed_stack = 0;
    int choosed_container = 0;
    int min_ri = yard[0].size();

    //Recorremos de derecha a izquierda
    for(int i = n_bays*n_rows-1; i >= 0; i--){
        int ri_stack = 0;
        int highest_c = 0;


        if(i == origin_stack){
            continue;
        }

        if(max_h == yard[i].size()) 
            continue;

        int size = yard[i].size();
        for(int j = 0; j < size; j++){
            if(yard[i][j] < c2relocate){
                ri_stack++;
                if(yard[i][j] > highest_c)
                    highest_c = yard[i][j];
            }
        }

        if(min_ri > ri_stack){
            min_ri = ri_stack;
            choosed_stack = i;
            choosed_container = highest_c;
        }

        else if(min_ri == ri_stack){
            if(highest_c > choosed_container){
                choosed_stack = i;
                choosed_container = highest_c;
            }
        }
    }

    //Retornamos el mejor stack para moverse
    return choosed_stack;
}


/* ---------------- FUNCIONES MIOPE REBELDES  ----------------*/

//Función miope inversa de espacio que cambia al stack con menos espacio
int myopic_min_space(vector<vector<int>> &yard, int origin_stack){

    //Recorremos todo el yard y vamos calculando
    int min_space = max_h + 1;
    int choosed_stack = -1;

    for(int i = 0; i < n_bays*n_rows; i++){

        if(i == origin_stack || yard[i].size() >= max_h){
            continue;
        }

        //Verificamos si es peor que lo que ya hemos encontrado
        int value = max_h - yard[i].size();
        if(value < min_space){
            min_space = value;
            choosed_stack = i;
        }
    }

    //Retornamos el peor stack para moverse
    return choosed_stack;
}

//Funcion miope inversa que cambia al stack con mas bloqueos
int RI_inverse(vector<vector<int>> &yard, int origin_stack){

    //Recorremos todo el yard y vamos calculando
    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    int choosed_stack = 0;
    int max_ri = -1;

    for(int i = 0; i < n_bays*n_rows; i++){
        int ri_stack = 0;

        if(i == origin_stack){
            continue;
        }

        if(max_h == yard[i].size()) //Stack lleno
            continue;

        int size = yard[i].size();
        for(int j = 0; j < size; j++){
            if(yard[i][j] < c2relocate)
                ri_stack++;
        }

        //Verificamos si es peor que lo que ya hemos encontrado
        if(ri_stack > max_ri){
            max_ri = ri_stack;
            choosed_stack = i;
        }
    }

    //Retornamos el peor stack para moverse
    return choosed_stack;
}


/* -------------- HEURÍSTICAS TOP_STACK ----------------*/
/* 
Estas heurísticas solo se fijarán en el contenedor que está en el tope del stack
No preferiran los stacks vacíos pues myopic space ya hace eso

*/

//Eligirá el stack que tienene el container con id mas cercano mayor que el actual 
int top_diff(vector<vector<int>> &yard, int origin_stack){

    int destiny_stack = -1;
    int c2relocate = yard[origin_stack][yard[origin_stack].size()-1];

    int selected_stack = -1;
    
    int min_diff_up = 10e6;   //Calculamos la mínima diferencia que hay si es que el valor es mayor que el container actual
    int selected_stack_up = -1;

    int min_diff_down = 10e6; //Calculamos la mínima diferencia que hay si es que el valor es menor que el container actual
    int selected_stack_down = -1;

    for(int i = 0; i < n_bays*n_rows; i++){
        
        //Nos saltamos el container actual
        if(i == origin_stack) continue;

        //Stack lleno
        if(yard[i].size() >= max_h) 
            continue;

        //Calculamos
        int size = yard[i].size();
        if(size == 0) continue; //ignora los contenedores vacíos
        int id_actual = yard[i][size-1];

        //Vamos si es que el id es mayor al c2relocate
        if(id_actual > c2relocate){
            int value = id_actual - c2relocate;

            if (value < min_diff_up) {
                min_diff_up = value;
                selected_stack_up = i;
            }
        }

        //Ahora el c2relocate es mayor que el id
        else if(c2relocate > id_actual){
            int value = c2relocate - id_actual;

            if(value < min_diff_down){
                min_diff_down = value;
                selected_stack_down = i;
            }
        }

    }

    //Seleccionamos el más cercano que sea mayor a nosotros
    selected_stack = selected_stack_up;

    //Si es que no hay ninguno que sea mayor a c2relocate elegimos el mas cercano que sea menor
    if(selected_stack == -1){
        selected_stack = selected_stack_down;
    }

    //Si es que sigue sin elegir ninguno es que todos están vacíos
    if(selected_stack == -1){
        if(origin_stack == 0){
            selected_stack = 1;
        }
        else{
            selected_stack = 0;
        }
    }

    return selected_stack;
}



//Función para elegir una de las funciones miopes disponibles de forma random
//La idea es que le aplique una heurística al yard y retorne la heurística que utilizó
int apply_random_heuristic(vector<vector<int>> &yard, vector<int> &stack_position, int origin_stack){

    //Obtenemos un número aleatorio para elegir la heurística
    //int n_heuristic = 5;
    int choosed = getRandomInt(1, params.n_heu);
    int destiny_stack;

    //Heurística space
    if (choosed == 1){
        destiny_stack = myopic_space(yard, origin_stack);
    }
    //Heurística RI
    else if(choosed == 2){
        destiny_stack = RI(yard, origin_stack);
    }
    //Heurística RIL
    else if(choosed == 3){
        destiny_stack = RIL(yard, origin_stack);
    }

    //Heurística RI right handed
    else if(choosed == 4){
        destiny_stack = RI_R(yard, origin_stack);
    }
   
    //Heurística RIL right handed
    else if(choosed == 5){
        destiny_stack = RIL_R(yard, origin_stack);
    }
    
    //Heuristica top_diff
    else if(choosed == 6){
        destiny_stack = top_diff(yard, origin_stack);
    }

    //Heurística rebelde de espacio
    else if(choosed == 7){
        destiny_stack = myopic_min_space(yard, origin_stack);
    }

    //Heurística rebelde de RI
    else if(choosed == 8){
        destiny_stack = RI_inverse(yard, origin_stack);
    }


    //Realizamos el movimiento
    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    yard[origin_stack].pop_back();
    yard[destiny_stack].push_back(c2relocate);
    stack_position[c2relocate] = destiny_stack;


    return choosed;
}