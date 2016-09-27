#pragma once

#define NET_DEBUG 0

#ifdef GLOBAL_DEBUG
#define NET_DEBUG 1
#endif

#include "layer_p.h"
#include <map>

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
 *
 * Delta rule learning algo:
 * 1. For single-laayer perceptron the delta leaning rule is used
 * 2. Input vectors are readd and output  vectors are formed
 * 3. The output vectors ther are compared with the ideal output vectors
 * 4. The difference is calculated and delta rule function is then executed
 */
class perceptron
{
private:
	/**
	 * The number of layers
	 */
	int layerCount;

	/**
	 * A single layer of the perceptron
	 */
	layer *networkLayers;

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
	 * The number of neurons in the layers.
	 * Number of neurons MAY NOT be equal to the size of the input vector
	 */
	vector <int> neuronInLayer;

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

	/* Flags and flag variables */

	/**
	 * Whether we want to randomize our neurons
	 */
	bool randomize;

	/**
	 * Are the files initialized yet?
	 * true -- files are initialized. Flag is set by initFiles function.
	 * false -- files are _not_ initialized. The default state of the flag.
	 */
	bool filesInitFlag;
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

	bool initNetwork(int layerCount, vector <int> layers);
	bool initFiles(string inputFile, string outputFile = "", string bkpFile = "");

	void getInput();
	void getInput(int k); //from file

	/**
	 * bool write -- whether we want to write output data to the file
	 * If false, the data is printed to the STDOUT
	 */
	void processData(bool write = true);

	bool writeVectorToFile(FILE *fp, vector<float> &arr, int n = -1);
	vector<float> readVectorFromFile(FILE *fp, int start, size_t vectorSize, bool print);


	float getInputVectorComponent(int vec, int comp);
	float getOutputVectorComponent(int vec, int comp);

	/* The number of input vectors in file */
	int getComponentCount(FILE* fp, float component_size);

	void writeWeightsToFile();

	void readWeightsFromFile();
	//void readWeightsFromFile(string newBkpFilename);

	void printVectorsFromFile(FILE *fp, int fileVectorCount, int fileVectorCompCount);
	void printVectorsFromFile(string pnFileType);
	
	void eraseOutputFile();
};
