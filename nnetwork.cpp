#include "nnetwork.h"
#define ISRAND true
ofstream nnetworkErrLog("log.txt");

neuralNetwork::neuralNetwork()
{
	//fuck off!
	//seed random pool
    srand (static_cast <unsigned> (time(0)));
}

neuralNetwork::~neuralNetwork()
{
	delete[] neuronsInLayers;
	delete[] networkLayers;

	delete[] fileNames;
	delete[] networkInput;
	delete[] networkOutput;
}

void neuralNetwork::generateNames()
{
	string defname = "layer";
	fileNames = new string[layersCount];
	for(int i = 0; i < layersCount; i++)
	{
		fileNames[i] = defname + (const char)(i + '0') + ".bin";
	}

}

bool neuralNetwork::initNetwork()
{
	cout<<"Input the number of layers\n"; cin>>layersCount;
	cout<<"Input the size of input vector\n"; cin>>inputVectorSize;

	neuronsInLayers = new int[layersCount];
	networkInput = new float[inputVectorSize];

    generateNames(); //generate filenames
	for(int i = 0; i < layersCount; i++)
	{
		cout<<"Input the number of neurons in "<<i<<"'th layer";
		cin>>neuronsInLayers[i];
	}

	networkOutput = new float[neuronsInLayers[layersCount - 1]]; //Initialize the output vector

	networkLayers = new layer[layersCount];

	networkLayers[0].initLayer(neuronsInLayers[0], inputVectorSize);
	networkLayers[0].constructNeurons(ISRAND, fileNames[0].c_str());
	for(int i = 1; i < layersCount; i++)
	{
		networkLayers[i].initLayer(neuronsInLayers[i], neuronsInLayers[i - 1]);
		networkLayers[i].constructNeurons(ISRAND, fileNames[i].c_str());
	}
	return true;
}

void neuralNetwork::getInput(const char *fname, int k) //k - ordinary number of vector
{
    FILE *fp;

    if((fp = fopen(fname, "rb+")) == NULL)
    {
        nnetworkErrLog<<"Error reading input vectors! Function neuralNetwork::getInput.";
        exit(1);

    }

	/* k is indexed from 0 */
	if(k > inputVectorNumber(fp) - 1)
	{
		nnetworkErrLog<<"Incorrect ordinary number of input vector!";
		fclose(fp);
		exit(1);
	}

	/* inputVectorSize -- the number of components in the input vector */
    fseek(fp, (sizeof(float) * inputVectorSize * k), SEEK_SET);

    for(int i = 0; i < inputVectorSize; i++)
    {
        fread(&networkInput[i], sizeof(float), 1, fp);
    }

    fclose(fp);

}

/* UNTESTED FUNCTION! */
/**
 * int vec -- ordinary number of a vector
 * int comp -- ordinary number of a vec'th vector's component
 * <p>
 * V0=(x0, x1, x2);
 * V1=(x0, x1, x2);
 *
 * vec = 0, comp = 2 <==> V0.x2
 * vec = 1, comp = 3 <==> V1.x3
 */
float neuralNetwork::getInputVectorComponent(FILE *fp, int vec, int comp)
{
	float component;

	/* Check whether given values are correct. (-1) because vec and comp are indexed from 0 */
	if((vec > inputVectorNumber(fp) - 1) || (comp > inputVectorSize - 1))
	{
		nnetworkErrLog << "Incorrect vector or component number! Cannot read " << vec <<"'th vector's "<< comp <<"'th component!";
		exit(1);

	}
	/* Jump directly to the vec'th vector in the input file */
    fseek(fp, (sizeof(float) * inputVectorSize * vec), SEEK_SET);

	/* Jump to the comp'th component of the vec'th vector */
	fseek(fp, (comp * sizeof(float)), SEEK_CUR);

	/* Read the needed vector's component */
	fread(&component, sizeof(float), 1, fp);

	return component;

}

float neuralNetwork::getOutputVectorComponent(FILE *fp, int vec, int comp)
{
	float component;
	int outputVectorSize = neuronsInLayers[layersCount - 1];
	int outputVectorCount = inputVectorNumber(fp); //the number of output vecrors equals to the number og input ones

	/* Check whether given values are correct. (-1) because vec and comp are indexed from 0 */
	if((vec > outputVectorCount - 1) || (comp > outputVectorSize - 1))
	{
		nnetworkErrLog << "Incorrect vector or component number! Cannot read " << vec <<"'th vector's "<< comp <<"'th component!";
		exit(1);

	}
	/* Jump directly to the vec'th vector in the input file */
    fseek(fp, (sizeof(float) * outputVectorSize * vec), SEEK_SET);

	/* Jump to the comp'th component of the vec'th vector */
	fseek(fp, (comp * sizeof(float)), SEEK_CUR);

	/* Read the needed vector's component */
	fread(&component, sizeof(float), 1, fp);

	return component;

}

/* UNTESTED FUNCTION! */
void neuralNetwork::scaleInput(const char* fname, int startInterval, int endInterval)
{
    FILE *fp;

    if((fp = fopen(fname, "rb+")) == NULL)
    {
        nnetworkErrLog<<"Error reading input vectors! Function neuralNetwork::scaleInput.";
        exit(1);

    }

	float minComp, maxComp;
	float tmpComp;
	int inputVectorCount = inputVectorNumber(fp);

	/* Now we must find max and min first components throughout the all vectors */
	for(int i = 0; i < inputVectorSize; i++)
	{
		minComp = getInputVectorComponent(fp, i, 0); 
		maxComp = minComp;

		for(int j = 0; j < inputVectorCount; j++)
		{
			tmpComp = getInputVectorComponent(fp, j, i);
			if(tmpComp > maxComp) maxComp = tmpComp;
			if(tmpComp < minComp) minComp = tmpComp;	
		}

		/* And do the actual scaling */
		for(int k = 0; k < inputVectorSize; k++)
		{
			networkInput[k] = ((networkInput[k] - minComp) * (endInterval - startInterval) / (maxComp - minComp)) + endInterval;
		}


	}

	fclose(fp);

}

/* UNTESTED FUNCTION! */
void neuralNetwork::reverseScaleOutput(const char* fname, int startInterval, int endInterval)
{
    FILE *fp;

    if((fp = fopen(fname, "rb+")) == NULL)
    {
        nnetworkErrLog<<"Error reading output vectors! Function neuralNetwork::reverseScaleOutput.";
        exit(1);

    }

	float minComp, maxComp;
	float tmpComp;
	int outputVectorSize = neuronsInLayers[layersCount - 1];
	int outputVectorCount = inputVectorNumber(fp); //the number of output vectors equals to the number of input ones

	/* Now we must find max and min first components throughout the all vectors */
	for(int i = 0; i < inputVectorSize; i++)
	{
		minComp = getOutputVectorComponent(fp, i, 0); 
		maxComp = minComp;

		for(int j = 0; j < outputVectorCount; j++)
		{
			tmpComp = getOutputVectorComponent(fp, j, i);
			if(tmpComp > maxComp) maxComp = tmpComp;
			if(tmpComp < minComp) minComp = tmpComp;	
		}

		/* And do the actual scaling */
		for(int k = 0; k < outputVectorSize; k++)
		{
			networkOutput[k] = minComp + ((networkOutput[k] - startInterval) * (maxComp - minComp) / (endInterval - startInterval));
		}

	}

	fclose(fp);
}

void neuralNetwork::processLayersData()
{
    //getInput and scaleInput are executed from main()
    //We have scaled input vector now
    for(int i = 1; i < networkLayers[0].inputsCount; i++)
    {
        networkLayers[0].inputs[i] = networkInput[i-1];
    }

    for(int i = 0; i < layersCount; i++)
    {
        networkLayers[i].computeOutput();
        for(int j = 1; j < networkLayers[i + 1].inputsCount; j++)
        {
            networkLayers[i + 1].inputs[j] = networkLayers[i].outputs[j-1];
        }

    }

    for(int i = 0; i < neuronsInLayers[layersCount - 1]; i++)
    {
         networkOutput[i] = networkLayers[layersCount - 1].outputs[i];
    }


    //reverseScaleOutput is done from main()
}

void neuralNetwork::writeWeightsToFiles()
{
    for(int i = 0; i < layersCount; i++)
    {
        networkLayers[i].writeNeuronsToFile(fileNames[i].c_str());
    }
}

int neuralNetwork::inputVectorNumber(FILE *fp)
{
	int num, pos;
	pos = ftell(fp); //save our old position in file 

	fseek(fp, 0, SEEK_END);
	num = ftell(fp)/(inputVectorSize * sizeof(float));
	fseek(fp, pos, SEEK_SET); //restore our old position

	return num;

}
