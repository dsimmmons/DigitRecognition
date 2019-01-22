#pragma once
#include <cmath>
#include <vector>
#include "layer.h"

class network
{
public:

	network(int iNumLayers, int iNumInputs, int iNumHiddens, int iNumOutputs): 
		numLayers(iNumLayers),
		numInputs(iNumInputs),
		numHiddens(iNumHiddens),
		numOutputs(iNumOutputs)
	{
		if (numLayers < 3 || numInputs < 1 || numHiddens < 1 || numOutputs < 1)
		{
			throw std::runtime_error("Invalid Network Construction");
			return;
		}

		correctAns.resize(numOutputs);
		layerList.resize(numLayers);

		layerList[0] = new layer(numInputs, 0);
		layerList[1] = new layer(numHiddens, numInputs);
		for (int i = 1; i < (numLayers - 2); i++)
		{
			layerList[i + 1] = new layer(numHiddens, numHiddens);
		}
		layerList[numLayers-1] = new layer(numOutputs, numHiddens);
	}

	~network()
	{
		for (int i = 0; i < numLayers; i++)
		{
			delete layerList[i];
		}
	}
	//============================================================

	void setInputValue(float inputValue, int index)
	{
		layerList[0]->inputActivation(inputValue, index);
	}

	//============================================================

	void think()
	{
		for (int i = 1; i < numLayers; i++)
		{
			layer const * p = layerList[i - 1];
			layerList[i]->fire(p);
		}
	}

	//============================================================

	void calculateCost(int realAnswer)
	{
		cost = 0.0f;

		int max(0);

		for (int i = 0; i < numOutputs; i++)
		{
			if (layerList[numLayers -1]->getActivation(max) < layerList[numLayers - 1]->getActivation(i))
			{
				max = i;
			}

			if (i == realAnswer)
			{
				correctAns[i] = 1.0f;
			}
			else
			{
				correctAns[i] = 0.0f;
			}

			cost += pow((correctAns[i] - layerList[numLayers-1]->getActivation(i)), 2) * 0.5f;

			correct = 0;
			if (correctAns[max] == 1.0f)
			{
				correct = 1;
			}
			answer = max;
		}
	}

	float getCost() const
	{
		return cost;
	}

	int isCorrect() const
	{
		return correct;
	}

	int getAnswer() const
	{
		return answer;
	}

	//============================================================

	void learn(float learningRate, int batchSize) // stochastic gradient descent + back propegation
	{

		//Calculate Output Layer

		for (int j = 0; j < numOutputs; j++)
		{
			layerList[numLayers-1]->inputdCdA((layerList[numLayers-1]->getActivation(j) - correctAns[j]) * transformPrime(layerList[numLayers-1]->getZ(j)), j);

			for (int k = 0; k < numHiddens; k++)
			{
				layerList[numLayers-1]->movedCdW(-(learningRate / (float)batchSize) * layerList[numLayers-1]->getdCdA(j) * layerList[numLayers-2]->getActivation(k), j, k);
			}

			layerList[numLayers-1]->movedCdB(-(learningRate / (float)batchSize) *  layerList[numLayers-1]->getdCdA(j), j);
		}

		//Calculate Hidden Layers

		for (int i = (numLayers - 2); i > 0; i--)
		{
			for (int j = 0; j < layerList[i]->getNumNeurons(); j++)
			{
				layerList[i]->inputdCdA(0.0f, j);

				for (int k = 0; k < layerList[i + 1]->getNumNeurons(); k++)
				{
					layerList[i]->movedCdA(layerList[i + 1]->getWeight(k,j) * layerList[i + 1]->getdCdA(k) * transformPrime(layerList[i]->getZ(j)), j);
				}

				for (int k = 0; k < layerList[i - 1]->getNumNeurons(); k++)
				{
					layerList[i]->movedCdW(-(learningRate / (float)batchSize) * layerList[i]->getdCdA(j) * layerList[i - 1]->getActivation(k), j, k);
				}

				layerList[i]->movedCdB(-(learningRate / (float)batchSize) * layerList[i-1]->getdCdA(j), j);
			}
		}
	}

	void applyLearned()
	{
		for (int i = 1; i < numLayers; i++)
		{
			layerList[i]->applyLearnedAndReset();
		}
	}

	//============================================================

	float transformPrime(float inputValue) //Sigmoid
	{
		return transform(inputValue) * (1 - transform(inputValue));
	}

	float transform(float inputValue) // Sigmoid
	{
		return 1.0f / (1 + exp(-inputValue));
	}

private:

	std::vector<layer*> layerList;

	std::vector<float> correctAns;

	int numInputs;

	int numHiddens;

	int numOutputs;

	int numLayers;

	float cost;

	int correct;

	int answer;
};