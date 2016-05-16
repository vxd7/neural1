#pragma once
#include "neuron.h"

class layer{
private:
	int numberOfNeurons;
	int inputVectorSize;

	vector<neuron> neurons;
public:
	vector<float> input;
	vector<float> output;

	layer();
	~layer();

	void initLayer(int neuronCount, int inputSize);
	void constructNeurons(bool isRand, const char *fname = NULL);

	void computeOutput();

	bool writeNeuronsToFile(const char *fname);
	bool readNeuronsFromFile(const char *fname);

	
};

