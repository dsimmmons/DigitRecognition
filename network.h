#pragma once
#include <cmath>
#include "neuron.h"
#include <vector>
#include "newF.h"

#define INPUTS 784
#define HIDDENS 30
#define OUTPUTS 10

class network
{
public:
	network()
	{

		y.resize(OUTPUTS);

		dCdAOuput.resize(OUTPUTS);

		dCdAHidden.resize(HIDDENS);

		outWD.resize(OUTPUTS);

		for (int j = 0; j < OUTPUTS; j++)
		{
			outWD[j].resize(HIDDENS);
		}

		hiddWD.resize(HIDDENS);

		for (int j = 0; j < HIDDENS; j++)
		{
			hiddWD[j].resize(INPUTS);
		}

		outBD.resize(OUTPUTS);

		hiddBD.resize(HIDDENS);

		aOut.resize(OUTPUTS);

		aHidd.resize(HIDDENS);

		// Generate random network of INPUTS input neurons, HIDDENS hidden neurons, and OUTPUTS output neurons

		for (int i = 0; i < INPUTS; i++)
		{
			inputs[i] = new neuron(1);
		}
		for (int i = 0; i < HIDDENS; i++)
		{
			hiddens[i] = new neuron(2);
			hiddens[i]->resizeWeights(INPUTS);
			hiddens[i]->randWeights();
			hiddens[i]->randBias();
		}
		for (int i = 0; i < OUTPUTS; i++)
		{
			outputs[i] = new neuron(3);
			outputs[i]->resizeWeights(HIDDENS);
			outputs[i]->randWeights();
			outputs[i]->randBias();
		}
	}

	~network()
	{
		delete[] inputs;
		delete[] hiddens;
		delete[] outputs;
	}

	//============================================================

	void setInputValue(float inputValue, int index)
	{
		inputs[index]->setZ(inputValue);
	}

	//============================================================

	void think()
	{
		//============================================================
		//Calculate Hidden Layer values 

		for (int i = 0; i < HIDDENS; i++)
		{
			hiddens[i]->resetZ();
			for (int j = 0; j < INPUTS; j++)
			{
				hiddens[i]->addElementWeight(inputs[j]->getZ(), j);
			}
			hiddens[i]->applyBias();
			aHidd[i] = transform(hiddens[i]->getZ());	
		}

		//============================================================
		//Calculate Output Layer values

		for (int i = 0; i < OUTPUTS; i++)
		{
			outputs[i]->resetZ();
			for (int j = 0; j < HIDDENS; j++)
			{
				outputs[i]->addElementWeight(aHidd[j], j);
			}
			outputs[i]->applyBias();
			aOut[i] = transform(outputs[i]->getZ());
		}
	}

	//============================================================

	void calculateCost(int realAnswer)
	{
		cost = 0.0f;
		int max = 0;
		for (int i = 0; i < OUTPUTS; i++)
		{
			if (aOut[i] > aOut[max])
			{
				max = i;
			}

			if (i == realAnswer)
			{
				y[i] = 1.0f;
				answer = i;
			}
			else
			{
				y[i] = 0.0f;
			}

			cost += pow((y[i] - aOut[i]), 2);
		}
		myAnswer = max;
		correct = (y[max] == 1.0f);
	}

	float getCost()
	{
		return cost;
	}

	int getAnswer()
	{
		return myAnswer;
	}

	bool isCorrect()
	{
		return correct;
	}

	//============================================================

	void learnErrors(int learningRate, int batchSize) // Back Propegation + Standard Gradient Descent
	{
		for (int j = 0; j < OUTPUTS; j++)
		{
			dCdAOuput[j] = (aOut[j] - y[j]) * (transformPrime(outputs[j]->getZ()));

			for (int k = 0; k < HIDDENS; k++)
			{
				outWD[j][k] +=  -((float)learningRate / (float)batchSize) * dCdAOuput[j] * aHidd[k];
			}
			outBD[j] += -((float)learningRate / (float)batchSize) * dCdAOuput[j];
		}

		for (int j = 0; j < HIDDENS; j++)
		{
			dCdAHidden[j] = 0.0f;
			for (int k = 0; k < OUTPUTS; k++)
			{
				dCdAHidden[j] += outputs[k]->getWeight(j) * dCdAOuput[k] * transformPrime(hiddens[j]->getZ());
			} 

			for (int k = 0; k < INPUTS; k++)
			{
				hiddWD[j][k] += -((float)learningRate / (float)batchSize) * dCdAHidden[j] * inputs[k]->getZ();
			}
			hiddBD[j] += -((float)learningRate / (float)batchSize) * dCdAHidden[j];
		}
	}

	void applyLearned()
	{
		for (int j = 0; j < OUTPUTS; j++)
		{
			for (int k = 0; k < HIDDENS; k++)
			{
				outputs[j]->changeWeight((outWD[j][k]), k);
			}
			outputs[j]->changeBias(outBD[j]);
		}
		for (int j = 0; j < HIDDENS; j++)
		{
			for (int k = 0; k < INPUTS; k++)
			{
				hiddens[j]->changeWeight(hiddWD[j][k], k);
			}
			hiddens[j]->changeBias(hiddBD[j]);
		}
	}

	void clearLearned()
	{
		for (int i = 0; i < OUTPUTS; i++)
		{
			dCdAOuput[i] = 0.0f;
			outBD[i] = 0.0f;
			for (int k = 0; k < HIDDENS; k++)
			{
				outWD[i][k] = 0.0f;
			}
		}
		for (int i = 0; i < HIDDENS; i++)
		{
			dCdAHidden[i] = 0.0f;
			hiddBD[i] = 0.0f;
			for (int k = 0; k < INPUTS; k++)
			{
				hiddWD[i][k] = 0.0f;
			}
		}
	}
	//============================================================

	float transform(float input)
	{
		return sigmoid(input);
	}

	float transformPrime(float input)
	{
		return sigPrime(input);
	}

	//============================================================

	float sigPrime(float input)
	{
		return sigmoid(input) * (1- sigmoid(input));
	}

	float sigmoid(float value)
	{
		return  1.0f / (1 + exp(-value));
	}

	//============================================================

private:

	neuron * inputs[INPUTS];

	neuron * hiddens[HIDDENS];

	neuron * outputs[OUTPUTS];

	std::vector<float> y;

	std::vector<float> dCdAOuput;

	std::vector<float> dCdAHidden;

	std::vector<std::vector<float>> outWD;

	std::vector<float> outBD;

	std::vector<std::vector<float>> hiddWD;

	std::vector<float> hiddBD;

	std::vector<float> aOut;

	std::vector<float> aHidd;

	float cost;

	int myAnswer;

	int answer;

	bool correct;
};