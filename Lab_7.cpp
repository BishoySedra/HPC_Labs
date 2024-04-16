// HANDS-ON: Implement the DOT Product of two arrays using MPI
// NOTE: to run this code, you need to type in cmd the following command:
// mpiexec -n 3 "Dot_Product.exe"
// This command will run the code with 3 processes and you can pass any number.

#include <iostream>
#include <mpi.h>

using namespace std;

const int array_size = 100;

int main()
{
	// initialize MPI
	MPI_Init(NULL, NULL);

	// get the rank and size of the world
	int rank, world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// declare two arrays
	int arr1[array_size], arr2[array_size];

	if (rank == 0) {
		
		// initialize two arrays
		for (int i = 0; i < array_size; i++) {
			arr1[i] = 2;
			arr2[i] = 3;
		}

	}
	// declare two arrays to store the chunk of the original arrays
	int chunk_size = array_size / world_size;
	int* arr1_chunk = new int[chunk_size];
	int* arr2_chunk = new int[chunk_size];

	// Scatter the original arrays to the processes
	MPI_Scatter(&arr1, chunk_size, MPI_INT, arr1_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&arr2, chunk_size, MPI_INT, arr2_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// Calculate the partial answer
	int partial_answer = 0;
	for (int i = 0; i < chunk_size; i++) {
		partial_answer += (arr1_chunk[i] * arr2_chunk[i]);
	}

	// Declare an array to store the results from all processes
	int* results = new int[world_size];

	// Gather the partial answers from all processes
	MPI_Gather(&partial_answer, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Calculate the final answer
	if (rank == 0) {
		
		int final_output = 0;
		for (int i = 0; i < world_size; i++) {
			final_output += results[i];
		}

		int r = array_size - (array_size % world_size);
		for (int i = r; i < array_size; i++) {
			final_output += (arr1[i] * arr2[i]);
		}

		cout << "The result of DOT Product = " << final_output << "\n";
	}
	
	// finalizing MPI
	MPI_Finalize();
}
