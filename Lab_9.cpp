// HANDS-ON: Calculating the summation of the following formula using MPI Broadcast and Reduce functions -> (5 - 3i)^2
// The formula is calculated for the range of 1 to N, where N is the input from the user.
// The formula is divided into equal chunks and each process calculates the formula for its chunk. The final result is calculated by the root process by adding all the partial results from each process. The remainder is calculated by the root process if the number of elements is not divisible by the number of processes.
// you can execute the program by running the following command:
// mpiexec -n <number_of_processes>

#include <iostream>
#include <mpi.h>

using namespace std;

int main()
{
	// initialize MPI
	MPI_Init(NULL, NULL);

	// get number of processes and rank
	int processes, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N = 0;

	if (rank == 0)
	{
		cout << "Enter value of N = ";
		cin >> N;
	}

	// send value of N to all processes
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int chunk_size = N / processes;
	int start = (rank * chunk_size) + 1;
	int end = (rank + 1) * chunk_size;

	int partial_answer = 0;
	for (int i = start; i <= end; i++)
	{
		int term = 5 - (3 * i);
		partial_answer += (term * term);
	}

	int final_result;
	MPI_Reduce(&partial_answer, &final_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{

		// handling remainder
		int remainder = N - (N % processes);

		for (int i = remainder + 1; i <= N; i++)
		{
			int term = 5 - (3 * i);
			final_result += (term * term);
		}

		cout << "Result = " << final_result << "\n";
	}

	// Finalize MPI
	MPI_Finalize();

	return 0;
}