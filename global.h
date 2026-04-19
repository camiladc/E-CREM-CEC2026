#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <fstream>


struct individuo
{
    // Lista de movimientos (heurísticas codificadas)
    std::vector<int> moves;
    
    // Funciones objetivo
    float fobj[3];

};


struct hyperparams{
    float pmyo;
    float pmut; //Probabilidad que se realice mutacion
    
    //Probabilidades individuales para cada mutación (la idea es que sumen 1)
    float pmut_swap;
    float pmut_inversion; 

    float p_swap;
    float p_intFlip;

    float pcross; //Probabilidad que se realice cross over 
    float pcross_one; //Probabilidad que se realice opc en vez de tpc

    int popsize;
    int max_gen;
    int n_heu;//Cantidad de heurísticas
    int elite;

    float alfa;
    float beta;
};

struct debug_params{
    bool save_pops;
    bool show_initial_yard;
};

struct write_files{
    std::ofstream f_all_pops;
    std::ofstream f_best_ind;
    std::ofstream exec_params;
};

/* Reader functions */
void readInstance(std::ifstream &f, std::vector<std::vector<int>> &yard, std::vector<int> &stack_position);

/* Random functions */
void randomize(int seed);
int getRandomInt(int a, int b);
float getRandomProb();
float getRandomProb_lim(float lim);

/* Initialice functions */
individuo initialize_ind(std::vector<std::vector<int>> &initial_yard, std::vector<int> &stack_position);
std::vector<individuo> initialize_pop(std::vector<std::vector<int>> &initial_yard, std::vector<int> &stack_positions);

/*Heuristics*/
int RIL(std::vector<std::vector<int>> &yard, int origin_stack);
int RI(std::vector<std::vector<int>> &yard, int origin_stack);
int RI_R(std::vector<std::vector<int>> &yard, int origin_stack);
int RIL_R(std::vector<std::vector<int>> &yard, int origin_stack);
int myopic_space(std::vector<std::vector<int>> &yard, int origin_stack);
int myopic_min_space(std::vector<std::vector<int>> &yard, int origin_stack);
int RI_inverse(std::vector<std::vector<int>> &yard, int origin_stack);
int top_diff(std::vector<std::vector<int>> &yard, int origin_stack);
int apply_random_heuristic(std::vector<std::vector<int>> &yard, std::vector<int> &stack_position, int origin_stack);

/*Evolutive Algorithm functions*/
std::vector<individuo> one_point_crossover(individuo padre1, individuo padre2);
std::vector<individuo> opc(individuo &padre1, individuo &padre2);
std::vector<individuo> two_point_crossover(individuo padre1, individuo padre2);
std::vector<individuo> tpc(individuo &padre1, individuo &padre2);
void swap(individuo &ind);
void inversion(individuo &ind);
void intFlip(individuo &ind);
void mutatePop(std::vector<individuo> &pop);
void evaluateInd(individuo &ind, std::vector<std::vector<int>> &yard, std::vector<int> &stack_position);
void evaluateIndWithTime(individuo &ind, std::vector<std::vector<int>> &yard, std::vector<int> &stack_position);
void evaluateIndWithTimePrint(individuo &ind, std::vector<std::vector<int>> &yard, std::vector<int> &stack_position);
void evaluatePop(std::vector<individuo> &pop, std::vector<std::vector<int>> &initial_yard, std::vector<int> &stack_position);
void generateNewPop(std::vector<individuo> &old_pop);

/* Extra functions */
void printYard(std::vector<std::vector<int>> &yard);
void printInd(individuo &ind);
std::tuple<int,int,int> search_next_c(std::vector<std::vector<int>> &yard);
bool desc_sort(std::pair<int, int>& a,  std::pair<int, int>& b);
bool compararPorFobjAsc(const individuo &a, individuo &b);


/* file functions */
void writeIndDecoded(std::vector<std::vector<int>> &yard, std::vector<int> &stack_position, individuo &ind, std::ofstream &f);
void writeInd(individuo &ind, std::ofstream &f);
void writePob(int n_gen_actual, std::vector<individuo>& pop, std::ofstream &f);


/* ------------- VARIABLES GLOBALES ---------------------*/

// Declaramos variables globales de la instancia
extern std::string instance_name;
extern int n_bays;
extern int n_rows;
extern int max_h;
extern int n_initial_containers;
extern std::vector<std::vector<int>> initial_yard;
extern std::vector<int> stack_position;

//declaramos los hiperparametros
extern hyperparams params;
extern debug_params debug;

//Declaramos los archivos
extern write_files files;

// Declaramos el generador aleatoreo
extern std::mt19937 rng;


#endif