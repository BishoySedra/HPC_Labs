// Purpose: MPI program to demonstrate ping-pong communication

#include <iostream>
#include <mpi.h>

using namespace std;

int main()
{
    // initialize MPI
    MPI_Init(NULL, NULL);

    // get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        int x = 0;
        MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "PONG"
             << "\n";
    }

    if (rank == 1)
    {
        int x;
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "PING"
             << "\n";

        MPI_Send(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // finalize MPI
    MPI_Finalize();
    return 0;
}