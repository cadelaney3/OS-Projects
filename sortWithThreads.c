/**
 * Chris Delaney
 * Project 4
 * using threads to sort
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	/* establish the first sorting thread */
	//1. call malloc to allocate a “parameters”
	//2. use “parameters” to specify the first half of the array
    //3. create the first thread

	parameters* firstHalf = malloc(sizeof(parameters));
	firstHalf->from_index = 0;
	firstHalf->to_index = (SIZE/2) - 1;
	pthread_create(&workers[0], 0, sorter, firstHalf); 
	i++;
	

	/* establish the second sorting thread */
	//1. call malloc to allocate a “parameters”
	//2. use “parameters” to specify the first half of the array
      //3. create the second thread

	parameters* secondHalf = malloc(sizeof(parameters));
	secondHalf->from_index = SIZE/2;
	secondHalf->to_index = SIZE - 1;
	pthread_create(&workers[1], 0, sorter, secondHalf);
	i++;
  
	
	/* now wait for the 2 sorting threads to finish */
	// use ptheread_join; wait for 2 sorting threads to finish 

	pthread_join(workers[0], 0);
	pthread_join(workers[1], 0);
	

	/* establish the merge thread */
	//reuse “parameters” to hold the beginning index in each half
	//create the third thread: merge 

	parameters* merging = malloc(sizeof(parameters));
	merging->from_index = firstHalf->from_index;
	merging->to_index = secondHalf->from_index;
	pthread_create(&workers[2], 0, merger, merging);
	
	/* wait for the merge thread to finish */
	 
	pthread_join(workers[2], 0);

	/* output the sorted array */

	for(i=0; i < SIZE; i++) {
		printf("%d\n", result[i]);
	}	

    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
	parameters* p = (parameters *)params;
	int temp;
	
	// Sorting algorithm here. It can be any sorting algorithm.

	for(int i = p->from_index; i <= p->to_index; i++) {
		for(int j = p->from_index; j <= p->to_index; j++) {
			if(list[i] < list[j]) {
				temp = list[i];
				list[i] = list[j];
				list[j] = temp;
			}
		}
	}

	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
	parameters* p = (parameters *)params;
	
	//reuse “parameters” to hold the first index in each half
	//merge two sorted sublist to the arry result

	int i = 0;
	int j = p->from_index;
	int k = p->to_index;


	while(j < (SIZE/2) && k < SIZE) {
		if(list[j] <= list[k]) {
			result[i] = list[j];
			j++;
		}
		else {
			result[i] = list[k];
			k++;
		}
		i++;
	}	
	if (k < SIZE) {
		while (k < SIZE) {
			result[i] = list[k];
			k++;
			i++;
		}
	}
	if (j < (SIZE/2)) {
		while (j < (SIZE/2)) {
			result[i] = list[j];
			j++;
			i++;
		}
	}


	pthread_exit(0);
}