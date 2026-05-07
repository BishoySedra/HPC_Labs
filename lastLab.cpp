#include <iostream>
#include <mpi.h>
using namespace std;

void sendValueTo(int *value, int count, int rank) {
   MPI_Send(value, count, MPI_INT, rank, 0, MPI_COMM_WORLD);
}

void recvValueFrom(int *value, int count, int rank) {
   MPI_Recv(value, count, MPI_INT, rank, 0, MPI_COMM_WORLD,
            MPI_STATUSES_IGNORE);
}

int main() {
   MPI_Init(NULL, NULL);
   int rank, num_procs, *arr, *partial_arr, total_res = 0;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

   const int size = 15;
   arr = new int[size];
   int counts[] = {3, 5, 7};
   int disp[] = {12, 0, 5};

   if (rank == 0) {
      for (int i = 0; i < size; i++)
         arr[i] = i + 1;
   }

   int partial_size = counts[rank];
   partial_arr = new int[partial_size];
   MPI_Scatterv(arr, counts, disp, MPI_INT, partial_arr, partial_size, MPI_INT,
                0, MPI_COMM_WORLD);

   int partial_res = 0;
   for (int i = 0; i < partial_size; i++)
      partial_res += partial_arr[i];

   // int *gatherArr = new int[num_procs];
   // MPI_Allgather(&partial_res, 1, MPI_INT, gatherArr, 1, MPI_INT,
   //               MPI_COMM_WORLD);
   //
   // for (int i = 0; i < num_procs; i++)
   //    total_res += gatherArr[i];

   MPI_Allreduce(&partial_res, &total_res, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

   cout << "Rank " << rank << ": " << total_res << endl;

   if (rank == 0) {
      delete[] arr;
      delete[] partial_arr;
      // delete[] gatherArr;
   }
   MPI_Finalize();
}
