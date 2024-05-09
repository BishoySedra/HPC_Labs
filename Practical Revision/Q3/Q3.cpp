// Question 3: Parallel Array Search
// Array of size 150 initialized with numbers from 0 to 149.
//  The target number is entered by the user.
//  The array is divided among the processes using scatter.
//  Each process searches for the target number in its local array.
//  The process that finds the target number sets the found variable to its rank.
//	return found variable to the root process.
//  The root process prints the rank of the process that found the target number.

#include <iostream>
#include <mpi.h>

using namespace std;

const int Array_size = 150;

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

	// array of size 150 and target number
	int array[Array_size];
	int target;

	// root process
	if (rank == 0)
	{
		// initialize array with numbers from 0 to 149
		for (int i = 0; i < Array_size; i++)
		{
			array[i] = i;
		}

		// get target number from user
		cout << "Enter the target number: ";
		cin >> target;
	}

	// broadcast target number to all processes
	MPI_Bcast(&target, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter array to all processes
	int chunk_size = Array_size / processes;
	int* local_array = new int[chunk_size];

	MPI_Scatter(&array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// search for target number in local array
	int found = -1;
	for (int i = 0; i < chunk_size; i++)
	{
		if (local_array[i] == target)
		{
			found = rank;
			break;
		}
	}

	// return found variable to root process
	int final_found;
	MPI_Reduce(&found, &final_found, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	// root process
	if (rank == 0)
	{
		// handling the remainder of the array
		int rem = Array_size - (Array_size % processes);
		for (int i = rem; i < Array_size; i++)
		{
			if (array[i] == target)
			{
				final_found = rank;
				break;
			}
		}

		cout << "The process that found the target number is: " << final_found << "\n";
	}

	// finalize MPI
	MPI_Finalize();
}