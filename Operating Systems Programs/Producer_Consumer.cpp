/*
 * Producer_Consumer.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: sam
 */

#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#include "Circular_queue.cpp"

using namespace std;

void *produce(void*);
void *consume(void*);

struct Data {
	Circular_queue buffer;
	sem_t empty;
	sem_t full;
	sem_t mutex;
};

int main(int argc, char *argv[]) {

	if (argc != 4){
		cout<<"incorrect number of arguments. there should be three arguments."<<endl;
		return 0;
	}

	const int sleeptime = atoi(argv[1]);
	const int producerNum = atoi(argv[2]);
	const int consumerNum = atoi(argv[3]);

	pthread_t* producerThreads;
	producerThreads = new pthread_t[producerNum];
	pthread_t* consumerThreads;
	consumerThreads = new pthread_t[consumerNum];

	sem_t empty;
	sem_t full;
	sem_t mutex;

	Circular_queue buffer;

	if (sem_init(&empty,0,0) == -1)
	              printf("%s\n",strerror(errno));

	if (sem_init(&full,0,BUFFER_SIZE) == -1)
		              printf("%s\n",strerror(errno));

	if (sem_init(&mutex,0,1) == -1)
		              printf("%s\n",strerror(errno));

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	Data *data = (struct Data *) malloc (sizeof (struct Data));

	data->buffer = buffer;
	data->empty = empty;
	data->full = full;
	data->mutex = mutex;


	int x;

	for(x = 0; x < producerNum && x < consumerNum; x++){
		cout << x << endl;
		pthread_create(&producerThreads[x],&attr,produce,(void *) data);
		pthread_create(&consumerThreads[x],&attr,consume,(void *) data);
	}

	if(consumerNum > producerNum) {
		for(x = 0; x < consumerNum; x++){
			pthread_create(&consumerThreads[x],&attr,consume,(void *) data);
		}
	}
	else {
		for(x = 0; x < producerNum; x++){
			pthread_create(&producerThreads[x],&attr,produce,(void *) data);
		}
	}

	sleep(sleeptime);
	pthread_create(&consumerThreads[0],&attr,consume,(void *) data);
	return 0;
}

void *produce(void* input){
	Data *data = (Data*) input;

	int product = rand() % 1000;

	if (sem_wait(&(data->full)) != 0)
	              printf("%s\n",strerror(errno));


		if (sem_wait(&(data->mutex)) != 0)
		              printf("%s\n",strerror(errno));
			/*CRITICAL SECTION*/
		data->buffer.insert_item(product);
			/*END CRITICAL SECTION*/
		if (sem_post(&(data->mutex)) != 0)
					  printf("%s\n",strerror(errno));

	if (sem_post(&(data->empty)) != 0)
	              printf("%s\n",strerror(errno));

	printf("Producer: %d \n", product);

	pthread_exit(0);
}

void *consume(void* input){
	Data *data = (Data*) input;

	int consumed;

	if (sem_wait(&(data->empty)) != 0)
	              printf("%s\n",strerror(errno));

		if (sem_wait(&(data->mutex)) != 0)
	              	  printf("%s\n",strerror(errno));
			/*CRITICAL SECTION*/
		consumed = data->buffer.remove_item();
			/*END CRITICAL SECTION*/
		if (sem_post(&(data->mutex)) != 0)
				  	  printf("%s\n",strerror(errno));

	if (sem_post(&(data->full)) != 0)
	              printf("%s\n",strerror(errno));

	printf("Consumer: %d \n", consumed);

	pthread_exit(0);
}
