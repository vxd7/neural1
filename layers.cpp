#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "neuron.h"
//#include "layers.h"

#define TFUNC 2 //activation function
using namespace std;
ofstream fout("log.txt");


class layer{
public:
    neuron *neurons;
    int numberOfNeurons; //in current layer
    int inputsCount; //number of neurons in previous layer, inputs in current one

    float *inputs, *outputs;

    void layer(int neuronsC, int inputC)
    {
        inputsCount = inputC;
        numberOfNeurons = neuronsC;

        inputs = (float *)calloc(inputsCount, sizeof(float));
        outputs = (float *)calloc(numberOfNeurons, sizeof(float));

        neurons = (neuron *)calloc(numberOfNeurons, sizeof(neuron));

        constructNeurons();
    }

    void ~layer()
    {
        free(inputs);
        free(outputs);
        free(neurons);
    }

    float computeBeta()
    {
        float magicConst = 0.7;
        return (magicConst*pow( (float)numberOfNeurons, (1.0/(float)inputsCount) ));
    }

    void constructNeurons()
    {
        for(int i = 0; i < numberOfNeurons; i++)
        {
            neurons[i].neuron(inputsCount, computeBeta());
        }
    }

    float *computeOutput()
    {
        for(int i = 0; i < numberOfNeurons; i++)
        {
            outputs[i] = neurons[i].summate(inputs, TFUNC);
        }

        return outputs;
    }

    bool writeNeuronsToFile(const int *fname)
    {
        FILE *fp;
        if((fp = fopen(fname, "wb+") == NULL))
        {
            fout<<"Error writing weights to file. Layer:" << fname<<"\n";
            cout<<"Network is fucked!";
            return false;
        }


        for(int i = 0; i < numberOfNeurons; i++)
        {
            neurons[i].outWeights(fp);
        }

        fclose(fp);
        fout<<"Successfully written! Layer:" << fname<<"\n";
        return true;
    }

};
