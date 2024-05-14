// Question 7: -Implement program using mpi to count the even numbers in array with 100 elements 
// and print the count of even numbers in each process.
// you should can run the program with any number of processes.
// the result should be equal to 50.

#include <iostream>
#include <mpi.h>

using namespace std;

const int SIZE = 100;

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
	
	// array of 100 elements
	int arr[SIZE];

	// fill the array with random numbers
	for (int i = 0; i < SIZE; i++)
	{
		arr[i] = (i + 1);
	}

	// Scatter the array to all processes
	int chunk_size = SIZE / processes;
	int* local_arr = new int[chunk_size];
	MPI_Scatter(&arr, chunk_size, MPI_INT, local_arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// count the even numbers in the local array
	int cnt = 0;
	for (int i = 0; i < chunk_size; i++)
	{
		if (local_arr[i] % 2 == 0)
		{
			cnt++;
		}
	}
	
	// reduce the count of even numbers in all processes
	int total_cnt;
	MPI_Reduce(&cnt, &total_cnt, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		
		// handle the remaining elements
		int remaining = SIZE - (SIZE % processes);
		for (int i = remaining; i < SIZE; i++)
		{
			if (arr[i] % 2 == 0)
			{
				total_cnt++;
			}
		}

		cout << "Total count of even numbers = " << total_cnt << endl;
	}

	// finalize MPI
	MPI_Finalize();
}