// Question 2: Write a program that counts the number of elements in an array that are divisible by 7.
//  The array is divided into chunks and each chunk is distributed to a different process.
//  The root process should handle the remaining elements.

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

    int a[Array_size];

    if (rank == 0)
    {

        for (int i = 0; i < 150; i++)
        {
            a[i] = i + 1;
        }
    }

    // scatter the array to all processes
    int chunk_size = Array_size / processes;
    int *local_a = new int[chunk_size];

    MPI_Scatter(&a, chunk_size, MPI_INT, local_a, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // count the numbers divisible by 7
    int count = 0;
    for (int i = 0; i < chunk_size; i++)
    {
        if (local_a[i] % 7 == 0)
        {
            count++;
        }
    }

    // reduce the count to the root process
    int total_count;
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        // handle the remaining elements
        int remaining = Array_size - (Array_size % processes);
        for (int i = remaining; i < Array_size; i++)
        {
            if (a[i] % 7 == 0)
            {
                total_count++;
            }
        }

        cout << "Total numbers divisible by 7: " << total_count << endl;
    }

    // finalize MPI
    MPI_Finalize();
}