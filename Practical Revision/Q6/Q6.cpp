// Question 6: Write a program that finds the maximum number in an array of 200 elements using MPI.
//  The array should be scattered among the processes and the maximum number should be gathered to the root process.
//  The root process should then find the maximum number among the gathered numbers and print it.
//  The array should be filled with numbers from 1 to 200.
// The program should be able to run with any number of processes.

#include <iostream>
#include <mpi.h>

using namespace std;

const int SIZE = 200;

int main()
{
	// initialize MPI
	MPI_Init(NULL, NULL);

	// number of available processes
	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	// rank of process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// array to be scattered
	int arr[SIZE];

	// fill the array with numbers
	if (rank == 0) {
		for (int i = 0; i < SIZE; i++) {
			arr[i] = (i + 1);
		}
	}

	// scatter the array
	int chunk_size = SIZE / processes;
	int* local_arr = new int[chunk_size];
	MPI_Scatter(arr, chunk_size, MPI_INT, local_arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// find the max number in the local array
	int max_number = INT_MIN;
	for (int i = 0; i < chunk_size; i++) {
		if (local_arr[i] > max_number) {
			max_number = local_arr[i];
		}
	}

	// gather the max numbers from all processes
	int* results = new int[processes];
	MPI_Gather(&max_number, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// find the final max number
	if (rank == 0) {
		
		int final_max = INT_MIN;

		// check the remaining numbers
		int rem = SIZE - (SIZE % processes);
		for (int i = rem; i < SIZE; i++) {
			if (arr[i] > final_max) {
				final_max = arr[i];
			}
		}

		// check the gathered numbers
		for (int i = 0; i < processes; i++) {
			if (results[i] > final_max) {
				final_max = results[i];
			}
		}

		cout << "Max number is " << final_max << "\n";

	}

	// finalize MPI
	MPI_Finalize();
}