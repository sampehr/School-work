//============================================================================
// Name        : k-means.cpp
// Author      : Sam Pehr
// Description : k-means algorithm
// Run Note    : I compiled this in eclipse on windows. Simply compile and run - the program asks for input on whether
//               to randomize data or input your own initial cluster centers.
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <list>
#include <time.h>
#include <cmath>
#include <array>
#include <assert.h>
using namespace std;

const int DATA_SIZE = 8;
const int k = 3;

struct Point {
	double xCoord;
	double yCoord;
};

Point data [DATA_SIZE] = {
		{2, 10},
		{2, 5},
		{8, 4},
		{5, 8},
		{7, 5},
		{6, 4},
		{1, 2},
		{4, 9}
};

Point centers [k];

void similarity(list<int>[k]);

int main() {

	list<int> clusters[k];
	for(int count = 0; count < k; count++){
		clusters[count].insert(clusters[count].begin(), -1);
	}

	bool randomFlag = false;
	char charInput;
	cout << "randomly assign initial cluster centers? [valid input: y or n]";
	cin >> charInput;
	switch(charInput){
	case 'y':
		randomFlag = true;
		break;
	case 'n':
		randomFlag = false;
		break;
	default:
		cout << endl << "invlid input. program terminating." << endl;
		assert(0);
	}

	if(randomFlag == true) {
		//randomly assign cluster centers
		srand(time(NULL));
		cout << "randomly assigning cluster centers" << endl;
		cout << "cluster centers: ";
		for(int count = 0; count < k; count++) {
			int new_center = rand() % DATA_SIZE; //randomly find new cluster center

			for(int count2 = 0; count2 < k; count2++){ //determine if new cluster center has already been used
				if( new_center == *(clusters[count2].begin()) ) {
					new_center = rand() % DATA_SIZE; //if already used, run rand again and recheck
					count2 = -1;
				}
			}
			clusters[count].erase(clusters[count].begin());
			clusters[count].insert(clusters[count].begin(), new_center);
			centers[count].xCoord = data[new_center].xCoord;
			centers[count].yCoord = data[new_center].yCoord;
			cout << "A" << new_center + 1 << " ";
		}
		cout << endl;

		for(int count = 0; count < k; count++) {
			clusters[count].erase(clusters[count].begin());
		}

	}
	else {
		cout << "input centers" << endl;
		for(int count = 0; count < k; count++) {
			int input;
			bool inputValid = false;
			while(inputValid == false) {
				inputValid = true;
				cout << " starting k center A" << count << " [valid inputs: 1-" << DATA_SIZE << "]" << endl;
				cin >> input;
				if(input < 1 || input > DATA_SIZE){
					cout << "invalid input. please try again." << endl;
					inputValid = false;
				}
			}


			cout << endl;

			clusters[count].erase(clusters[count].begin());
			centers[count].xCoord = data[input-1].xCoord;
			centers[count].yCoord = data[input-1].yCoord;

		}
	}

	//assign every point to a cluster
	similarity(clusters);

	bool change = true;

	while(change == true) {
		change = false;
		for(int clustercount = 0; clustercount < k; clustercount++){
			list<int>::iterator clusterPtr = clusters[clustercount].begin();\
			double xAverage = 0;
			double yAverage = 0;
			int pointTotal = 0;
			while(clusterPtr != clusters[clustercount].end()) {
				xAverage += data[*(clusterPtr)].xCoord;
				yAverage += data[*(clusterPtr)].yCoord;
				pointTotal++;
				clusterPtr++;
			}

			if(pointTotal !=0){
				xAverage = xAverage / pointTotal;
				yAverage = yAverage / pointTotal;
			}
			if( ( (centers[clustercount].xCoord != xAverage) || (centers[clustercount].yCoord != yAverage) )){
				change = true;
				centers[clustercount].xCoord = xAverage;
				centers[clustercount].yCoord = yAverage;
			}
		}

		if(change == true) {
			similarity(clusters);
		}
	}

	for(int count = 0; count < k; count++) {
		cout << "cluster: " << count << endl;
		list<int>::iterator clusterPtr = clusters[count].begin();
		while(clusterPtr != clusters[count].end()){
			cout << "A" << (*(clusterPtr)) + 1 << " ";
			clusterPtr++;
			clusters[count].pop_front();
		}
		cout << endl;
	}

	return 0;
}


void similarity(list<int> clusters []) {
	double distance [k][DATA_SIZE];

	//calculate distance
	for(int count = 0; count < DATA_SIZE; count++){
		for(int count2 = 0; count2 < k; count2++){

			distance[count2][count] = abs(data[count].xCoord - centers[count2].xCoord) +
					abs(data[count].yCoord - centers[count2].yCoord);
		}
	}

	//clear all clusters
	for(int clusternumber = 0; clusternumber < k; clusternumber++){
		clusters[clusternumber].erase(clusters[clusternumber].begin(), clusters[clusternumber].end());
	}

	//recreate clusters
	for(int datanumber = 0; datanumber < DATA_SIZE; datanumber++){
		int smallest = 0;
		for(int clusternumber = 0; clusternumber < k; clusternumber++){
			if(distance[clusternumber][datanumber] < distance[smallest][datanumber]) {
				smallest = clusternumber;
			}
		}

		clusters[smallest].emplace_back(datanumber);
	}

}
