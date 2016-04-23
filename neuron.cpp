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

		nmin = 50;

		randomizeWeights();
	}

	void randomizeWeights()
	{
		for(int i = 1; i < inputsCount; i++)
		{
			//seed random pool
			srand (static_cast <unsigned> (time(0)));
			//generates random weights for neuron's weights [-1...1]
			weights[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX * 2 - 1);
		}
	}

}
