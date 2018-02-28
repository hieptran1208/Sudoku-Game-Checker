/*
 * hw2.c
 *
 *  Created on: Feb 25, 2018
 *      Author: hieptran1208
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int flag = 0;			// flag = 0: no duplicates

void test(int array[]);

void* check_row (void* arg)
{
	int *temp_ptr = (int*)arg;
	int temp = *temp_ptr;
	int testArray[9];

	// Initialize the test array
	testArray[0] = temp;
	for(int k = 1; k< 9; k++)
	{
		temp = *(++temp_ptr);
		testArray[k] = temp;
	}

	test(testArray);

	pthread_exit(0);
}

void* check_col (void* arg)
{
	int *temp_ptr = (int*)arg;
	int temp = *temp_ptr;
	int testArray[9];

	// Initialize the test array
	testArray[0] = temp;
	for(int k = 1; k< 9; k++)
	{
		temp = *((9*k)+temp_ptr);
		testArray[k] = temp;
	}

	test(testArray);

	pthread_exit(0);
}


void* check_square (void* arg)
{
	int *temp_ptr = (int*)arg;
	int temp = *temp_ptr;
	int testArray[9];

	// Initialize the test array
	testArray[0] = temp;
	for(int j = 0; j < 3; j++)
	{
		for(int k = 1; k < 3; k++)
		{
			temp = *(++temp_ptr);
			testArray[k + 3*j] = temp;
		}
		temp = *(6+temp_ptr);
	}

	test(testArray);

	pthread_exit(0);
}

void test(int array[])
{
// test the array for duplicates
	for(int i = 0; i < 9 && !flag; i++)
	{
		for(int j = i+ 1; j< 9 && !flag; j++)
		{
			if(array[i] == array[j])
				flag = 1;
		}
	}
}



int main(int argc, char** argv)
{
	int grid[9][9] ={{6,5,3,1,2,8,7,9,4},
					 {1,7,4,3,5,9,6,8,2},
					 {9,2,8,4,6,7,5,3,1},
					 {2,8,6,5,1,4,3,7,9},
					 {3,9,1,7,8,2,4,5,6},
					 {5,4,7,6,9,3,2,1,8},
					 {8,6,5,2,3,1,9,4,7},
					 {4,1,2,9,7,5,8,6,3},
					 {7,3,9,8,4,6,1,2,5}};


//	pthread_t tid;

	// Thread ID
	pthread_t tids_row[9];
	// Create attributes and launch thread
	for(int i = 0; i< 9; i++)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids_row[i], &attr, check_row, grid[i]);		// check for rows
	}

	pthread_t tids_col[9];
	for(int i = 0; i< 9; i++)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids_col[i], &attr, check_col, grid[0] + i);		// check for column: grid[0] + i with i:0..9
	}


	pthread_t tids_square;
//	for(int i = 0; i< 9; i++)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids_square, &attr, check_square, grid[0]);		// check for column: grid[0] + i with i:0..9
	}



	// Wait for the thread to be done
	for(int i = 0; i< 9; i++)
	{
		pthread_join(tids_row[i], NULL);
		if(flag)
			printf("The row %d has duplicates\n", i+1);
	}

	// check duplicates on all 9 columns
	for(int i = 0; i< 9; i++)
	{
		pthread_join(tids_col[i], NULL);
		if(flag)
			printf("The column %d has duplicates\n", i+1);
	}

//	for(int i = 0; i< 9; i++)
	{
		pthread_join(tids_square, NULL);
		if(flag)
			printf("The square has duplicates\n");
	}

}
