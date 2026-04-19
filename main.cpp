#include<bits/stdc++.h>
#include <fstream>
#include <chrono>

#include "global.h"
using namespace std;

//Creamos las variables globales del problema 
string instance_name;
int n_bays;
int n_rows;
int max_h;
int n_initial_containers;

vector<vector<int>> initial_yard;
vector<int> stack_position; //stack_position[i] -> indica el stack en el que está el individuo i

//Variables con los hyperparámetros
hyperparams params;
debug_params debug;

//Variables con los archivos
write_files files;

//Generador de numeros aleatoreos
mt19937 rng;


int main(int argc, char *argv[]){


    //Pedimos la semilla por consola
    if(argc < 14){
        cout << "Usage:\n./main instance_path seed max_gen popsize pcross pmut pmut_swap pmut_inversion alfa beta pcross_one p_swap p_intFlip\n";
        cout << "Example:\n./main Original_instances/Random/R020306_0020_001.txt 11 1500 30 0.4 0.7 0.5 0.3 5 0.001 0.3 0.4 0.9\n";
        exit(1);
    }

    //Asignamos el path
    string path = argv[1];
  
    //Asignamos la semilla 
    int seed = atoi(argv[2]);
    randomize(seed);

    //Definimos hyperparametros del algoritmo
    params.max_gen = atoi(argv[3]);
    params.popsize = atoi(argv[4]); // Multiplo de 4 
    params.pmyo = 0.5;
    params.pcross = stof(argv[5]);
    params.pmut = stof(argv[6]);
    params.pmut_swap = stof(argv[7]);
    params.pmut_inversion = stof(argv[8]);
    params.n_heu = 5;
    params.elite = 1; // Multiplo de 4, menor a popsize
    params.alfa = stof(argv[9]);
    params.beta = stof(argv[10]);
    params.pcross_one = stof(argv[11]);
    params.p_swap = stof(argv[12]);
    params.p_intFlip = stof(argv[13]);

    //Definimos que vamos a debugear
    debug.save_pops = true;
    debug.show_initial_yard = true;

    //Creamos los archivos de data
    if(debug.save_pops){
        files.f_all_pops.open("data/all_pops.txt");
        files.f_best_ind.open("data/best_ind.txt");
        files.exec_params.open("data/params.txt");
    }

    //Escribimos los parámetros de ejecución
    if(debug.save_pops){
        files.exec_params << "PARAMS: \n";
        files.exec_params << "Instance " << path << "\n";
        files.exec_params << "Seed: " << seed << "\n";
        files.exec_params << "Max_gen: " << params.max_gen << "\n"; 
        files.exec_params << "Popsize: " << params.popsize << "\n";
        files.exec_params << "Cross prob: " << params.pcross << "\n";
        files.exec_params << "Mut prob: " << params.pmut << "\n";
        files.exec_params << "Swap prob: " << params.pmut_swap << "\n";
        files.exec_params << "Inversion prob: " << params.pmut_inversion << "\n";
        files.exec_params << "N heuristics: " << params.n_heu << "\n";
        files.exec_params << "Elites: " << params.elite << "\n";
    }

    
    //Comenzamos lectura de la instancia
    ifstream file(path);

    if(!file.is_open()){
        cout << "\nNo se pudo abrir el archivo" << path << endl;
        exit(1);
    }

    cout << "Reading instance: " << path << "\n";
    readInstance(file, initial_yard, stack_position);
    //Mostramos el yard inicial
    if(debug.show_initial_yard) printYard(initial_yard);

    
    auto start = chrono::high_resolution_clock::now();
    //Inicializamos la población

    vector<individuo> pop = initialize_pop(initial_yard, stack_position);

    evaluatePop(pop, initial_yard, stack_position);

    //Guardamos la población inicial en el txt
    if(debug.save_pops){
        writePob(0, pop, files.f_all_pops);
    }
    

    bool pruebas = false;
    if(pruebas){
        //Prueba de crossover
        cout<<endl<<"Prueba Crossover: "<<endl;
        vector<individuo> hijos = one_point_crossover(pop[params.popsize-1],pop[params.popsize-2]);
        evaluateInd(hijos[0], initial_yard, stack_position); 
        evaluateInd(hijos[1], initial_yard, stack_position);
        cout<<endl<<"Padres: ";
        cout<<endl; printInd(pop[params.popsize-1]); cout<<endl; printInd(pop[params.popsize-2]);
        cout<<endl<<"Hijos: ";
        cout<<endl; printInd(hijos[0]); cout<<endl; printInd(hijos[1]);

        //Prueba de mutación
        cout<<endl<<"Prueba Mutaciones: "<<endl;
        individuo original = hijos[0];
        cout<<endl; cout<<"Ind original: "; printInd(original);cout<<endl;

        //Swap
        swap(hijos[0]);
        evaluateInd(hijos[0], initial_yard, stack_position);
        cout<<endl; cout<<"Mutacion swap: "; printInd(hijos[0]);cout<<endl;
        hijos[0] = original;

        //Inversion
        inversion(hijos[0]);
        evaluateInd(hijos[0], initial_yard, stack_position);
        cout<<endl; cout<<"Mutacion inversion: "; printInd(hijos[0]);cout<<endl;
        hijos[0] = original;

        //Int flip
        intFlip(hijos[0]);
        evaluateInd(hijos[0], initial_yard, stack_position);
        cout<<endl; cout<<"Mutacion intFlip: "; printInd(hijos[0]);cout<<endl;
    }    

    //Ejecutamos el algoritmo

    for (int i = 0; i < params.max_gen; i++)
    {      
        generateNewPop(pop);
        //mutatePop(pop);
        evaluatePop(pop, initial_yard, stack_position);

        if(debug.save_pops) writePob(i+1, pop, files.f_all_pops);
    }
    
    //De la última población sorteamos de forma descendente
    sort(pop.begin(), pop.end(), compararPorFobjAsc);

    if(debug.save_pops) writeIndDecoded(initial_yard, stack_position, pop[0], files.f_best_ind);


    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout<<endl<<"Mejor individuo encontrado: ";printInd(pop[0]);
    cout<<endl<<"Tiempo de ejecucion: "<<elapsed.count()<<" segundos"<<endl;

    //if(debug.save_pops) evaluateIndWithTimePrint(pop[0], initial_yard, stack_position);

    return 0;

}