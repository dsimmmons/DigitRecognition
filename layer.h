#pragma once
#include <vector>

class layer
{
public:

	layer(int neuronNum, int weightNum):
		neuronCount(neuronNum),
		weightCount(weightNum)
	{
		weight.resize(neuronCount);
		for (int j = 0; j < neuronCount; j++)
		{
			weight[j].resize(weightCount);
		}
		bias.resize(neuronCount);
		a.resize(neuronCount);
		z.resize(neuronCount);
		dCdA.resize(neuronCount);
		dCdW.resize(neuronCount);
		for (int i = 0; i < neuronCount; i++)
		{
			dCdW[i].resize(weightCount);
		}
		dCdB.resize(neuronCount);
		reset();
		randomize();
	}

	//============================================================

	void randomize()
	{
		for (int j = 0; j < neuronCount; j++)
		{
			bias[j] = 2 * ((static_cast <float> (rand())) / (static_cast <float> (RAND_MAX))) - 1;

			for (int k = 0; k < weightCount; k++)
			{
				weight[j][k] = 2 * ((static_cast <float> (rand())) / (static_cast <float> (RAND_MAX))) - 1;
			}
		}
	}

	//============================================================

	void fire(layer const *prevLayer)
	{
		for (int j = 0; j < neuronCount; j++)
		{
			z[j] = 0.0f;
			for (int k = 0; k < weightCount; k++)
			{
				z[j] += prevLayer->getActivation(k) * weight[j][k];
			}
			z[j] += bias[j];
			a[j] = applyTransform(z[j]);
		}
	}

	//============================================================

	float applyTransform(float inputValue)
	{
		return 1.0f / (1 + exp(-inputValue));
	}

	//============================================================

	void applyLearnedAndReset()
	{
		for (int j = 0; j < neuronCount; j++)
		{
			for (int k = 0; k < weightCount; k++)
			{
				weight[j][k] += dCdW[j][k];
				dCdW[j][k] = 0.0f;
			}
			bias[j] += dCdB[j];
			dCdB[j] = 0.0f;
		}
	}

	void reset()
	{
		for (int j = 0; j < neuronCount; j++)
		{
			dCdA[j] = 0.0f;
			dCdB[j] = 0.0f;
			for (int k = 0; k < weightCount; k++)
			{
				dCdW[j][k] = 0.0f;
			}
		}
	}

	//============================================================

	void movedCdW(float value, int neuron, int weight)
	{
		dCdW[neuron][weight] += value;
	}

	void movedCdB(float value, int neuron)
	{
		dCdB[neuron] += value;
	}

	void movedCdA(float value, int neuron)
	{
		dCdA[neuron] += value;
	}

	//============================================================

	void inputActivation(float value, int neuron)
	{
		a[neuron] = value;
	}

	float getActivation(int neuron) const
	{
		return a[neuron]; 
	}

	int getNumNeurons() const
	{
		return neuronCount;
	}

	void inputdCdA(float value, int neuron)
	{
		dCdA[neuron] = value;
	}

	float getdCdA(int neuron) const
	{
		return dCdA[neuron];
	}

	float getZ(int neuron) const
	{
		return z[neuron];
	}

	float getWeight(int neuronIndex, int weightIndex) const
	{
		return weight[neuronIndex][weightIndex];
	}

	float moveWeight(float value, int neuronIndex, int weightIndex)
	{
		weight[neuronIndex][weightIndex] += value;
	}

private:

	int neuronCount;

	int weightCount;

	std::vector<float> a;

	std::vector<float> z;

	std::vector<std::vector<float>> weight;

	std::vector<float> bias;

	std::vector<float> dCdA;

	std::vector<std::vector<float>> dCdW;

	std::vector<float> dCdB;
};