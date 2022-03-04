#ifndef CALCULATION_H
#define CALCULATION_H
//#include "createSmallWorld.h"
#include "Infection.h"

using namespace std;

class Calculation {
public:
    Calculation(int _D_hor, int _D_ver, int _filenumber);
    void calculation();

private:
    int D_hor;
    int D_ver;
    int filenumber;
    int NNN;
    vector<int> n;
    bool get_calculation_n();
    int getIntNum_p(string p_st, int Dval);
    int getIntNum_k(string k_st, int Dval);
};

#endif // CALCULATION_H
