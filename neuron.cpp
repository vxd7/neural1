#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "neuron.h"

class neuron{
public:
	int inputsCount;
	float *weights;
	float output;

	float nmin; //порог

	void neuron(int inputs, float beta){
		inputsCount  = inputs;
		weights = (float *)calloc(inputsCount + 1, sizeof(float));
		output = 0.0;

		nmin = 1; //пороговое значение
		weights[0] = nmin;

		//seed random pool
		srand (static_cast <unsigned> (time(0)));
		randomizeWeights(beta);
	}

	void ~neurons()
	{
	    free(weights);
	}

	void randomizeWeights(float beta)
	{

		for(int i = 1; i < inputsCount; i++)
		{
			//generates random weights for neuron's weights [-0.5...0.5]
			weights[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX * 1 - 0.5);
		}

		float norm;
		float squareSum = 0;

		for(int j = 0; j < inputsCount; j++)
		{
			squareSum += weights[j]*weights[j];
		}
		norm = sqrt(squareSum);

		//normalize weights
		for(int k = 0; k < inputsCount; k++)
		{
			weights[k] = beta*weights[k]/norm;
		}

	}

	/*Changes weights of the current neuron.
	 * Input:
	 * int *delta-- vector
	 * */
	void changeWeights(int *delta)
	{
		for(int i = 0; i < inputsCount; i++)
		{
			weights[i] += delta[i];
		}
	}

	//Treshold function
	float tfuncSign(float sig)
	{
		//Heavieside here!
		if(sig >= 0.0) return 1.0;
		else return 0.0;
	}

	float tfuncSigmoid(float sig)
	{
		//speed of learning
		int omicron = 0.5;
		int omicron0 = 0.5;

		return (1 / exp(-(sig+omicron)/omicron0));

	}

	//Summation function
	float summate(int *inputs, int tfunc)
	{
		float signal = 0.0;

		for(int i = 1; i < inputsCount; i++)
		{
			signal += weights[i] * inputs[i];
		}
		signal += weights[0]; //activation weight

		switch(tfunc)
		{
			case 1: return tfuncSign(signal); break;
			case 2: return tfuncSigmoid(signal); break;
		}
	}

	void outWeights(FILE *fp)
	{
		fseek(fp, 0, SEEK_END);
		for(int i = 0; i < inputsCount; i++)
		{
			fwrite((weights + i), sizeof(float), 1, fp);
		}

	}

};
