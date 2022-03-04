#pragma once
#include "Agent.h"
#include "Params.h"

using namespace std;

class Infection :public Params{
private:
    int S_number, I_number, R_number;
    double S_to_I_probability, I_to_R_probability;
    double time;

    Agent agent;

    void initial_probability();
    void time_change();
    void state_change();
    void S_to_I_change(double prime_x);
    void I_to_R_change(double prime_x);

public:
    double Peak_Infected, FES, switch_count_ave;
    bool isLoop;

    Infection(int _N, int _p, int _k); //N,p,k‚ð“ü‚ê‚é‚¾‚¯
    ~Infection();
    
    void infection_part(map<int, vector<vector<int>>>& LatticeList);
};