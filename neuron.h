#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <string>

#include <algorithm>

//#include "layers.h"

#define TFUNC 2 //activation function
using namespace std;


class neuron{
public:
	int inputsCount;
	float *weights;
	float output;

	float nmin;


	neuron();
	~neuron();

	void initNeuron(int inputs, float beta, bool isRand);

	void randomizeWeights(float beta);

	//Treshold function
	float tfuncSign(float sig);

	float tfuncSigmoid(float sig);
	//Summation function
	float summate(float *inputs, int tfunc);

	void outWeights(FILE *fp);

};

