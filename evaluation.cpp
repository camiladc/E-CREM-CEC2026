#include<bits/stdc++.h>

#include "global.h"

using namespace std;

void apply_chosen_heuristic(vector<vector<int>> &yard, vector<int> &stack_position, int origin_stack, int choosed){
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
    
    //Heurística top_diff
    else if(choosed == 6){
        destiny_stack = top_diff(yard, origin_stack);
    }

    //Heurística rebelde de espacio
    else if(choosed == 7){
        destiny_stack = myopic_min_space(yard, origin_stack);
    }

    //Heurística rebelde RI
    else if(choosed == 8){
        destiny_stack = RI_inverse(yard, origin_stack);
    }

    //Realizamos el movimiento
    //cout << "DESTINY: " << destiny_stack << endl;
    int h_origin = yard[origin_stack].size();
    int c2relocate = yard[origin_stack][h_origin-1];
    yard[origin_stack].pop_back();
    yard[destiny_stack].push_back(c2relocate);
    stack_position[c2relocate] = destiny_stack;

}


void evaluateInd(individuo &ind, vector<vector<int>> &initial_yard, vector<int> &stack_position){

    ind.fobj[0] = 0.0; ind.fobj[1] = 0.0; ind.fobj[2] = 0.0;

    //Copiamos variables iniciales para trackear los movimientos
    vector<vector<int>> actual_yard = initial_yard;
    vector<int> actual_positions = stack_position;

    //Inicializamos variables
    int id_to_retrieve = 1;
    int n_containers_actual = n_initial_containers;
    int move_actual = 0;
    int n_moves = ind.moves.size();

    //Hasta que no queden containers
    while(n_containers_actual > 0){

        ind.fobj[0]++;
        int origin_stack = actual_positions[id_to_retrieve];
        int h_origin = actual_yard[origin_stack].size();

        //Si está en el tope lo sacamos
        if(id_to_retrieve == actual_yard[origin_stack][h_origin-1]){
            actual_yard[origin_stack].pop_back();
            actual_positions[id_to_retrieve] = -1;
            id_to_retrieve++;
            n_containers_actual--;
        }

        //Si no es así entonces aplicamos heuristica que toca
        else{

            //Si aun quedan movimientos en la lista, aplicarlos
            if(move_actual < n_moves){
                apply_chosen_heuristic(actual_yard, actual_positions, origin_stack, ind.moves[move_actual]);
                move_actual++;
            }
            //Si no quedan, agregar nuevos movimientos de forma aleatoria
            else{
                int coded_heuristic = apply_random_heuristic(actual_yard, actual_positions, origin_stack);
                ind.moves.push_back(coded_heuristic);
            }
            
            //Si se acaban los movimentos de la lista y aun quedan container, repetirlos desde el principio
            //move_actual == n_moves-1? move_actual = 0 : move_actual++;
        }   
    }

    //Si no se utilizaron todos los movimientos, eliminar los sobrantes
    while(move_actual < n_moves){
        ind.moves.pop_back();
        move_actual++;
    }
}

void evaluatePop(vector<individuo> &pop, vector<vector<int>> &initial_yard, vector<int> &stack_position){

    for (int i = 0; i < params.popsize; i++)
    {
        //evaluateInd(pop[i], initial_yard, stack_position);
        evaluateIndWithTime(pop[i], initial_yard, stack_position);
    }
    
}

void evaluateIndWithTime(individuo &ind, vector<vector<int>> &initial_yard, vector<int> &stack_position){

    ind.fobj[0] = 0.0; ind.fobj[1] = 0.0; ind.fobj[2] = 0.0;

    //Copiamos variables iniciales para trackear los movimientos
    vector<vector<int>> actual_yard = initial_yard;
    vector<int> actual_positions = stack_position;

    //Inicializamos variables
    int id_to_retrieve = 1;
    int id_to_relocate;
    int n_containers_actual = n_initial_containers;
    int move_actual = 0;
    int n_moves = ind.moves.size();

    // Parameters of the time comsumption
    float gantry_base = 40.0; // grantry time base to move
    float trolley_base = 30.0; // pick up and drop off time
    float gantry_move = 3.5; // time per bay movement
    float trolley_move = 1.2; // time per stack movement

    int crane_init_stack;
    int crane_final_stack;
    int crane_distance_stack;
    int crane_init_bay = 0;
    int crane_final_bay;
    int crane_distance_bay;

    bool flag = true;

    //Hasta que no queden containers
    while(n_containers_actual > 0){

        ind.fobj[0]++;
        int origin_stack = actual_positions[id_to_retrieve];
        int h_origin = actual_yard[origin_stack].size();

        //cout << " move - " << ind.fobj[0] << endl;

        // Crane time repositioning movement
        if (flag == true) // First movement of the crane
        {
            // Special placement for the initial stowage position of the crane from stack -1 to stack 0 of each bay
            crane_init_stack = crane_init_bay*n_rows;
            ind.fobj[1] += trolley_move; // Time for getting into the yard from stack -1
            flag = false;

        }else // Continue with the rest movements
        {
            // Reposition of the crane from previous position to new origin position
            crane_init_stack = crane_final_stack; // Previous position
        }
        crane_init_bay = floor(crane_init_stack/n_rows);

        // New origin position
        crane_final_stack = origin_stack; 
        crane_final_bay = floor(crane_final_stack/n_rows);

        crane_distance_bay = abs(crane_final_bay - crane_init_bay);
        crane_distance_stack = abs(crane_init_stack - crane_final_stack + n_rows*(crane_final_bay - crane_init_bay));

        //cout << crane_distance_bay << " - " << crane_distance_stack << endl;

        // Reposition movement without container (no trolley base time)
        if (crane_distance_bay == 0 && crane_distance_stack > 0) // Movement in the same bay (only trolley movement)
        {
            ind.fobj[1] += crane_distance_stack*trolley_move;

        }else if(crane_distance_bay > 0 && crane_distance_stack == 0) // Movement in the same stack (only grantry movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move;

        }else if (crane_distance_bay > 0 && crane_distance_stack > 0) // Movement in diferente bay and stack (gantry and trolley movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move + crane_distance_stack*trolley_move;
        }

        //cout << ind.fobj[1] << endl; 

        // Crane time for associated movement (Relocation or Extraction)
        crane_init_stack = origin_stack; // Set stack origin position
        crane_init_bay = floor(crane_init_stack/n_rows); // Set bay origin position

        // Extraction
        //Si está en el tope lo sacamos
        if(id_to_retrieve == actual_yard[origin_stack][h_origin-1]){
            actual_yard[origin_stack].pop_back();
            actual_positions[id_to_retrieve] = -1;
            id_to_retrieve++;
            n_containers_actual--;

            ind.fobj[1] += trolley_move; // Time for getting out of the yard to stack -1
            flag = true; // Next movement will need to add repositioning into the yard

            crane_final_stack = crane_init_bay*n_rows; // Set stack destination to start of each bay
            crane_final_bay = crane_init_bay; // Set bay destination position as the same
        }

        // Relocation
        //Si no es así entonces aplicamos heuristica que toca
        else{
            id_to_relocate = actual_yard[origin_stack][h_origin-1];
            //Si aun quedan movimientos en la lista, aplicarlos
            if(move_actual < n_moves){
                apply_chosen_heuristic(actual_yard, actual_positions, origin_stack, ind.moves[move_actual]);
                move_actual++;
            }
            //Si no quedan, agregar nuevos movimientos de forma aleatoria
            else{
                int coded_heuristic = apply_random_heuristic(actual_yard, actual_positions, origin_stack);
                ind.moves.push_back(coded_heuristic);
            }
            crane_final_stack = actual_positions[id_to_relocate]; // Set stack destination position
            crane_final_bay = floor(crane_final_stack/n_rows); // Set bay destination position
        } 

        crane_distance_bay = abs(crane_final_bay - crane_init_bay);
        crane_distance_stack = abs(crane_init_stack - crane_final_stack + n_rows*(crane_final_bay - crane_init_bay));
        //cout << crane_distance_bay << " - " << crane_distance_stack << endl;

        // Movement with loaded container (trolley base time included)
        if (crane_distance_bay == 0 && crane_distance_stack > 0) // Movement in the same bay (only trolley movement)
        {
            ind.fobj[1] += crane_distance_stack*trolley_move;

        }else if(crane_distance_bay > 0 && crane_distance_stack == 0) // Movement in the same stack (only grantry movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move;

        }else if (crane_distance_bay > 0 && crane_distance_stack > 0) // Movement in diferente bay and stack (gantry and trolley movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move + crane_distance_stack*trolley_move;
        
        }

        // Container movement
        ind.fobj[1] += trolley_base;
        //cout << " acc time - " << ind.fobj[1] << endl;      
  
    }

    //Si no se utilizaron todos los movimientos, eliminar los sobrantes
    while(move_actual < n_moves){
        ind.moves.pop_back();
        move_actual++;
    }

    // Calculate full objective function
    ind.fobj[2] = params.alfa*ind.fobj[0] + params.beta*ind.fobj[1];
}

void evaluateIndWithTimePrint(individuo &ind, vector<vector<int>> &initial_yard, vector<int> &stack_position){

    ind.fobj[0] = 0.0; ind.fobj[1] = 0.0; ind.fobj[2] = 0.0;

    //Copiamos variables iniciales para trackear los movimientos
    vector<vector<int>> actual_yard = initial_yard;
    vector<int> actual_positions = stack_position;

    //Inicializamos variables
    int id_to_retrieve = 1;
    int id_to_relocate;
    int n_containers_actual = n_initial_containers;
    int move_actual = 0;
    int n_moves = ind.moves.size();

    // Parameters of the time comsumption
    float gantry_base = 40.0; // grantry time base to move
    float trolley_base = 30.0; // pick up and drop off time
    float gantry_move = 3.5; // time per bay movement
    float trolley_move = 1.2; // time per stack movement

    int crane_init_stack;
    int crane_final_stack;
    int crane_distance_stack;
    int crane_init_bay = 0;
    int crane_final_bay;
    int crane_distance_bay;

    bool flag = true;

    //Hasta que no queden containers
    while(n_containers_actual > 0){

        ind.fobj[0]++;
        int origin_stack = actual_positions[id_to_retrieve];
        int h_origin = actual_yard[origin_stack].size();

        cout << " move - " << ind.fobj[0] << endl;

        // Crane time repositioning movement
        if (flag == true) // First movement of the crane
        {
            // Special placement for the initial stowage position of the crane from stack -1 to stack 0 of each bay
            crane_init_stack = crane_init_bay*n_rows;
            ind.fobj[1] += trolley_move; // Time for getting into the yard from stack -1
            flag = false;

        }else // Continue with the rest movements
        {
            // Reposition of the crane from previous position to new origin position
            crane_init_stack = crane_final_stack; // Previous position
        }
        crane_init_bay = floor(crane_init_stack/n_rows);

        // New origin position
        crane_final_stack = origin_stack; 
        crane_final_bay = floor(crane_final_stack/n_rows);

        crane_distance_bay = abs(crane_final_bay - crane_init_bay);
        crane_distance_stack = abs(crane_init_stack - crane_final_stack + n_rows*(crane_final_bay - crane_init_bay));

        //cout << crane_distance_bay << " - " << crane_distance_stack << endl;

        // Reposition movement without container (no trolley base time)
        if (crane_distance_bay == 0 && crane_distance_stack > 0) // Movement in the same bay (only trolley movement)
        {
            ind.fobj[1] += crane_distance_stack*trolley_move;

        }else if(crane_distance_bay > 0 && crane_distance_stack == 0) // Movement in the same stack (only grantry movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move;

        }else if (crane_distance_bay > 0 && crane_distance_stack > 0) // Movement in diferente bay and stack (gantry and trolley movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move + crane_distance_stack*trolley_move;
        }

        //cout << ind.fobj[1] << endl; 

        // Crane time for associated movement (Relocation or Extraction)
        crane_init_stack = origin_stack; // Set stack origin position
        crane_init_bay = floor(crane_init_stack/n_rows); // Set bay origin position

        // Extraction
        //Si está en el tope lo sacamos
        if(id_to_retrieve == actual_yard[origin_stack][h_origin-1]){
            actual_yard[origin_stack].pop_back();
            actual_positions[id_to_retrieve] = -1;
            id_to_retrieve++;
            n_containers_actual--;

            ind.fobj[1] += trolley_move; // Time for getting out of the yard to stack -1
            flag = true; // Next movement will need to add repositioning into the yard

            crane_final_stack = crane_init_bay*n_rows; // Set stack destination to start of each bay
            crane_final_bay = crane_init_bay; // Set bay destination position as the same
        }

        // Relocation
        //Si no es así entonces aplicamos heuristica que toca
        else{
            id_to_relocate = actual_yard[origin_stack][h_origin-1];
            //Si aun quedan movimientos en la lista, aplicarlos
            if(move_actual < n_moves){
                apply_chosen_heuristic(actual_yard, actual_positions, origin_stack, ind.moves[move_actual]);
                move_actual++;
            }
            //Si no quedan, agregar nuevos movimientos de forma aleatoria
            else{
                int coded_heuristic = apply_random_heuristic(actual_yard, actual_positions, origin_stack);
                ind.moves.push_back(coded_heuristic);
            }
            crane_final_stack = actual_positions[id_to_relocate]; // Set stack destination position
            crane_final_bay = floor(crane_final_stack/n_rows); // Set bay destination position
        } 

        crane_distance_bay = abs(crane_final_bay - crane_init_bay);
        crane_distance_stack = abs(crane_init_stack - crane_final_stack + n_rows*(crane_final_bay - crane_init_bay));
        //cout << crane_distance_bay << " - " << crane_distance_stack << endl;

        // Movement with loaded container (trolley base time included)
        if (crane_distance_bay == 0 && crane_distance_stack > 0) // Movement in the same bay (only trolley movement)
        {
            ind.fobj[1] += crane_distance_stack*trolley_move;

        }else if(crane_distance_bay > 0 && crane_distance_stack == 0) // Movement in the same stack (only grantry movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move;

        }else if (crane_distance_bay > 0 && crane_distance_stack > 0) // Movement in diferente bay and stack (gantry and trolley movement)
        {
            ind.fobj[1] += gantry_base + crane_distance_bay*gantry_move + crane_distance_stack*trolley_move;
        
        }

        // Container movement
        ind.fobj[1] += trolley_base;
        cout << " acc time - " << ind.fobj[1] << endl;      
  
    }

    //Si no se utilizaron todos los movimientos, eliminar los sobrantes
    while(move_actual < n_moves){
        ind.moves.pop_back();
        move_actual++;
    }

    // Calculate full objective function
    ind.fobj[2] = params.alfa*ind.fobj[0] + params.beta*ind.fobj[1];
}