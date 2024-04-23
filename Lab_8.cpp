// HANDS-ON: Dot Product using scatter and reduce
// NOTE: to run this code, you need to type in cmd the following command:
// mpiexec -n 3 "Dot_Product.exe"
// This command will run the code with 3 processes and you can pass any number.

#include <iostream>
#include <mpi.h>

// a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + ... + a[n] * b[n]

using namespace std;

const int array_size = 100;

int main()
{
    // initialize MPI
    MPI_Init(NULL, NULL);

    // get rank, size
    int rank, processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    // create arrays
    int a[array_size], b[array_size];
    // int* results = new int[processes];

    // rank 0 initializes the arrays
    if (rank == 0)
    {
        for (int i = 0; i < array_size; i++)
        {
            a[i] = 2;
            b[i] = 3;
        }
    }

    // scatter the arrays
    int chunk_size = array_size / processes;
    int *a_chunk = new int[chunk_size];
    int *b_chunk = new int[chunk_size];

    MPI_Scatter(&a, chunk_size, MPI_INT, a_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&b, chunk_size, MPI_INT, b_chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // calculate the dot product
    int dot_product = 0;
    for (int i = 0; i < chunk_size; i++)
    {
        dot_product += a_chunk[i] * b_chunk[i];
    }

    // reduce the dot products to the final result
    int final_result = 0;
    MPI_Reduce(&dot_product, &final_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // gather the results
    // MPI_Gather(&dot_product, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // rank 0 calculates the final result
    if (rank == 0)
    {
        /*int final_result = 0;
        for (int i = 0; i < processes; i++)
        {
            final_result += results[i];
        }
        */

        int remaining = array_size - (array_size % processes);
        for (int i = remaining; i < array_size; i++)
        {
            final_result += a[i] * b[i];
        }

        cout << "The dot product is: " << final_result << endl;
    }

    // finalizing MPI
    MPI_Finalize();
}
