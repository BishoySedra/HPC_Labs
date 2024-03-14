// Purpose: MPI program to demonstrate ring communication

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
        // send message to process 1
        int number = 100;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // receive message from process 3
        MPI_Recv(&number, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process 0 received number " << number << " from process 3" << endl;
    }

    if (rank == 1)
    {
        // receive message from process 0
        int number;
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process 1 received number " << number << " from process 0" << endl;

        // send message to process 2
        MPI_Send(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }

    if (rank == 2)
    {
        // receive message from process 1
        int number;
        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process 2 received number " << number << " from process 1" << endl;

        // send message to process 3
        MPI_Send(&number, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
    }

    if (rank == 3)
    {
        // receive message from process 2
        int number;
        MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process 3 received number " << number << " from process 2" << endl;

        // send message to process 0
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // finalize MPI
    MPI_Finalize();
    return 0;
}