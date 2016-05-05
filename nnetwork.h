#pragma once
#include "layer.h"


class neuralNetwork{
public:
    layer *networkLayers;

    int layersCount;
    int *neuronsInLayers;

	int inputVectorSize;

    float *networkInput;
    float *networkOutput;

	string *fileNames;

    neuralNetwork();
    ~neuralNetwork();

	bool initNetwork();
	void generateNames();

    void getInput(); //from keyboard
	void getInput(const char *fname, int k); //from file
	void scaleInput(const char* fname, int startInterval, int endInterval); //Preprocess input vector in order to fit inc
	void reverseScaleOutput(const char* fname, int startInterval, int endInterval);

	void processLayersData();
	void writeWeightsToFiles();


	float getInputVectorComponent(FILE *fp, int vec, int comp);
	float getOutputVectorComponent(FILE *fp, int vec, int comp);

	/* The number of input vectors in file */
	int inputVectorNumber(FILE *fp);

};
