#include "layer.h"
ofstream layerLog("neuralog.txt");


void layer::initLayer(int neuronsC, int inputC)
{
	layerLog << "class layer: Allocating memory\n";

    inputsCount = inputC;
    inputsCount++; //because we want to preserve the activational weights
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
	layerLog << "class layer: Deleting allocated variables\n";

	delete[] inputs;
	delete[] outputs;
	delete[] neurons;
}

float layer::computeBeta()
{
    float magicConst = 0.7;
    return (magicConst*pow( (float)numberOfNeurons, (float)(1.0/(float)inputsCount) ));
}

void layer::constructNeurons(bool isRand, const char *fname)
{
	layerLog << "class layer: constructing neurons\n";

    float beta = computeBeta();
    for(int i = 0; i < numberOfNeurons; i++)
    {
        neurons[i].initNeuron(inputsCount, beta, isRand);

    }

	if(!isRand) //if we don't want to randomize  -- then read from file
	{
		readNeuronsFromFile(fname);
	}
}

void layer::computeOutput()
{
	layerLog << "class layer: computing output\n";

    for(int i = 0; i < numberOfNeurons; i++)
    {
        outputs[i] = neurons[i].summate(inputs, TFUNC);
    }

}

bool layer::writeNeuronsToFile(const char *fname)
{
	layerLog << "class layer: writing neurons to file\n";

    FILE *fp;
    if((fp = fopen(fname, "wb+")) == NULL)
    {
        layerLog<<"[ERROR] class layer: Error writing weights to file. Layer:" << fname<<"\n";
        cout<<"Network is fucked!";
        return false;
    }


    for(int i = 0; i < numberOfNeurons; i++)
    {
        neurons[i].outWeights(fp);
    }

    fclose(fp);
    layerLog<<"class layer: Successfully written! Layer:" << fname<<"\n";
    return true;
}

bool layer::readNeuronsFromFile(const char *fname)
{
	layerLog << "class layer: reading neurons weights from file\n";

    FILE *fp;
    if((fp = fopen(fname, "rb+")) == NULL)
    {
        layerLog<<"[ERROR] class layer: Cannot open file "<< fname << ". Function readNeuronsFromFile\n";
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
	return true;
}

