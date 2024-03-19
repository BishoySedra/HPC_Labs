#include <iostream>
#include <mpi.h>

using namespace std;

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int arr[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int total_sum = 0;

    if (rank == 0)
    {

        // first sum the first 5 elements
        int total_sum = 0;
        for (int i = 0; i < 5; i++)
        {
            total_sum += arr[i];
        }

        // sending each 5 elements to 2 processes
        for (int i = 1; i <= 2; i++)
        {
            MPI_Send(&arr[(i * 5)], 5, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // receiving the sum from each process
        int sum;
        for (int i = 1; i <= 2; i++)
        {
            MPI_Recv(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += sum;
        }

        cout << "Total Sum: " << total_sum << endl;
    }
    else if (rank <= 2)
    {

        // receiving the 5 elements from process 0
        int arr[5];
        MPI_Recv(&arr, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // sum the 5 elements
        int sum = 0;
        for (int i = 0; i < 5; i++)
        {
            sum += arr[i];
        }

        // send the sum to process 0
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}