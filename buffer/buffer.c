/**
 * Chris Delaney 
 * Project 5
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];

/*define semaphores and mutex*/
sem_t empty;
sem_t occupied;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
        /* Acquire Empty Semaphore */

	sem_wait(&empty);
	
	/* Acquire mutex lock to protect buffer */

	pthread_mutex_lock(&m);

	buffer[insertPointer] = item;
	if (++insertPointer >= BUFFER_SIZE) {
		insertPointer = 0;
	}

	/* Release mutex lock and full semaphore */
	pthread_mutex_unlock(&m);
	sem_post(&occupied);

	return 0;
}

int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */
	sem_wait(&occupied);

	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&m);

	*item = buffer[removePointer];
	if (++removePointer >= BUFFER_SIZE) {
		removePointer = 0;
	}

	/* Release mutex lock and empty semaphore */
	pthread_mutex_unlock(&m);
	sem_post(&empty);
	 

	return 0;
}


int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	/*call atoi to get arguments */
	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

 	sem_init(&empty, 0, BUFFER_SIZE);
 	sem_init(&occupied, 0, 0);


	/* Create the producer and consumer threads */
	pthread_t consumers[consumerThreads];
	pthread_t producers[producerThreads];
 
	for(i = 0; i< producerThreads; i++) {
		pthread_create(&producers[i], 0, producer, buffer);
	}
	for(j = 0; j < consumerThreads; j++)
	{
 		pthread_create(&consumers[j], 0, consumer, buffer);
	}

	/* Sleep for user specified time */
	sleep(sleepTime);

	return 0;
}

void *producer(void *param)
{
	buffer_item item;
	while(1)
	{
		sleep(rand() % 3);
		item = rand() % 100;
		if (insert_item(item)) {
			printf("report error condition");
		}
		else {
			printf("producer produced %d\n", item);
		}

	}

}

void *consumer(void *param)
{
	buffer_item item;
	while(1)
	{
 		sleep(rand() % 3);
		if(remove_item(&item)) {
			printf("report error condition");
		}
		else {
			printf("consumer consumed %d\n", item);
		}
	}
}