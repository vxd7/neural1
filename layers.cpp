#include "layer.h"
ofstream fout("log.txt");

void layer::initLayer(int neuronsC, int inputC)
{
    inputsCount = inputC;
    numberOfNeurons = neuronsC;

    inputs = (float *)calloc(inputsCount, sizeof(float));
    outputs = (float *)calloc(numberOfNeurons, sizeof(float));

    neurons = (neuron *)calloc(numberOfNeurons, sizeof(neuron));

    constructNeurons();
}
layer::layer()
{
    //nothing here!
}

layer::~layer()
{
    free(inputs);
    free(outputs);
    free(neurons);
}

float layer::computeBeta()
{
    float magicConst = 0.7;
    return (magicConst*pow( (float)numberOfNeurons, (1.0/(float)inputsCount) ));
}

void layer::constructNeurons(bool isRand)
{
    for(int i = 0; i < numberOfNeurons; i++)
    {
        neurons[i].initNeuron(inputsCount, computeBeta(), isRand);

    }
}

float* layer::computeOutput()
{
    for(int i = 0; i < numberOfNeurons; i++)
    {
        outputs[i] = neurons[i].summate(inputs, TFUNC);
    }

    return outputs;
}

bool layer::writeNeuronsToFile(const char *fname)
{
    FILE *fp;
    if((fp = fopen(fname, "wb+")) == NULL)
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

bool layer::readNeuronsFromFile(const char *fname)
{
    FILE *fp;
    if((fp = fopen(fname, "wb+")) == NULL)
    {
        fout<<"Cannot open file. Function readNeuronsFromFile\n";
        return false;
    }
}

sslastikov@hse.ru
