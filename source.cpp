#include "Calculation.h"

int main(int argc, char* argv[]) {
	if (argc == 1) return 0;
	int filenumber = stoi(argv[1]);
	int D_hor = 101;
	int D_ver = 8;

	Calculation calculation(D_hor, D_ver, filenumber);
	calculation.calculation();
	
	return 0;
}