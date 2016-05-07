#include "neuron.h"
ofstream neuronErrLog("log.txt");


void neuron::initNeuron(int inputs, float beta, bool isRand){
    inputsCount  = inputs;
	weights = new float[inputsCount];
    output = 0.0;

    nmin = 0.0; //пороговое значение
    weights[0] = nmin;


    if(isRand){
        randomizeWeights(beta);
    }
}

neuron::neuron()
{
    
}
neuron::~neuron()
{
	delete[] weights;
}

void neuron::randomizeWeights(float beta)
{
	 
    for(int i = 1; i < inputsCount; i++)
    {
        //generates random weights for neuron's weights [-0.5...0.5]
        weights[i] = (float)rand()/(float)(RAND_MAX) * 1.0 - 0.5;
        cout<<weights[i]<<" ";
    }
    cout<<"\n";


    /*float norm;
    float squareSum = 0;

    for(int j = 0; j < inputsCount; j++)
    {
        squareSum += weights[j]*weights[j];
    }
    norm = sqrt(squareSum);

    //normalize weights
    for(int k = 0; k < inputsCount; k++)
    {
        weights[k] = beta*weights[k]/norm;
    }*/

}


//Treshold function
float neuron::tfuncSign(float sig)
{
    //Heavieside here!
    if(sig >= 0.0) return 1.0;
    else return 0.0;
}

float neuron::tfuncSigmoid(float sig)
{
    //speed of learning
    float omicron = 0.0;
    float omicron0 = 1.0;

    return (1.0 /(1.0 + exp( -(sig+omicron)/omicron0 )));

}

//Summation function
float neuron::summate(float *inputs, int tfunc)
{
    float signal = 0.0;

    for(int i = 0; i < inputsCount-1; i++)
    {
        signal += weights[i+1] * inputs[i+1];
    }
    signal += weights[0]; //activation weight

    switch(tfunc)
    {
        case 1: signal = tfuncSign(signal); break;
        case 2: signal = tfuncSigmoid(signal); break;
    }

    return signal;
}

void neuron::outWeights(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
	/* Write activ weight too */
    for(int i = 0; i < inputsCount; i++)
    {
        fwrite((weights + i), sizeof(float), 1, fp);
    }

}
