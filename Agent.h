#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include "createSmallWorld.h"

using namespace std;

enum State {
    S,I,R
};

/*#define S 0
#define I 1
#define R 2 //‚±‚±‚É‚Ó‚ê‚é
*/

class Agent {
private:
    void initialize_NI(int N);

public:
    vector<int> state;
    vector< vector<int> > neighbors;
    vector<int> NI;
    vector<int> S_list;
    vector<int> I_list;

    Random* random;

    //int num0, num1, num2, num3, num4;

    void initialize_state(int N, int& numberS, int& numberI, int& numberR);
    void switch_network(int N, int p, int k, bool isLoop, map<int, vector<vector<int>>>& LatticeList);
    int update_NI(int N);
};