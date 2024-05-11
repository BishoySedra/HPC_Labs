// Question 4: Write a parallel program using MPI to perform the following operation on an array of size 1000.
//  If the number is even, increase it by 2, and if it is odd, decrease it by 1.
//  Calculate the sum of the array elements after the operation.
//  The result should be displayed by the master process.
//  The result summation should be equal 501000

#include <iostream>
#include <mpi.h>

using namespace std;

const int Array_size = 1000;
const int value_to_increase = 2;
const int value_to_decrease = 1;

int main()
{
	// intialize MPI
	MPI_Init(NULL, NULL);

	// number of available processes
	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	// rank of process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// declare the array of size 1000
	int arr[Array_size];

	// initialize the array via master rank
	if (rank == 0)
	{
		for (int i = 0; i < Array_size; i++)
		{
			arr[i] = i + 1;
		}
	}

	// scatter the original array called "arr" to all processes
	int chunk_size = Array_size / processes;
	int *local_arr = new int[chunk_size];
	MPI_Scatter(&arr, chunk_size, MPI_INT, local_arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	int *results = new int[processes];

	// Doing the operation for each process
	int sum = 0;
	for (int i = 0; i < chunk_size; i++)
	{
		// check if the number is even then increase by 2 and if odd decrease by 1
		if ((local_arr[i] % 2) == 0)
		{
			local_arr[i] += value_to_increase;
		}
		else
		{
			local_arr[i] -= value_to_decrease;
		}

		sum += local_arr[i];
	}

	// gather the results
	MPI_Gather(&sum, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{

		int final_sum = 0;
		for (int i = 0; i < processes; i++)
		{
			final_sum += results[i];
		}

		// handling reminder of the array
		int rem = Array_size - (Array_size % processes);
		for (int i = rem; i < Array_size; i++)
		{
			// check if the number is even then increase by 2 and if odd decrease by 1
			if ((arr[i] % 2) == 0)
			{
				arr[i] += value_to_increase;
			}
			else
			{
				arr[i] -= value_to_decrease;
			}

			final_sum += arr[i];
		}

		cout << "Result = " << final_sum << "\n";
	}

	// finalize MPI
	MPI_Finalize();
}