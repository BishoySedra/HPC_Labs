// Question 1: Write a parallel program using MPI to add a constant value b to each element of an array A of size N.
//  The value of b and the size of the array N should be entered by the user.
//  The program should be able to handle any number of processes.
//  The array should be scattered to all processes and the result should be gathered to the root process.
//  The root process should print the result.
//  The program should be able to handle the case when the number of processes is not a multiple of the size of the array.

#include <iostream>
#include <mpi.h>

using namespace std;

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

	// cout << processes << " processes are available." << endl;

	// define array and integer variable
	int b, N;

	if (rank == 0)
	{

		cout << "Enter the size of the array: ";
		cin >> N;

		cout << "Enter the value of b: ";
		cin >> b;
	}

	// scatter the array to all processes
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int *A = new int[N];

	if (rank == 0)
	{
		for (int i = 0; i < N; i++)
		{
			A[i] = (i + 1);
		}
	}

	// calculate the size of the array for each process
	int chunk_size = N / processes;

	// define local array for each process
	int *local_A = new int[chunk_size];

	// scatter the array to all processes
	MPI_Scatter(A, chunk_size, MPI_INT, local_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// adding the value of b to the local array of each process
	for (int i = 0; i < chunk_size; i++)
	{
		local_A[i] += b;
	}

	int *result = new int[N];

	// gather the array to the root process
	MPI_Gather(local_A, chunk_size, MPI_INT, result, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// print the result
	if (rank == 0)
	{

		// handling the remainders
		int remainder = N - (N % processes);

		for (int i = remainder; i < N; i++)
		{
			result[i] = A[i] + b;
		}

		cout << "The result is: ";

		for (int i = 0; i < N; i++)
		{
			cout << result[i] << " ";
		}

		cout << endl;
	}

	// finalize MPI
	MPI_Finalize();
}