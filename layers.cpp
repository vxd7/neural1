#include "layer.h"
ofstream fout("log.txt");

void layer::initLayer(int neuronsC, int inputC)
{
    inputsCount = inputC;
    numberOfNeurons = neuronsC;

	inputs = new float[inputsCount];
	outputs = new float[numberOfNeurons];

	neurons = new neuron[numberOfNeurons];

}
layer::layer()
{
    //nothing here!
}

layer::~layer()
{
	delete[] inputs;
	delete[] outputs;
	delete[] neurons;
}

float layer::computeBeta()
{
    float magicConst = 0.7;
    return (magicConst*pow( (float)numberOfNeurons, (1.0/(float)inputsCount) ));
}

void layer::constructNeurons(bool isRand, const char *fname);
{
    for(int i = 0; i < numberOfNeurons; i++)
    {
        neurons[i].initNeuron(inputsCount, computeBeta(), isRand);

    }

	if(!isRand) //if we don't want to randomize  -- then read from file
	{
		readNeuronsFromFile(fname);
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
    if((fp = fopen(fname, "rb+")) == NULL)
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
    if((fp = fopen(fname, "rb+")) == NULL)
    {
        fout<<"Cannot open file. Function readNeuronsFromFile\n";
        return false;
    }

	float curWeight;

	fseek(fp, 0, SEEK_SET); //jump to the start of file
	for(int i = 0; i < numberOfNeurons; i++)
	{
		for(int j = 1; j < inputsCount; j++)
		{
			fread(&curWeight, sizeof(float), 1, fp);
			neurons[i].weights[j] = curWeight;
		}
	}

	fclose(fp);
}

