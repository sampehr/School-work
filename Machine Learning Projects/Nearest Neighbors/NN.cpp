//============================================================================
// Name        : NN.cpp
// Author      : me sam
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//IMPORTANT NOTE #1:
//please store iris.txt in the same directory as this program

//IMPORTANT NOTE #2:
//on my home machine, this program compiles and runs fine. while testing this program by remote accessing a linux lab
//computer, however, this program outputs "ERROR: DATA HAS INVALID CLASSIFICATION" and asserts
//there seems to be a difference in the number of characters received when reading the txt file between these two machines
//if you recievce the "ERROR: DATA HAS INVALID CLASSIFICATION" output, please comment out line 274 and uncomment line 275

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
//#include <LIMITS.H>
using namespace std;

const int DATA_SIZE = 147;
const int TEST_DATA_SIZE = 3;
const int K1 = 1;
const int K2 = 3;
const int K3 = 5; //assumed to be the biggest k

const double SEPAL_LENGTH_MINIMUM = 4.3;
const double SEPAL_WIDTH_MINIMUM = 2.0;
const double PETAL_LENGTH_MINIMUM = 1.0;
const double PETAL_WIDTH_MINIMUM = 0.1;
const double SEPAL_LENGTH_RANGE = 7.9 - SEPAL_LENGTH_MINIMUM;
const double SEPAL_WIDTH_RANGE = 4.4 - SEPAL_WIDTH_MINIMUM;
const double PETAL_LENGTH_RANGE = 6.9 - PETAL_LENGTH_MINIMUM;
const double PETAL_WIDTH_RANGE = 2.5 - PETAL_WIDTH_MINIMUM;

/*const double SEPAL_LENGTH_MAXIMUM = 7.9;
const double SEPAL_WIDTH_MAXIMUM = 4.4;
const double PETAL_LENGTH_MAXIMUM = 6.9;
const double PETAL_WIDTH_MAXIMUM = 2.5;*/

struct Plant {
	double sepalLength;
	double sepalWidth;
	double petalLength;
	double petalWidth;
	string classification;
};

Plant data[DATA_SIZE];
Plant testData[TEST_DATA_SIZE];

void dataInput();
void normalizeTestData();
double findDistance(Plant, Plant);
void findClassifications(int);

int main() {
	dataInput();
	normalizeTestData();

	findClassifications(K1);
	cout << "k = 1 classification: " << endl;
	for (int count = 0; count < TEST_DATA_SIZE; count++) {
		cout << "test data " << count << " classification: " << testData[count].classification << endl;
	}
	cout << endl;

	findClassifications(K2);
	cout << "k = 3 classification: " << endl;;
	for (int count = 0; count < TEST_DATA_SIZE; count++) {
		cout << "test data " << count << " classification: " << testData[count].classification << endl;
	}
	cout << endl;

	findClassifications(K3);
	cout << "k = 5 classification: " << endl;
	for (int count = 0; count < TEST_DATA_SIZE; count++) {
		cout << "test data " << count << " classification: " << testData[count].classification << endl;
	}
	cout << endl;


	return 0;
}

void findClassifications(int k) {
	double minimums[K3]; //assumed to be the biggest k
	int minimumPositions[K3];
		for(int count = 0; count < K3; count++) {
			minimums[count] = 2147483647;
		}

		for (int testNum = 0; testNum < TEST_DATA_SIZE; ++testNum) {

			for(int dataNum = 0; dataNum < DATA_SIZE; dataNum++) {
				double distance = findDistance(data[dataNum], testData[testNum]);
				//cout << "distance: " << distance << endl;

				for(int count = 0; count < k; count++) {
					//cout << "outside if" << endl;
					if(distance < minimums[count]) {
						//cout << "inside if" << endl;
						minimums[count] = distance;
						minimumPositions[count] = dataNum;
						count = k;
					}
					//cout << "if done" << endl << endl;
				}
			}

			int setosaCount = 0;
			int versicolorCount = 0;
			int virginicaCount = 0;
			string setosa = "Iris-setosa";
			string versicolor ="Iris-versicolor";
			string virginica = "Iris-virginica";

			for(int count = 0; count < k; count++) {
				/*cout << "minimumPositions[count]: " << minimumPositions[count] << endl;
				cout << "data[ minimumPositions[count] ].classification: " << data[ minimumPositions[count] ].classification << endl;
				cout << "minimums[count]: " << minimums[count] << endl;*/

				if (data[ minimumPositions[count] ].classification.compare(setosa) == 0) {
					setosaCount++;
				}
				else if (data[ minimumPositions[count] ].classification.compare(versicolor) == 0) {
					versicolorCount++;
				}
				else if (data[ minimumPositions[count] ].classification.compare(virginica) == 0) {
					virginicaCount++;
				}
				else {
					cout << "ERROR: DATA HAS INVALID CLASSIFICATION" << endl;
					cout << "invlid class: " << data[ minimumPositions[count] ].classification << endl;
					assert(0);
				}
			}

			//cout << "setosaCount: " << setosaCount << " versicolorCount: " << versicolorCount << " virginicaCount: "
			//		<< virginicaCount << endl;
			if(setosaCount >= versicolorCount && setosaCount >= virginicaCount) {
				testData[testNum].classification = setosa;
			}
			else if (versicolorCount >= setosaCount && versicolorCount >= virginicaCount) {
				testData[testNum].classification = versicolor;
			}
			else if (virginicaCount >= setosaCount && virginicaCount >= versicolorCount) {
				testData[testNum].classification = virginica;
			}
			else {
				cout << "ERROR: COULD NOT CLASSIFY TEST DATA" << endl;
				assert(0);
			}
			//cout << "classification: " << testData[testNum].classification << endl;
		}
}

double findDistance(Plant p1, Plant p2){
	//cout << "p1.sepalLength: " << p1.sepalLength << " p2.sepalLength: " << p2.sepalLength << endl;
	double sepalLength = pow(p1.sepalLength - p2.sepalLength, 2);
	double sepalWidth = pow(p1.sepalWidth - p2.sepalWidth, 2);
	double petalLength = pow(p1.petalLength - p2.petalLength, 2);
	double petalWidth = pow(p1.petalWidth - p2.petalWidth, 2);
	double distance = sqrt(sepalLength + sepalWidth + petalLength + petalWidth);

	//cout << "sepalLength: " << sepalLength << " sepalWidth: " << sepalWidth << " petalLength: " << petalLength <<
	//		" petalWidth: " << petalWidth << " distance: " << distance << endl;
	return distance;
}

void normalizeTestData(){
	/*______test data______
	  4.9, 3.0, 1.4, 0.2, ?
	  4.9, 2.4, 3.3, 1.0, ?
	  4.9, 2.5, 4.5, 1.7, ?*/

	testData[0].sepalLength = 4.9;
	testData[0].sepalWidth = 3.0;
	testData[0].petalLength = 1.4;
	testData[0].petalWidth = 0.2;

	testData[1].sepalLength = 4.9;
	testData[1].sepalWidth = 2.4;
	testData[1].petalLength = 3.3;
	testData[1].petalWidth = 1.0;

	testData[2].sepalLength = 4.9;
	testData[2].sepalWidth = 2.5;
	testData[2].petalLength = 4.5;
	testData[2].petalWidth = 1.7;

	for(int count = 0; count < TEST_DATA_SIZE; count++) {
		testData[count].sepalLength = (testData[count].sepalLength - SEPAL_LENGTH_MINIMUM) / SEPAL_LENGTH_RANGE;
		testData[count].sepalWidth = (testData[count].sepalWidth - SEPAL_WIDTH_MINIMUM) / SEPAL_WIDTH_RANGE;
		testData[count].petalLength = (testData[count].petalLength - PETAL_LENGTH_MINIMUM) / PETAL_LENGTH_RANGE;
		testData[count].petalWidth = (testData[count].petalWidth - PETAL_LENGTH_MINIMUM) / PETAL_LENGTH_RANGE;
	}

	/*for(int count = 0; count < TEST_DATA_SIZE; count++) {
			testData[count].sepalLength = testData[count].sepalLength / SEPAL_LENGTH_MAXIMUM;
			testData[count].sepalWidth = testData[count].sepalWidth / SEPAL_WIDTH_MAXIMUM;
			testData[count].petalLength = testData[count].petalLength / PETAL_LENGTH_MAXIMUM;
			testData[count].petalWidth = testData[count].petalWidth / PETAL_LENGTH_MAXIMUM;
		}*/
}

void dataInput() {
	ifstream iris;
	iris.open("iris.txt");
	string inputLine;
	string inputField;
	string inputCurrent;
	double inputDoubled;
	double inputNormalized;
	int split;

	for (int count = 0; count < DATA_SIZE; count++ ) {
		//get next Plant
		getline(iris, inputLine);
		//cout << inputLine << endl;
		inputCurrent = inputLine;

		//get sepalLength
		split = inputCurrent.find(',');
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());

		//normalize and store sepalLength
		inputNormalized = (inputDoubled - SEPAL_LENGTH_MINIMUM) / SEPAL_LENGTH_RANGE;
		//inputNormalized = inputDoubled / SEPAL_LENGTH_MAXIMUM;
		data[count].sepalLength = inputNormalized;

		inputCurrent = inputCurrent.substr(split+1);

		//get sepalWidth
		split = inputCurrent.find(',');
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());

		//normalize and store sepalWidth
		inputNormalized = (inputDoubled - SEPAL_WIDTH_MINIMUM) / SEPAL_WIDTH_RANGE;
		//inputNormalized = inputDoubled / SEPAL_WIDTH_MAXIMUM;
		data[count].sepalWidth = inputDoubled;

		inputCurrent = inputCurrent.substr(split+1);

		//get petalLength
		split = inputCurrent.find(',');
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());

		//normalize and store petalLength
		inputNormalized = (inputDoubled - PETAL_LENGTH_MINIMUM) / PETAL_LENGTH_RANGE;
		//inputNormalized = inputDoubled / PETAL_LENGTH_MAXIMUM;
		data[count].petalLength = inputDoubled;

		inputCurrent = inputCurrent.substr(split+1);

		//get petalWidth
		split = inputCurrent.find(',');
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());

		//normalize and store petalWidth
		inputNormalized = (inputDoubled - PETAL_WIDTH_MINIMUM) / PETAL_WIDTH_RANGE;
		//inputNormalized = inputDoubled / PETAL_WIDTH_MAXIMUM;
		data[count].petalWidth = inputDoubled;

		inputCurrent = inputCurrent.substr(split+1);

		//get classification
		inputField = inputCurrent.substr(0, inputCurrent.size()-1); //comment this line out if assert happens!
		//inputField = inputCurrent.substr(0, inputCurrent.size()); //uncomment this line is assert happens!
		data[count].classification = inputField;

		//cout << data[count].classification<<endl;
	}

	iris.close();
}
