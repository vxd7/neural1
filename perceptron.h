#pragma once
#include "layer_p.h"

/**
 * Perceptron overall:
 * 0. The number of neurons in the layer MUST be equal to the number of components in the input vector;
 * 1. Each neuron's weights are indexed from 0;
 * 2. Activational weight doesn't count;
 * 3. We have only one layer -- number zero;
 * 4.
 * 
 * Input/Output vectors:
 * 0. Components of the vector are indexed from 0;
 * 1. Each input/output vecctor has fixed number of components;
 * 2. Each component is of type float;
 * 3. Each input/ouput file has fixed number of input/output vectors;
 * 4. The output vector's number of components matches to the number of neurons in the layer
 * 5. Components in the file are indexed from 0;
 */
class perceptron
{
private:
	/**
	 * A single layer of the perceptron
	 */
	layer pnLayer;

	/**
	 * Whether we want to randomize our neurons
	 */
	bool randomize;

	/**
	 * The size of the input vector -- number of float components in it
	 * Neurons in the layer must have this number of weights each
	 */
	float inputVectorSize;
	float outputVectorSize;

	/**
	 * Total number of vectors in file.
	 * Each vector has its components and each component is a float
	 */
	int inputVectorCount;
	int outputVectorCount;

	/**
	 * The number of neurons in the pnLayer layer.
	 * Number of neurons MAY NOT be equal to the size of the input vector
	 */
	int neuronCount;

	/**
	 * =====FILE DESCRIPTORS:=====
	 */

	/**
	 * Descriptor for input vectors
	 */
	FILE* pnInputFile;

	/**
	 * Descriptor for output of the perceptron
	 */
	FILE* pnOutputFile;

	/**
	 * Descriptor for layer's weights file
	 */
	FILE* pnBkpFile;

public:
	vector<float> pnInput;
	vector<float> pnOutput;

	/**
	 * A file for the layer of the perceptron.
	 * File stores weights for the neurons in the layer
	 */
	string bkpFileName;
	string inputFileName;
	string outputFileName;

	perceptron(bool rnd = false);
	~perceptron();

	bool initNetwork();
	bool initFiles(string inputFile, string outputFile = "", string bkpFile = "");

	void getInput();
	void getInput(int k); //from file

	void scaleInput(int startInterval, int endInterval); //Preprocess input vector in order to fit inc
	void reverseScaleOutput(int startInterval, int endInterval);

	void processData();

	bool writeVectorToFile(FILE *fp, vector<float> &arr, int n = -1);
	vector<float> readVectorFromFile(FILE *fp, int start, int end, bool print);


	float getInputVectorComponent(int vec, int comp);
	float getOutputVectorComponent(int vec, int comp);

	/* The number of input vectors in file */
	int getComponentCount(FILE* fp, float component_size);
	void writeWeightsToFile();
	
};

