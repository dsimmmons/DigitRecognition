#include "reader.h"
#include "network.h"
#include <cstdlib>

#define INPUTS 784
#define IMAGES 50000
#define MINIBATCH 10

void drawImage(int image, reader a, reader b)
{
	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			if ((int)(10 * (a.getPixel(image * 784 + i * 28 + j))) > 5)
			{
				std::cout << "X ";
			}
			else
			{
				std::cout << ". ";
			}
		}
		std::cout << endl;
	}
	std::cout << b.getInt(image) << endl;
}

int main()
{
	reader images("C:\\Users\\denve\\OneDrive\\Documents\\MLData\\MNISTDatabase\\MNIST\\train-images-idx3-ubyte\\train-images.idx3-ubyte");
	reader labels("C:\\Users\\denve\\OneDrive\\Documents\\MLData\\MNISTDatabase\\MNIST\\train-labels-idx1-ubyte\\train-labels.idx1-ubyte");
	
	network myNetwork;

	float learningRate = 3;

	int correct;

	float accuracy;

	float myBatchCost = 0.3;

	int imageIndex;

		for (int b = 0; b < ((float)IMAGES / (float)MINIBATCH); b++)
		{
			myBatchCost = 0.0f;
			myNetwork.clearLearned();
			correct = 0;

			for (int m = 0; m < MINIBATCH; m++)
			{
				imageIndex = (int)(IMAGES * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
				for (int i = 0; i < INPUTS; i++)
				{
					myNetwork.setInputValue(images.getPixel((imageIndex * INPUTS) + i), i);
				}

				myNetwork.think();
				myNetwork.calculateCost(labels.getInt(imageIndex));
				if (myNetwork.isCorrect())
				{
					correct++;
				}
				myNetwork.learnErrors(learningRate, MINIBATCH);
				myBatchCost += myNetwork.getCost();

			}
			//drawImage(imageIndex, images, labels);
			//std::cout << "Brain Guess: " << myNetwork.getAnswer() << endl;
			myNetwork.applyLearned();

			accuracy = 100.0f * ((float)correct / MINIBATCH);
			myBatchCost = myBatchCost / (20 * MINIBATCH);

			std::cout << "Batch : " << b + 1 << " Accuracy : ";
			if (accuracy < 10)
			{
				std::cout << 0;
			}
			std::cout << accuracy << "% Cost : " << myBatchCost << endl;

		}
	std::cout << "Done" << endl;
	return 0;
}
