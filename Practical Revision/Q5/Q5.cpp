// Question 5: Write a parallel program using MPI to calculate the following expression:
// Σ (x^2 * y) for x = 1 to 50 and y = 1 to 50
// Each process should calculate a part of the sum and then the master process should collect all the partial sums and calculate the final sum.
// The program should be able to run with any number of processes.
// but required to be run with 5 processes.
// the result should be equal to 1625625

#include <iostream>
#include <mpi.h>

using namespace std;

const int Array_size = 50;

int main()
{

	// 1625625
	// intialize MPI
	MPI_Init(NULL, NULL);

	// number of available processes
	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	// rank of process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// create two arrays of size 50
	int arr1[Array_size], arr2[Array_size];

	// initialize two arrays via master rank
	if (rank == 0) {
		for (int i = 0; i < Array_size; i++) {
			arr1[i] = i + 1;
			arr2[i] = i + 1;
		}
	}

	// scatter the original arrays to all processes
	int chunk_size = Array_size / processes;
	int* local_arr1 = new int[chunk_size];
	int* local_arr2 = new int[chunk_size];

	MPI_Scatter(&arr1, chunk_size, MPI_INT, local_arr1, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&arr2, chunk_size, MPI_INT, local_arr2, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// doing the same operation for each process
	int partial_sum = 0;
	for (int i = 0; i < chunk_size; i++) {
		int term1 = (local_arr1[i] * local_arr1[i]);
		int term2 = local_arr2[i];
		partial_sum += (term1 * term2);
	}

	// reduce all these partial_sums to one final sum
	int final_sum;
	MPI_Reduce(&partial_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		// handling the remainders of the original arrays
		int rem = Array_size - (Array_size % processes);

		for (int i = rem; i < Array_size; i++) {
			int term1 = (arr1[i] * arr1[i]);
			int term2 = arr2[i];
			final_sum += (term1 * term2);
		}

		cout << "Result = " << final_sum << "\n";
	}

	// finalize MPI
	MPI_Finalize();
}