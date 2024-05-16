// Question 9:
// Write a program using MPI to count number of prime numbers in an array of size 100.
// the output should be equal to 25.

#include <iostream>
#include <cmath>
#include <mpi.h>

using namespace std;

const int SIZE = 100;

bool is_prime(int n)
{
	if (n <= 1)
	{
		return false;
	}

	for (int i = 2; i * i <= n; i++)
	{
		if (n % i == 0)
		{
			return false;
		}
	}

	return true;
}

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

	int arr[SIZE];

	// initialize array
	if (rank == 0)
	{
		for (int i = 0; i < SIZE; i++)
		{
			arr[i] = i + 1;
		}
	}

	// scatter array
	int chunk_size = SIZE / processes;
	int* local_arr = new int[chunk_size];
	MPI_Scatter(arr, chunk_size, MPI_INT, local_arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// count prime numbers
	int count = 0;
	for (int i = 0; i < chunk_size; i++)
	{
		if (is_prime(local_arr[i]))
		{
			count++;
		}
	}

	// reduce count
	int total_count;
	MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		// handline the remaining elements
		int rem = SIZE - (SIZE % processes);
		for (int i = rem; i < SIZE; i++)
		{
			if (is_prime(arr[i]))
			{
				total_count++;
			}
		}

		cout << "Total prime numbers: " << total_count << "\n";
	}



	// finalize MPI
	MPI_Finalize();

	return 0;
}