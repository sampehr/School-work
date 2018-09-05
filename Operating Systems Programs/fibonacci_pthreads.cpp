/*
 * fibonacci_pthreads.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: sam
 */


#include <pthread.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

int* fibList;
void *fibonacci(void *param);

int main(int argc, char *argv[]) {
	if (argc != 2){
		cout<<"incorrect number of arguments. there should be one argument, n."<<endl;
		return 0;
	}
	else if(0 >= atoi(argv[1]) || atoi(argv[1]) > 45) {
		cout<<"input error. 0<n<=45"<<endl;
		return 0;
	}

	const int N = atoi(argv[1]);

	fibList = new int[N];

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid,&attr,fibonacci,(void *) argv[1]);

	pthread_join(tid,NULL);

	for(int k = 0; k<N; k++){
		cout << fibList[k] << " ";
		if(k%10 == 9){
			cout << endl;
		}
	}

	return 0;
}

/**
 * The thread will begin control in this function
 */
void *fibonacci(void *param)
{
	const int N = atoi((char *)param);

	if(N < 1){
		cout << "something went horribly wrong with your N. it is somehow less than 1, and also got through the argument checking.";
		pthread_exit(0);
	}

	fibList[0] = 0;

	if(N > 1){
		fibList[1] = 1;
	}

	for(int k = 2; k<N; k++){
		fibList[k] = fibList[k-1] + fibList[k-2];
	}

	pthread_exit(0);
}
