#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <string>

#include <algorithm>
#include <vector>

using namespace std;

class neuron{
private:
	int inputsCount; /**< Number of input to the current neuron */
	vector<float> weights; /**< Vector of neuron's weights */

public:
	float output; /**< An output value -- generated by summation funciton with threshold function altogether */

	float aWeight; /**< Activational weight -- currantly not used */
	bool isRand; /**< Whether we want to randomize out weights in current neuron */

	neuron(bool rand = false); /**< Constructor with optional parameter -- randomization flag. If set, randomization is performed */
	neuron();
	~neuron();

	void initNeuron(int inputSize); /**< Initializes the neuron. Sets inputs, randomizes if needed */
	void randomizeWeights(); /**< Function for weights' randomization */

	float tfunc(float sighal); /**< Threshold finction */
	void summate(vector<float> &input); /**< Summation function. Takes input vector as argument */

	bool writeWeights(FILE *fp); /**< Write weights to the file */
	bool readWeights(FILE *fp, int num);
};