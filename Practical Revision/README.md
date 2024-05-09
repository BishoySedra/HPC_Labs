# Q1: Adding specific value (b) to matrix (A) values

![Q1](https://raw.githubusercontent.com/BishoySedra/HPC_Labs/main/Practical%20Revision/Q1/Q1.jpeg)

## Solution

```cpp
// Question 1: Write a parallel program using MPI to add a constant value b to each element of an array A of size N.
//  The value of b and the size of the array N should be entered by the user.
//  The program should be able to handle any number of processes.
//  The array should be scattered to all processes and the result should be gathered to the root process.
//  The root process should print the result.
//  The program should be able to handle the case when the number of processes is not a multiple of the size of the array.

#include <iostream>
#include <mpi.h>

using namespace std;

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

    // cout << processes << " processes are available." << endl;

    // define array and integer variable
    int b, N;

    if (rank == 0)
    {

        cout << "Enter the size of the array: ";
        cin >> N;

        cout << "Enter the value of b: ";
        cin >> b;
    }

    // scatter the array to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *A = new int[N];

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            A[i] = (i + 1);
        }
    }

    // calculate the size of the array for each process
    int chunk_size = N / processes;

    // define local array for each process
    int *local_A = new int[chunk_size];

    // scatter the array to all processes
    MPI_Scatter(A, chunk_size, MPI_INT, local_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // adding the value of b to the local array of each process
    for (int i = 0; i < chunk_size; i++)
    {
        local_A[i] += b;
    }

    int *result = new int[N];

    // gather the array to the root process
    MPI_Gather(local_A, chunk_size, MPI_INT, result, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // print the result
    if (rank == 0)
    {

        // handling the remainders
        int remainder = N - (N % processes);

        for (int i = remainder; i < N; i++)
        {
            result[i] = A[i] + b;
        }

        cout << "The result is: ";

        for (int i = 0; i < N; i++)
        {
            cout << result[i] << " ";
        }

        cout << endl;
    }

    // finalize MPI
    MPI_Finalize();
}
```

# Q2: Counting numbers divisible by 7 in an array of size 150

![Q2](https://raw.githubusercontent.com/BishoySedra/HPC_Labs/main/Practical%20Revision/Q2/Q2.jpeg)

## Solution

```cpp
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
```

# Q3: Parallel Array Search

![Q3](https://raw.githubusercontent.com/BishoySedra/HPC_Labs/main/Practical%20Revision/Q3/Q3.jpeg)

## Solution

```cpp
// Question 3: Parallel Array Search
// Array of size 150 initialized with numbers from 0 to 149.
//  The target number is entered by the user.
//  The array is divided among the processes using scatter.
//  Each process searches for the target number in its local array.
//  The process that finds the target number sets the found variable to its rank.
//	return found variable to the root process.
//  The root process prints the rank of the process that found the target number.

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

	// array of size 150 and target number
	int array[Array_size];
	int target;

	// root process
	if (rank == 0)
	{
		// initialize array with numbers from 0 to 149
		for (int i = 0; i < Array_size; i++)
		{
			array[i] = i;
		}

		// get target number from user
		cout << "Enter the target number: ";
		cin >> target;
	}

	// broadcast target number to all processes
	MPI_Bcast(&target, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter array to all processes
	int chunk_size = Array_size / processes;
	int* local_array = new int[chunk_size];

	MPI_Scatter(&array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// search for target number in local array
	int found = -1;
	for (int i = 0; i < chunk_size; i++)
	{
		if (local_array[i] == target)
		{
			found = rank;
			break;
		}
	}

	// return found variable to root process
	int final_found;
	MPI_Reduce(&found, &final_found, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	// root process
	if (rank == 0)
	{
		// handling the remainder of the array
		int rem = Array_size - (Array_size % processes);
		for (int i = rem; i < Array_size; i++)
		{
			if (array[i] == target)
			{
				final_found = rank;
				break;
			}
		}

		cout << "The process that found the target number is: " << final_found << "\n";
	}

	// finalize MPI
	MPI_Finalize();
}
```
