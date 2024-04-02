// NOTE: to run this code, you need to type in cmd the following command:
// mpiexec -n 3 "Array_Sum_Another_Approach.exe"
// This command will run the code with 3 processes.
//
// Explanation:
// This code is another approach to calculate the sum of an array using MPI_Scatter and MPI_Gather functions.
// The array is divided into 3 parts and each part is sent to a different process.
// Each process calculates the sum of the local array and sends it back to the root process.
// The root process calculates the sum of the array and prints the result.

#include <iostream>
#include <mpi.h>

using namespace std;

int main()
{

    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // declare array of 15 elements
    int arr[15], local_sums[3];

    if (rank == 0)
    {

        // Initialize the array
        for (int i = 0; i < 15; i++)
        {
            arr[i] = (i + 1);
        }
    }

    // declare array of 5 elements
    int local_arr[5];

    // Scatter the array
    MPI_Scatter(arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the sum of the local array for each process
    int local_sum = 0;
    for (int i = 0; i < 5; i++)
    {
        local_sum += local_arr[i];
    }

    // Gather the local sums
    MPI_Gather(&local_sum, 1, MPI_INT, local_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        // Calculate the sum of the array
        int output_result = 0;
        for (int i = 0; i < 3; i++)
        {
            output_result += local_sums[i];
        }

        cout << "The sum of the array is: " << output_result << "\n";
    }

    MPI_Finalize();

    return 0;
}