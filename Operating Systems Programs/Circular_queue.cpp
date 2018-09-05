/*
 * Circular_queue.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: sam
 */
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

const int BUFFER_SIZE = 10;


class Circular_queue {
	int array[BUFFER_SIZE];
	int startPosition;
	int endPosition;
	bool isFull;
	bool isEmpty;

public:

	Circular_queue() {
		startPosition = 0;
		endPosition = 0;
		isFull = false;
		isEmpty = true;
	}

	void insert_item(int a){

		if(!isFull) {
			array[startPosition] = a;
			startPosition = ++startPosition % BUFFER_SIZE;
			isEmpty = false;
			if(startPosition == endPosition){
				isFull = true;
			}
		}
		else {
			cout << "INSERTING INTO A FULL BUFFER. INSERT FAILED." << endl;
		}

	}

	int remove_item(){

		int result;

		if (!isEmpty) {
			result = array[endPosition];
			endPosition = ++endPosition % BUFFER_SIZE;
			isFull = false;
			if(startPosition == endPosition){
				isEmpty = true;
			}
		}
		else {
			cout << "REMOVING FROM AN EMPTY BUFFER. REMOVE FAILED." << endl;
			result = -1;
		}
		return result;
	}

};
