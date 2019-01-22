
#include "reader.h"
#include "network.h"
#include <chrono>
#include <thread>

void drawImage(int image, reader a, reader b)
{
	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			if ((int)(10 * (a.getPixel(image * 784 + i * 28 + j))) > 7)
			{
				std::cout << "X ";
			}
			else if ((int)(10 * (a.getPixel(image * 784 + i * 28 + j))) > 3)
			{
				std::cout << "o ";
			}
			else
			{
				std::cout << ". ";
			}
		}
		std::cout << endl;
	}
}

int main()
{
	reader images("C:\\Users\\denve\\OneDrive\\Documents\\MLData\\MNISTDatabase\\MNIST\\train-images-idx3-ubyte\\train-images.idx3-ubyte");
	reader labels("C:\\Users\\denve\\OneDrive\\Documents\\MLData\\MNISTDatabase\\MNIST\\train-labels-idx1-ubyte\\train-labels.idx1-ubyte");

	reader testImages("C:\\Users\\denve\\OneDrive\\Documents\\MLData\\MNISTDatabase\\MNIST\\t10k-images-idx3-ubyte\\testImages.idx3-ubyte");
	reader testLabels("C:\\Users\\denve\\OneDrive\\Documents\\MLData\\MNISTDatabase\\MNIST\\t10k-labels-idx1-ubyte\\trainLabels.idx1-ubyte");

	network myNetwork(4, 784, 30, 10); 
	// First Number is the number of layers. 3 is the minimum(input->hidden->output). Higher values for more hidden layers. Mess with this!
	// Second Number is the number on inputs. Keep this at 784 for the number of pixels in the training images
	// Third number is the number of neurons in each hidden layer. Mess with this!
	// Fourth number is the number of outputs. Keep this at 10 for the number of digits

	int numElements = images.getNumElements();

	int numTests = 100; // Mess with this! Make sure that numTests * miniBatchSize is more than 1000 and less than 60000

	int miniBatchSize(10); // Mess with this!

	float learningRate(1.5f); // Mess with this!

	float myBatchCost(0.3f);

	float batchAccuracy;

	int imageIndex;

	//Start Training

	for (int b = 0; b < numTests; b++)
	{
		myBatchCost = 0.0f;
		batchAccuracy = 0.0f;
		for (int m = 0; m < miniBatchSize; m++)
		{
			imageIndex = b * miniBatchSize + m;
			for (int i = 0; i < 784; i++)
			{
				myNetwork.setInputValue(images.getPixel(imageIndex * 784 + i), i);
			}

			myNetwork.think();
			myNetwork.calculateCost(labels.getLabel(imageIndex));
			myNetwork.learn(learningRate, miniBatchSize);
			myBatchCost += myNetwork.getCost() / miniBatchSize;
			batchAccuracy += ((float)myNetwork.isCorrect() / (float)miniBatchSize);
		}
		myNetwork.applyLearned();
		std::cout << "Batch : " << b + 1 << " / " << numTests << " Accuracy : " << batchAccuracy * 100 <<  "% Cost : " << myBatchCost << endl;
	}

	std::cout << endl << "Done Training!" << endl << endl;

	//=======================================================================================================================================================================================

	// See your results

	do
	{
		cout << "Type ENTER to see results" << endl;
	} while (cin.get() != '\n');

	using namespace std::this_thread;
	using namespace std::chrono;

	for (int m = 0; m < testImages.getNumElements(); m++)
	{
		for (int i = 0; i < 784; i++)
		{
			myNetwork.setInputValue(testImages.getPixel(m * 784 + i), i);
		}
		myNetwork.think();
		myNetwork.calculateCost(testLabels.getLabel(m));
		drawImage(m, testImages, testLabels);
		std::cout << endl << "Machines Answer : " << myNetwork.getAnswer() << endl;
		sleep_until(system_clock::now() + seconds(1));
	}
	return 0;
}