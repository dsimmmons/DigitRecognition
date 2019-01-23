
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
				std::cout << "x ";
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
	std::cout << "Reading Files, Please Wait" << endl << endl;
	// GET DATAFILE HERE: http://yann.lecun.com/exdb/mnist/
	reader images("Paste pathway for training images here");
	reader labels("Paste pathway for training labels here");

	reader testImages("Paste pathway for testing images here");
	reader testLabels("Paste pathway for testing labels here");

	int arg1, arg2;

	std::cout << "Input number of layers (recomended 4): ";
	std::cin >> arg1;
	std::cout << endl;
	std::cout << "Input number of neurons per hidden layer (recomended 30): ";
	std::cin >> arg2;
	std::cout << endl;

	network myNetwork(arg1, 784, arg2, 10); 
	// First Number is the number of layers. 3 is the minimum(input->hidden->output). Higher values for more hidden layers. Mess with this!
	// Second Number is the number on inputs. Keep this at 784 for the number of pixels in the training images
	// Third number is the number of neurons in each hidden layer. Mess with this!
	// Fourth number is the number of outputs. Keep this at 10 for the number of digits

	int numElements = images.getNumElements();

	int miniBatchSize; // Mess with this!

	int numTests; // Mess with this! Make sure that numTests * miniBatchSize is more than 1000 and less than 60000

	float learningRate; // Mess with this!

	std::cout << "Input number of training images per batch(recomended: 10): ";
	std::cin >> miniBatchSize;
	std::cout << endl;
	std::cout << "Input number of training batches you want to run(recomended 1000): ";
	std::cin >> numTests;
	std::cout << endl;
	std::cout << "Input learning rate(recomended 1): ";
	std::cin >> learningRate;
	std::cout << endl;

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
	char c;
	cout << "Press any key to see results" << endl;
	cin >> c;

	using namespace std::this_thread;
	using namespace std::chrono;

	int correct(0);
	float accuracy(0.0f);

	for (int m = 0; m < testImages.getNumElements(); m++)
	{
		for (int i = 0; i < 784; i++)
		{
			myNetwork.setInputValue(testImages.getPixel(m * 784 + i), i);
		}
		myNetwork.think();
		myNetwork.calculateCost(testLabels.getLabel(m));
		correct += myNetwork.isCorrect();
		drawImage(m, testImages, testLabels);
		std::cout << endl << "Machines Answer : " << myNetwork.getAnswer() << endl;
		std::cout << endl << "Accumulated Accuracy : " << 100 * ((float)(correct) / (float)(m)) << "% " << endl;
		sleep_until(system_clock::now() + seconds(1));
	}
	return 0;
}
