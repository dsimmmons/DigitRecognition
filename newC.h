#pragma once
/*
  ==============================================================================

	newCharArray.h
	Created: 29 Nov 2018 9:30:06pm
	Author:  denve

  ==============================================================================
*/

#pragma once
class newChar
{
public:
	newChar(int mem)
	{
		size = mem;
		newTable = new char[size];
	}
	newChar()
	{
		size = 0;
		newTable = new char[0];
	}
	~newChar()
	{
		delete[] newTable;
	}

	void input(char value, int index)
	{
		if (index >= 0 && index < size)
		{
			newTable[index] = value;
		}
	}
	unsigned char get(int index) const
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
	void add(char value)
	{
		char * temp = new char[size + 1];
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
		char * temp = new char[length];
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
	char * point()
	{
		return newTable;
	}
private:
	char* newTable;
	int size;
};
