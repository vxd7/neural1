#include "neuron_p.h"

neuron::neuron(bool rand = false)
{
	/* Set randomization variable */
	isRand = rand;	

	/* Set the activational weight */
	aWeight = 1.0;

	/* Reset the output */
	output = 0.0
}

void neuron::initNeuron(int inputSize)
{
	/*Set the number of inputs to the current neuron */
    inputsCount = inputSize;

	/*Initialize the weights vector */
	/* Weights are indexed FROM ZERO, 
	 * activational weight doesn't count AT ALL */
	for(int i = 0; i < inputsCount; i++)
		weights.push_back(float);
		
	/* If the randomization flag is set -- then do the randomization */
	if(isRand)
		randomizeWeights();

	/* Add logger */
}

void neuron::randomizeWeights()
{
    for(int i = 0; i < inputsCount; i++)
    {
        //generates random weights for neuron's weights [-0.5...0.5]
        weights[i] = (float)rand()/(float)(RAND_MAX) * 1.0 - 0.5;

		/* Add logger */
    }
}

float neuron::tfunc(float signal)
{
	if(signal > 0.5) return 1.0;
	else return 0.0;
}

void neuron::summate(vector<float> &input)
{
    float signal = 0.0;

	/* Check whether input vector has the right size */
	if(input.size() != weights.size()) {
		cout << "Incorrect neuron input!\n";
		exit(1);
	}

	/* Sum up */
    for(int i = 0; i < inputsCount; i++)
    {
        signal += weights[i] * inputs[i];
    }

	/* And execute threshold function */
	signal = tfunc(signal);

	/* Store the result in the neuron */
	output = signal;

}

void neuron::writeWeights(FILE *fp)
{
    fseek(fp, 0, SEEK_END);

    for(int i = 0; i < inputsCount; i++)
    {
        fwrite((weights + i), sizeof(float), 1, fp);
    }

}
