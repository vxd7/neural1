#include <iostream>
#include <fstream>
#include <cmath>

class neuron{
public:
	int inputsCount;
	float *weights;
	float output;

	float nmin; //порог

	int row, col;

	neuron(){
		weights = (float *)calloc(inputsCount + 1, sizeof(float));
		output = 0.0;

		row = inputsCount + 1;
		col = 32;

		nmin = 1;
		weights[0] = nmin;

		randomizeWeights();
	}

	void randomizeWeights()
	{
		//seed random pool
		srand (static_cast <unsigned> (time(0)));

		for(int i = 1; i < inputsCount; i++)
		{
			//generates random weights for neuron's weights [-1...1]
			weights[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX * 2 - 1);
		}
	}

	/*Changes weights of the current neuron. 
	 * Input:
	 * int *input -- vector
	 * d - differenece between out of the neuron and expected out
	 * */
	void changeWeights(int *input, int d)
	{
		for(int i = 0; i < inputsCount; i++)
		{
			weights[i] = d * input[i];
		}
	}

	//Treshold function
	float tfunc(float sig)
	{
		//Heavieside here!
		if(sig >= 0.0) return 1.0;
		else return 0.0;
	}

	//Summation function
	float summate(int *inputs)
	{
		float signal = 0.0;

		for(int i = 0; i < inputsCount; i++)
		{
			signal += weights[i] * inputs[i];	
		}

		return tfunc(signal);
	}

}
