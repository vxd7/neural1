#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include "layers.h"

#define TFUNC 2 //activation function
using namespace std;

/**
 * A class for a single neuron. A neuron has
 * a certain amount of inputs and weights. Neuron summates given components of an input vector
 * according to its' weights. A bunch of neurons is called a layer.
 *
 * @see layer
 */
class neuron{
public:
	int inputsCount; /**< Number of inputs. */
	float *weights; /**< The weights of a current neuron. */
	float output; /**< The output of a neuron. */

	float nmin; /**< A threshold value. Being used as a fixed input value, passing as weights[0]. @see *weights */


	/**
	 * A constructor.
	 * Seeds the random pool. Nothing else is done.
	 */
	neuron();

	/**
	 * A destructr.
	 * Frees memory used by the *weights array.
	 */
	~neuron();

	/**
	 * Neuron's initialization function.
	 * Initializes inputsCount. Allocates a memory for the
	 * *weights array. Sets nmin threshold value. 
	 * <p>
	 * If the variable isRand is true, then the randomization
	 * and normalization of the weights are performed. If isRand 
	 * is false, then nothing else is done.
	 *
	 * @param inputs number of inputs.
	 * @param beta a magic constant needed for
	 * @param isRand a flag variable.
	 * @see randomizeWeights()
	 */
	void initNeuron(int inputs, float beta, bool isRand);

	/**
	 * A function for randomization of the weights of a neuron.
	 * The randomization and normalization on the weights are done.
	 * 1. Randomization: weights are given the random number between [-0.5 ; 0.5].
	 * 2. Normalization:weights are being normalized. See actual code for more info.
	 *
	 * @param beta a magic constant needed for
	 * @see beta
	 */
	void randomizeWeights(float beta);

	/**
	 * Changes the weights according to given delta vector.
	 * @param *delta vector
	 */
	void changeWeights(int *delta);

	/**
	 * Threshold function signum.
	 */
	float tfuncSign(float sig);

	/**
	 * Threshold function sigmoid
	 */
	float tfuncSigmoid(float sig);

	/**
	 * Summation function. Summates all the input signals multiplied 
	 * with weights of a neuron. Executes a threshold function at the 
	 * end. The threshold function is chosen according to the tfunc variable.
	 * <p>
	 * Valid values of tfunc variable: <br>
	 * 1 -> sign Heavieside threshold function is used. <br>
	 * 2 -> Sigmoid threshold function is used.
	 *
	 * This function uses arbitrary input vector. Class neuron doesn't
	 * store  any input vectors. Summate function must only be called from 
	 * a functions of a layer class.
	 *
	 * @param *inputs an input vector
	 * @param tfunc an integer showing which threshold function is to use.
	 * @see weights
	 * @see layer
	 */
	float summate(float *inputs, int tfunc);

	/**
	 * Function for writing the weights of a current neuron to the file.
	 * Writes all the weights of a current neuron to a binary file, 
	 * pointed by *fp.
	 * Goes to the end of file, add the info about the curren neuron. All
	 * the neuron's weights are being written there.
	 *
	 * @param *fp file pointer
	 */
	void outWeights(FILE *fp);

};

