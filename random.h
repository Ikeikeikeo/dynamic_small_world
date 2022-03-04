#ifndef RANDOM_H
#define RANDOM_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <random>

class Random {
public:
	int create_int_rand(std::mt19937 mt, int xmin, int xmax);
	double create_double_rand(std::mt19937 mt, double xmin, double xmax);
	int random(int N);
	int random(int Nmin, int Nmax);
	double random(double D);
	double random(double Dmin, double Dmax);
};

#endif	//RANDOM_H
