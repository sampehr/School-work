/*
 * Sleeping_TA.cpp
 *
 *  Created on: Apr 25, 2017
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

using namespace std;

const int WORK_TIME = 1000;
const int HELP_TIME = 200;

void *TA_work(void*);
void *student_work(void*);

struct Sema {
	sem_t chairs;
	sem_t awake;
	sem_t TAhelp;
	sem_t chairsmutex;
	sem_t awakemutex;

	int* studentChairs; //keeps track of how many students are waiting in chairs.
							//mirrors value of sem_getvalue(&(sema->chairs), &studentsWaiting)
								//updates when chairs updates
};

struct Data{
	Sema* sema;
	int id;
};

int main(int argc, char *argv[]) {
	if (argc != 3){
		cout<<"incorrect number of arguments. there should be two arguments."<<endl;
		return 0;
	}

	const int sleeptime = atoi(argv[1]);
	const int studentNum = atoi(argv[2]);



	pthread_t* studentThreads;
	studentThreads = new pthread_t[studentNum];


	pthread_t TA;

	pthread_attr_t attr;
	pthread_attr_init(&attr);


	sem_t chairs;
	sem_t awake;
	sem_t TAhelp;
	sem_t chairsmutex;
	sem_t awakemutex;

	if (sem_init(&chairs,0,0) == -1)
	              printf("%s\n",strerror(errno));

	if (sem_init(&awake,0,0) == -1)
		              printf("%s\n",strerror(errno));

	if (sem_init(&TAhelp,0,0) == -1)
				              printf("%s\n",strerror(errno));

	if (sem_init(&chairsmutex,0,1) == -1)
		              printf("%s\n",strerror(errno));

	if (sem_init(&awakemutex,0,1) == -1)
			              printf("%s\n",strerror(errno));



	Sema *sema = (struct Sema *) malloc (sizeof (struct Sema));
	sema->chairs = chairs;
	sema->awake = awake;
	sema->chairsmutex = chairsmutex;
	sema->awakemutex = awakemutex;
	sema->TAhelp = TAhelp;

	sema->studentChairs = (int*) malloc(sizeof(int));
	*(sema->studentChairs) = 0;
	pthread_create(&TA,&attr,TA_work,(void *) sema);

	for(int x = 0; x < studentNum; x++){
		Data *data = (struct Data *) malloc (sizeof (struct Data));
		data->sema = sema;
		data->id = x;
		pthread_create(&studentThreads[x],&attr,student_work,(void *) data);
	}

	sleep(sleeptime);

	return 0;
}

void *TA_work(void* input){
	//printf("TA_work begins\n");
	Sema *sema = (Sema*) input;
	int studentsWaiting;

	while(true){
		printf("TA goes to sleep \n");
		if (sem_wait(&(sema->awake)) != 0) //waits until student needs help
			printf("%s\n",strerror(errno));

		if (sem_wait(&(sema->chairsmutex)) != 0) //ensures that there is no data race on studentChairs
			printf("%s\n",strerror(errno));
		/*CRITICAL SECTION*/

		if (sem_post(&(sema->chairs)) != 0) //takes waking-student off of chair-list (frees slot 0)
			printf("%s\n",strerror(errno));

		*(sema->studentChairs) = *(sema->studentChairs) + 1; //frees slot 0

		/*END CRITICAL SECTION*/
		if (sem_post(&(sema->chairsmutex)) != 0)
			printf("%s\n",strerror(errno));

		if (sem_wait(&(sema->TAhelp)) != 0) //waits until student is done being helped
			printf("%s\n",strerror(errno));

		if (sem_wait(&(sema->chairsmutex)) != 0) //ensures that there is no data race on studentChairs
					printf("%s\n",strerror(errno));
		/*CRITICAL SECTION*/

		studentsWaiting = *(sema->studentChairs);

		if(studentsWaiting < 0) { //if studentsWaiting == 0, no one is waiting
			while(studentsWaiting < 0) {
				if (sem_post(&(sema->chairs)) != 0) //calls next student inside
					printf("%s\n",strerror(errno));

				*(sema->studentChairs) = *(sema->studentChairs) + 1; //a student has been pulled off the chairs

		/*END CRITICAL SECTION*/
				if (sem_post(&(sema->chairsmutex)) != 0)
								printf("%s\n",strerror(errno));

				if (sem_wait(&(sema->TAhelp)) != 0) //waits until student is done being helped
					printf("%s\n",strerror(errno));

				if (sem_wait(&(sema->chairsmutex)) != 0) //ensures that there is no data race on studentChairs
								printf("%s\n",strerror(errno));

		/*CRITICAL SECTION*/
				studentsWaiting = *(sema->studentChairs);
			}
		}

		/*ENDS chairsmutex CRITICAL SECTION TOO*/
			if (sem_post(&(sema->chairsmutex)) != 0)
				printf("%s\n",strerror(errno));

	}
}

void *student_work(void* input){
	Data *data = (Data*) input;
	int chairNum = 0;
	int awake;
	bool sitting = false;


	printf("Student %d begins work \n", data->id);

	while(true){

		usleep(WORK_TIME); //programming away

		if (sem_wait(&((data->sema)->chairsmutex)) != 0) //ensures that there is no data race on studentChairs
						printf("%s\n",strerror(errno));
		/*CRITICAL SECTION*/

		if(*((data->sema)->studentChairs) > -1) { //if studentchairs == 0, there are no students currently being helped

			*((data->sema)->studentChairs) = *((data->sema)->studentChairs) -1;

			sem_getvalue(&((data->sema)->awake), &awake);
			if (awake == 0) {
				if (sem_post(&((data->sema)->awake)) != 0)
					printf("%s\n",strerror(errno));
				printf("TA has been woken up =_= by Student %d, who sits down in a chair while TA is still groggy \n", data->id);
			}
			else {
				printf("Student %d catches TA just before nap! \n", data->id);
			}
			sitting = true;

		}
		else if(*((data->sema)->studentChairs) > -4) { //if studentChairs == -4, 3 students are waiting and the chairs are full

			*((data->sema)->studentChairs) = *((data->sema)->studentChairs) -1; //adds this student to waiting students
															//(before this student is suspended by a wait)
			chairNum = *((data->sema)->studentChairs);
			printf("    student %d waits in a chair (%d students in chairs) \n", data->id, (abs(chairNum)-1));
			sitting = true;
		}
		if(sitting){

		/*END CRITICAL SECTION*/
			if (sem_post(&((data->sema)->chairsmutex)) != 0) //releases next student checking for a free chair
				printf("%s\n",strerror(errno));

			if (sem_wait(&((data->sema)->chairs)) != 0) //waits in chair until TA is free, or updates chair if TA was woken up
				printf("%s\n",strerror(errno));
			/*CRITICAL SECTION*/

			printf("The TA is helping student %d \n", data->id);
			usleep(HELP_TIME);
			printf("The TA is done helping student %d \n\n", data->id);
			if (sem_post(&((data->sema)->TAhelp)) != 0) //releases TA to help another student
				printf("%s\n",strerror(errno));

			/*END CRITICAL SECTION*/
		}
		else {

		/*ENDS chairsmutex CRITICAL SECTION TOO*/
			if (sem_post(&((data->sema)->chairsmutex)) != 0) //releases next student checking for a free chair
				printf("%s\n",strerror(errno));

			printf("         student %d is back to programming because the chairs were full\n", data->id);
		}

		sitting = false;
	}
}
