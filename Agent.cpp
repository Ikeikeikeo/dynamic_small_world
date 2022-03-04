#include "Agent.h"
#include <chrono>

void Agent::initialize_NI(int N) {
    NI = vector<int>(N, 0);
}

void Agent::initialize_state(int N, int& numberS, int& numberI, int& numberR) {
    initialize_NI(N);

    state = vector<int>(N, S);
    S_list = vector<int>(numberS);
    I_list = vector<int>(numberI);

    iota(S_list.begin(), S_list.end(), 0);

    while (numberI < 5) {
        int xS = random->random((int)S_list.size() - 1);
        int num = S_list[xS];

        /*if (numberI == 0) num0 = num; //ここ消す
        if (numberI == 1) num1 = num;
        if (numberI == 2) num2 = num;
        if (numberI == 3) num3 = num;
        if (numberI == 4) num4 = num;*/

        state[num] = I;
        I_list.emplace_back(num);
        iter_swap(S_list.begin() + xS, S_list.end() - 1);
        S_list.pop_back();

        for (int i = 0; i < (int)neighbors[num].size(); ++i) {
            NI[neighbors[num][i]] += 1;
        }
        ++numberI;
        --numberS;
    }
}

void Agent::switch_network(int N, int p, int k, bool isLoop,map<int,vector<vector<int>>>& LatticeList) {
    //bool isLoop = false;
    //for (int k = 8; k <= 64; k += 8) LatticeList[k] = creatingLattice(N, k, isLoop);
    neighbors = creatingSW(k, (double)p / 100.0, false, LatticeList[k]); //(平均次数k, ショートカット確率p, ループの有無, 参考にするLattice)
    /*for (int i = 0; i < (int)neighbors.size(); ++i) {
        for (int j = 0; j < neighbors[i].size(); ++j) cout << neighbors[i][j] << ", ";
        cout << endl;
    }*/
}

int Agent::update_NI(int N) {
    initialize_NI(N);
    int NI_plus = 0;

    for (int i = 0; i < (int) I_list.size(); ++i) {
        for (int j = 0; j < (int)neighbors[I_list[i]].size(); ++j) {
            int x = neighbors[I_list[i]][j];
            if (state[x] == S) {
                NI[x] += 1;
                ++NI_plus;
            }
        }
    }
    return NI_plus;
}
