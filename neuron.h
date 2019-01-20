#pragma once
#include <cstdlib>
#include "newF.h"

class neuron
{
public:

	neuron(int layer): z(0)
	{
		this->layer = layer;
	}

	int getLayer()
	{
		return layer;
	}

	//============================================================

	void addElementWeight(float value, int index)
	{
		z += (value * weights.get(index));
	}

	void applyBias()
	{
		z += bias;
	}

	//============================================================

	float getZ()
	{
		return z;
	}

	void setZ(float inputValue)
	{
		z = inputValue;
	}

	void resetZ()
	{
		z = 0.0f;
	}
	
	float getWeight(int index)
	{
		return weights.get(index);
	}

	float getBias()
	{
		return bias;
	}

	//============================================================

	void changeBias(float value)
	{
		bias += value;
	}

	void changeWeight(float value, int index)
	{
		weights.input(weights.get(index) + value, index);
	}

	//============================================================

	void randBias()
	{
		bias = 2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 1;
	}

	void randWeights()
	{
		for (int i = 0; i < weights.getSize(); i++)
		{
			weights.input((2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 1), i);
		}
	}

	//============================================================

	void resizeWeights(int size)
	{
		weights.setSize(size);
	}

private:

	int layer;
	
	float z;

	float bias;

	newFloat weights;

};