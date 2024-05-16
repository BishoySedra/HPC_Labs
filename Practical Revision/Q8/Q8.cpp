// Question 8: -Implement program using mpi to calculate the product of array elements of size 10.
// then the calculate the result up to power 1 / 10.
// The formula is: result = (Π(arr[i]))^(1/10) where i = 1 to 10.
// the output should be equal to 4.52873

#include <iostream>
#include <cmath>
#include <mpi.h>

using namespace std;

const int SIZE = 10;

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

	// scatter array to all processes
	int chunk_size = SIZE / processes;
	int* local_arr = new int[chunk_size];
	MPI_Scatter(&arr, chunk_size, MPI_INT, local_arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// calculate product of local array
	int product = 1;
	for (int i = 0; i < chunk_size; i++)
	{
		product *= local_arr[i];
	}

	// calculate global product
	int global_product;

	MPI_Reduce(&product, &global_product, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
	 
	if (rank == 0)
	{
		// handing the remaining elements
		int rem = SIZE - (SIZE % processes);
		for (int i = rem; i < SIZE; i++)
		{
			global_product *= arr[i];
		}

		double result = pow(global_product, 1.0 / SIZE);
		cout << "Result: " << result << "\n";
	}


	// finalize MPI
	MPI_Finalize();

	return 0;
}