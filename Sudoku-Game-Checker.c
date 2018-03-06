/*
 * Sudoku-Game-Checker.c
 *
 *  Created on: Feb 25, 2018
 *      Author: hieptran1208
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int flag = 0;			// flag = 0: no duplicates. 	To test and break the program when there's a duplicate

void test(int array[]);


void* check_row (void* arg)				// Check all the rows
{
	int *temp_ptr = (int*)arg;
	int temp = *temp_ptr;
	int testArray[9];					// A temporary array to hold data in that row

	// Initialize the test array
	testArray[0] = temp;
	for(int k = 1; k< 9; k++)
	{
		temp = *(++temp_ptr);			// Assign data to the temp array
		testArray[k] = temp;
	}

	test(testArray);					// check on the temp array

	pthread_exit(0);
}

void* check_col (void* arg)				// check for the columns
{
	int *temp_ptr = (int*)arg;
	int temp = *temp_ptr;
	int testArray[9];					// temp array

	// Initialize the test array
	testArray[0] = temp;
	for(int k = 1; k< 9; k++)
	{
		temp = *((9*k)+temp_ptr);		// Assign data in column to temp array
		testArray[k] = temp;
	}

	test(testArray);					// check on temp array

	pthread_exit(0);
}


void* check_square (void* arg)			// check all the subgrids... similar to check_col
{
	int *temp_ptr = (int*)arg;
	int temp = *temp_ptr;
	int testArray[9];

	// Initialize the test array
	for(int j = 0; j < 3; j++)
	{
		for(int k = 0; k < 3; k++)
		{
			temp = *(temp_ptr++);
			testArray[k + 3*j] = temp;
		}
		for(int i =0;i<6;i++)
			temp = *(++temp_ptr);
	}

	test(testArray);

	pthread_exit(0);
}

void test(int array[])								// Compare elements in the array
{
// test the array for duplicates
	for(int i = 0; i < 9 && !flag; i++)
	{
		for(int j = i+ 1; j< 9 && !flag; j++)
		{
			if(array[i] == array[j])
			{
				flag = 1;
				break;
			}
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


	printf("CS149 Sudoku from Hiep Tran\n");
	printf("Here is the soduku matrix: \n");
	for(int i =0; i<9; i++)
	{
		for(int j=0; j<9; j++)
			printf("%d ", grid[i][j]);
		printf("\n");
	}
	printf("\n");

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


	pthread_t tids_square[9];

	int k = 0;				// temp index to keep track of the number of threads
	for(int i = 0; i< 3; i++)
	{
		for(int j=0; j<3; j++)
		{
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			// check for column: grid[0] + i with i:0..9
			pthread_create(&tids_square[k], &attr, check_square, grid[3*i]+3*j);
			k++;					// increment the number of threads
		}
	}



	// Wait for the thread to be done
	for(int i = 0; i< 9; i++)
	{
		pthread_join(tids_row[i], NULL);
	}

	// check duplicates on all 9 columns
	for(int i = 0; i< 9; i++)
	{
		pthread_join(tids_col[i], NULL);
	}

	for(int i = 0; i< 9; i++)
	{
		pthread_join(tids_square[i], NULL);
	}

	if(flag)
		printf("The sudoku is not valid\n");
	else
		printf("The sudoku is valid\n");
}
