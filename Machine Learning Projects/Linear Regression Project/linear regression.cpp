//============================================================================
// Name        : linear.cpp
// Author      : me sam
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//IMPORTANT NOTE:
//please store linear_regression_data.txt in the same directory as this program

#include <iostream>
#include <fstream>
#include <stdlib.h>
//#include <LIMITS.H>
#include <math.h>
#include <cmath>
using namespace std;

const int DATA_SIZE = 398;
const int TEST_DATA_SIZE = 3;
const double LEARNING_RATE = 0.001;

struct Car {
    double mpg;
    double cylinders;
    double displacement;
    double horsepower;
	double weight;
	double acceleration;
};

Car data[DATA_SIZE];
Car testData[TEST_DATA_SIZE];

void inputData();
double Hypothesis (double, double, double, double, double, double, Car);
double Cost(double, double, double, double, double, double);
Car Gradient(double, double, double, double, double, double);

int main() {
	inputData();

	double thetaIntercept = 1;
	double thetaCyl = 1;
	double thetaDis = 1;
	double thetaHorse = 1;
	double thetaWeight = 1;
	double thetaAcc = 1;
	bool convergence = false;
	double cost;
	double previous_cost = 0;
	int count = 0;

	while (convergence == false) {
		cost = Cost(thetaIntercept, thetaCyl, thetaDis, thetaHorse, thetaWeight, thetaAcc);
		cost = cost / DATA_SIZE;
		if(abs(cost - previous_cost) < .00001) {
		//if(false) {
			convergence = true;
		}
		else {
			count++;


			Car newGradients = Gradient(thetaIntercept, thetaCyl, thetaDis, thetaHorse, thetaWeight, thetaAcc);
			thetaIntercept += newGradients.mpg;
			thetaCyl += newGradients.cylinders;
			thetaDis += newGradients.displacement;
			thetaHorse += newGradients.horsepower;
			thetaWeight += newGradients.weight;
			thetaAcc += newGradients.acceleration;

			/*cout << "newGradients.mpg " << newGradients.mpg << endl;
			cout << "cost: " << cost << " previous cost: " << previous_cost << " difference: " << cost-previous_cost
					<< " cost unaveraged: " << cost*DATA_SIZE << endl;*/
			if(count > 100000) {
				convergence = true;
			}

			previous_cost = cost;
		}

	}

	cout << "intercept weight: " << thetaIntercept << endl;
	cout << "cylinders weight: " << thetaCyl << endl;
	cout << "displacement weight: " << thetaDis << endl;
	cout << "horsepower weight: " << thetaHorse << endl;
	cout << "weight weight: " << thetaWeight << endl;
	cout << "acceleration weight: " << thetaAcc << endl << endl;

	for(int count = 0; count < TEST_DATA_SIZE; count++) {
		double hypo = Hypothesis(thetaIntercept, thetaCyl, thetaDis, thetaHorse, thetaWeight, thetaAcc, testData[count]);
		cout << "test data #" << count+1 << " predicted result: " << hypo << " mpg" << endl;
	}

	return 0;
}

double Hypothesis (double thetaIntercept, double thetaCyl, double thetaDis, double thetaHorse, double thetaWeight,
		double thetaAcc, Car values) {

	return thetaIntercept + (thetaCyl * values.cylinders) + (thetaDis * values.displacement) + (thetaHorse * values.horsepower)
			+ (thetaWeight * values.weight) + (thetaAcc + values.acceleration);
}

double Cost(double thetaIntercept, double thetaCyl, double thetaDis, double thetaHorse, double thetaWeight, double thetaAcc){

	double cost = 0;
	for(int count = 0; count < DATA_SIZE; count++) {
		double hypo = Hypothesis(thetaIntercept, thetaCyl, thetaDis, thetaHorse, thetaWeight, thetaAcc, data[count]);
		cost += pow(hypo - data[count].mpg, 2);
	}
	cost = cost / 2;
	return cost;
}

Car Gradient(double thetaIntercept, double thetaCyl, double thetaDis, double thetaHorse, double thetaWeight, double thetaAcc){
		//double interceptCurrent, double cylCurrent, double disCurrent, double horseCurrent, double weightCurrent,
		//double accCurrent, double learningRate) {
		//double learningRate) {
	double interceptGradient = 0;
	double cylGradient = 0;
	double disGradient = 0;
	double horseGradient = 0;
	double weightGradient = 0;
	double accGradient = 0;

	for (int count = 0; count < DATA_SIZE; count++) {
		double hypo = Hypothesis(thetaIntercept, thetaCyl, thetaDis, thetaHorse, thetaWeight, thetaAcc, data[count]);
		interceptGradient = interceptGradient + (data[count].mpg - hypo);
		cylGradient = cylGradient + (data[count].mpg - hypo) * data[count].cylinders;
		disGradient = disGradient + (data[count].mpg - hypo) * data[count].displacement;
		horseGradient = horseGradient + (data[count].mpg - hypo) * data[count].horsepower;
		weightGradient = weightGradient + (data[count].mpg - hypo) * data[count].weight;
		accGradient = accGradient + (data[count].mpg - hypo) * data[count].acceleration;
	}

	interceptGradient = interceptGradient * LEARNING_RATE;
	cylGradient = cylGradient * LEARNING_RATE;
	disGradient = disGradient * LEARNING_RATE;
	horseGradient = horseGradient * LEARNING_RATE;
	weightGradient = weightGradient * LEARNING_RATE;
	accGradient = accGradient * LEARNING_RATE;

	Car gradients;
	gradients.mpg = interceptGradient; //using mpg to store intercept gradient, because mpg has no gradient
	gradients.cylinders = cylGradient;
	gradients.displacement = disGradient;
	gradients.horsepower = horseGradient;
	gradients.weight = weightGradient;
	gradients.acceleration = accGradient;

	return gradients;
}

void inputData() {
	ifstream cars;
	cars.open("linear_regression_data.txt");
	string inputLine;
	string inputCurrent;
	string inputField;
	double inputDoubled;
	int split;

	Car minCar;
	minCar.cylinders = 2147483647;
	minCar.displacement = 2147483647;
	minCar.horsepower = 2147483647;
	minCar.weight = 2147483647;
	minCar.acceleration = 2147483647;

	Car maxCar;
		maxCar.cylinders = 0;
		maxCar.displacement = 0;
		maxCar.horsepower = 0;
		maxCar.weight = 0;
		maxCar.acceleration = 0;

	for (int count = 0; count < DATA_SIZE; ++count) {
		getline(cars, inputLine);

		inputCurrent = inputLine;

		//get mpg
		split = inputCurrent.find(9);
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());
		data[count].mpg = inputDoubled;

		inputCurrent = inputCurrent.substr(split+1);

		//get cylinders
		split = inputCurrent.find(9);
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());
		data[count].cylinders = inputDoubled;
		if(inputDoubled < minCar.cylinders) {
			minCar.cylinders = inputDoubled;
		}
		if(inputDoubled > maxCar.cylinders) {
			maxCar.cylinders = inputDoubled;
		}

		inputCurrent = inputCurrent.substr(split+1);

		//get displacement
		split = inputCurrent.find(9);
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());
		data[count].displacement = inputDoubled;
		if(inputDoubled < minCar.displacement) {
			minCar.displacement = inputDoubled;
		}
		if(inputDoubled > maxCar.displacement) {
			maxCar.displacement = inputDoubled;
		}

		inputCurrent = inputCurrent.substr(split+1);

		//get horsepower
		split = inputCurrent.find(9);
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());
		data[count].horsepower = inputDoubled;
		if(inputDoubled < minCar.horsepower) {
			minCar.horsepower = inputDoubled;
		}
		if(inputDoubled > maxCar.horsepower) {
			maxCar.horsepower = inputDoubled;
		}

		inputCurrent = inputCurrent.substr(split+1);

		//get weight
		split = inputCurrent.find(9);
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());
		data[count].weight = inputDoubled;
		if(inputDoubled < minCar.weight) {
			minCar.weight = inputDoubled;
		}
		if(inputDoubled > maxCar.weight) {
			maxCar.weight = inputDoubled;
		}

		inputCurrent = inputCurrent.substr(split+1);

		//get acceleration
		split = inputCurrent.find(9);
		inputField = inputCurrent.substr(0, split);
		inputDoubled = atof(inputField.c_str());
		data[count].acceleration = inputDoubled;
		if(inputDoubled < minCar.acceleration) {
			minCar.acceleration = inputDoubled;
		}
		if(inputDoubled > maxCar.acceleration) {
			maxCar.acceleration = inputDoubled;
		}
	}

	for (int count = DATA_SIZE; count < DATA_SIZE + TEST_DATA_SIZE; ++count) {
			getline(cars, inputLine);

			inputCurrent = inputLine;

			//get mpg
			split = inputCurrent.find(9);
			/*inputField = inputCurrent.substr(0, split);
			inputDoubled = atof(inputField.c_str());
			data[count].mpg = inputDoubled;*/

			inputCurrent = inputCurrent.substr(split+1);

			//get cylinders
			split = inputCurrent.find(9);
			inputField = inputCurrent.substr(0, split);
			inputDoubled = atof(inputField.c_str());
			testData[count-DATA_SIZE].cylinders = inputDoubled;
			if(inputDoubled < minCar.cylinders) {
				minCar.cylinders = inputDoubled;
			}
			if(inputDoubled > maxCar.cylinders) {
				maxCar.cylinders = inputDoubled;
			}

			inputCurrent = inputCurrent.substr(split+1);

			//get displacement
			split = inputCurrent.find(9);
			inputField = inputCurrent.substr(0, split);
			inputDoubled = atof(inputField.c_str());
			testData[count-DATA_SIZE].displacement = inputDoubled;
			if(inputDoubled < minCar.displacement) {
				minCar.displacement = inputDoubled;
			}
			if(inputDoubled > maxCar.displacement) {
				maxCar.displacement = inputDoubled;
			}

			inputCurrent = inputCurrent.substr(split+1);

			//get horsepower
			split = inputCurrent.find(9);
			inputField = inputCurrent.substr(0, split);
			inputDoubled = atof(inputField.c_str());
			testData[count-DATA_SIZE].horsepower = inputDoubled;
			if(inputDoubled < minCar.horsepower) {
				minCar.horsepower = inputDoubled;
			}
			if(inputDoubled > maxCar.horsepower) {
				maxCar.horsepower = inputDoubled;
			}

			inputCurrent = inputCurrent.substr(split+1);

			//get weight
			split = inputCurrent.find(9);
			inputField = inputCurrent.substr(0, split);
			inputDoubled = atof(inputField.c_str());
			testData[count-DATA_SIZE].weight = inputDoubled;
			if(inputDoubled < minCar.weight) {
				minCar.weight = inputDoubled;
			}
			if(inputDoubled > maxCar.weight) {
				maxCar.weight = inputDoubled;
			}

			inputCurrent = inputCurrent.substr(split+1);

			//get acceleration
			split = inputCurrent.find(9);
			inputField = inputCurrent.substr(0, split);
			inputDoubled = atof(inputField.c_str());
			testData[count-DATA_SIZE].acceleration = inputDoubled;
			if(inputDoubled < minCar.acceleration) {
				minCar.acceleration = inputDoubled;
			}
			if(inputDoubled > maxCar.acceleration) {
				maxCar.acceleration = inputDoubled;
			}

		}

	//normalize data
	for(int count = 0; count < DATA_SIZE; count++){
		data[count].cylinders = (data[count].cylinders - minCar.cylinders) / (maxCar.cylinders - minCar.cylinders);
		data[count].displacement = (data[count].displacement - minCar.displacement) / (maxCar.displacement - minCar.displacement);
		data[count].horsepower = (data[count].horsepower - minCar.horsepower) / (maxCar.horsepower - minCar.horsepower);
		data[count].weight = (data[count].weight - minCar.weight) / (maxCar.weight - minCar.weight);
		data[count].acceleration = (data[count].acceleration - minCar.acceleration) / (maxCar.acceleration - minCar.acceleration);
	}

	for(int count = 0; count < TEST_DATA_SIZE; count++){
		testData[count].cylinders = (data[count].cylinders - minCar.cylinders) / (maxCar.cylinders - minCar.cylinders);
		testData[count].displacement = (data[count].displacement - minCar.displacement) / (maxCar.displacement - minCar.displacement);
		testData[count].horsepower = (data[count].horsepower - minCar.horsepower) / (maxCar.horsepower - minCar.horsepower);
		testData[count].weight = (data[count].weight - minCar.weight) / (maxCar.weight - minCar.weight);
		testData[count].acceleration = (data[count].acceleration - minCar.acceleration) / (maxCar.acceleration - minCar.acceleration);
	}

	cars.close();
}
