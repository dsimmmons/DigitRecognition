/*
  ==============================================================================

	newFloatArray.h
	Created: 25 Nov 2018 12:49:26pm
	Author:  denve

  ==============================================================================
*/

#pragma once

class newFloat
{
public:
	newFloat(int mem)
	{
		size = mem;
		newTable = new float[size];
	}
	newFloat()
	{
		size = 0;
		newTable = new float[size];
	}
	~newFloat()
	{
		delete[] newTable;
	}

	void input(float value, int index)
	{
		if (index >= 0 && index < size)
		{
			newTable[index] = value;
		}
	}
	float get(int index)
	{
		if (index >= 0 && index < size)
		{
			return newTable[index];
		}
		else
		{
			return 0;
		}
	}
	void add(float value)
	{
		float * temp = new float[size + 1];
		for (int i = 0; i < size; i++)
		{
			temp[i] = newTable[i];
		}
		temp[size] = value;
		delete[] newTable;
		newTable = temp;
		size++;
	}
	void setSize(int length)
	{
		float * temp = new float[length];
		if (size <= length)
		{
			for (int i = 0; i < size; i++)
			{
				temp[i] = newTable[i];
			}
		}
		else
		{
			for (int i = 0; i < length; i++)
			{
				temp[i] = newTable[i];
			}
		}
		delete[] newTable;
		newTable = temp;
		size = length;
	}
	int getSize()
	{
		return size;
	}
	void clear()
	{
		for (int i = 0; i < size; i++)
		{
			newTable[i] = 0.0f;
		}
	}
	float * point()
	{
		return newTable;
	}
private:
	float* newTable;
	int size;
};
