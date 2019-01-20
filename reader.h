#pragma once
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <vector>

using namespace std;

class reader
{
public:

	reader(const char fileName[])
	{

		char c;
		int index = 0;

		ifstream infile;
		infile.open(fileName, ios::binary | ios::in);

		fileSize = filesize(fileName);
		fileMapV.resize(fileSize);

		for (int i = 0; i < fileSize; i++)
		{
			infile.read(&c, 1);
			fileMapV[i] = c;
		}

		infile.close();

		readHeader();

	}

	//==============================================================

	float getPixel(int nIndex)
	{
		int a = fileMapV[nIndex + dataIndex];
		return ((float)a / 255);
	}

	int getInt(int nIndex)
	{
		return fileMapV[nIndex + dataIndex];
	}

private:

	std::ifstream::pos_type filesize(const char* filename)
	{
		std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
	}

	//==============================================================

	void readHeader()
	{
		magicNumber = getI32(0);

		if ((magicNumber != 2051) && (magicNumber != 2049))
		{
			throw std::runtime_error("Number isn't magic");
			return;
		}

		numElements = getI32(4);
		
		if (magicNumber == 2051)
		{
			rows = getI32(8);
			columns = getI32(12);
			dataIndex = 16;
		}
		else
		{
			dataIndex = 8;
		}
	}

	//==============================================================

	int32_t getI32(int nIndex) const
	{
		return fileMapV[nIndex + 3] | (fileMapV[nIndex + 2] << 8) | (fileMapV[nIndex + 1] << 16) | (fileMapV[nIndex + 0] << 24);
	}

	//==============================================================

	int fileSize;

	int32_t numElements;

	int32_t rows;

	int32_t columns;

	int32_t magicNumber;

	int32_t dataIndex;

	std::vector<unsigned char> fileMapV;
};