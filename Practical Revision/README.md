# Q1: Adding specific value (b) to the matrix (A) values

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
    // initialize MPI
    MPI_Init(NULL, NULL);

    // number of available processes
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    // rank of process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // cout << processes << " processes are available." << endl;

    //Define an array and integer variable
    int b, N;

    if (rank == 0)
    {

        cout << "Enter the size of the array: ";
        cin >> N;

        cout << "Enter the value of b: ";
        cin >> b;
    }

    //Scatter the array to all processes
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

    //Calculate the size of the array for each process
    int chunk_size = N / processes;

    //Define the local array for each process
    int *local_A = new int[chunk_size];

    //Scatter the array to all processes
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

        // handling the remainder
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
    // initialize MPI
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

    //Scatter the array to all processes
    int chunk_size = Array_size / processes;
    int *local_a = new int[chunk_size];

    MPI_Scatter(&a, chunk_size, MPI_INT, local_a, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    //Count the numbers divisible by 7
    int count = 0;
    for (int i = 0; i < chunk_size; i++)
    {
        if (local_a[i] % 7 == 0)
        {
            count++;
        }
    }

    //Reduce the count to the root process
    int total_count;
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        //Handle the remaining elements
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
	// initialize MPI
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

		//Get the target number from the user
		cout << "Enter the target number: ";
		cin >> target;
	}

	//Broadcast target number to all processes
	MPI_Bcast(&target, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter array to all processes
	int chunk_size = Array_size / processes;
	int* local_array = new int[chunk_size];

	MPI_Scatter(&array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// Search for the target number in a local array
	int found = -1;
	for (int i = 0; i < chunk_size; i++)
	{
		if (local_array[i] == target)
		{
			found = rank;
			break;
		}
	}

	// return found variable to the root process
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

# Q4: Modifying array elements then summation of all elements in parallel

![Q4](https://raw.githubusercontent.com/BishoySedra/HPC_Labs/main/Practical%20Revision/Q4/Q4.jpeg)

## Solution

```cpp
// Question 4: Write a parallel program using MPI to perform the following operation on an array of size 1000.
//  If the number is even, increase it by 2, and if it is odd, decrease it by 1.
//  Calculate the sum of the array elements after the operation.
//  The result should be displayed by the master process.
//  The result summation should be equal to 501000

#include <iostream>
#include <mpi.h>

using namespace std;

const int Array_size = 1000;
const int value_to_increase = 2;
const int value_to_decrease = 1;

int main()
{
	// initialize MPI
	MPI_Init(NULL, NULL);

	// number of available processes
	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	// rank of process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Declare the array of size 1000
	int arr[Array_size];

	// initialize the array via master rank
	if (rank == 0)
	{
		for (int i = 0; i < Array_size; i++)
		{
			arr[i] = i + 1;
		}
	}

	// scatter the original array called "arr" to all processes
	int chunk_size = Array_size / processes;
	int *local_arr = new int[chunk_size];
	MPI_Scatter(&arr, chunk_size, MPI_INT, local_arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	int *results = new int[processes];

	// Doing the operation for each process
	int sum = 0;
	for (int i = 0; i < chunk_size; i++)
	{
		//Check if the number is even then increase by 2 and if odd decrease by 1
		if ((local_arr[i] % 2) == 0)
		{
			local_arr[i] += value_to_increase;
		}
		else
		{
			local_arr[i] -= value_to_decrease;
		}

		sum += local_arr[i];
	}

	// gather the results
	MPI_Gather(&sum, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{

		int final_sum = 0;
		for (int i = 0; i < processes; i++)
		{
			final_sum += results[i];
		}

		// handling reminder of the array
		int rem = Array_size - (Array_size % processes);
		for (int i = rem; i < Array_size; i++)
		{
			//Check if the number is even then increase by 2 and if odd decrease by 1
			if ((arr[i] % 2) == 0)
			{
				arr[i] += value_to_increase;
			}
			else
			{
				arr[i] -= value_to_decrease;
			}

			final_sum += arr[i];
		}

		cout << "Result = " << final_sum << "\n";
	}

	// finalize MPI
	MPI_Finalize();
}
```

# Q5: Solving Σ (x^2 * y) for x = 1 to 50 and y = 1 to 50

![Q5](https://raw.githubusercontent.com/BishoySedra/HPC_Labs/main/Practical%20Revision/Q5/Q5.jpeg)

## Solution

```cpp
// Question 5: Write a parallel program using MPI to calculate the following expression:
// Σ (x^2 * y) for x = 1 to 50 and y = 1 to 50
// Each process should calculate a part of the sum and then the master process should collect all the partial sums and calculate the final sum.
// The program should be able to run with any number of processes.
// but required to be run with 5 processes.
//The result should be equal to 1625625

#include <iostream>
#include <mpi.h>

using namespace std;

const int Array_size = 50;

int main()
{

	// initialize MPI
	MPI_Init(NULL, NULL);

	// number of available processes
	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	// rank of process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Create two arrays of size 50
	int arr1[Array_size], arr2[Array_size];

	// initialize two arrays via master rank
	if (rank == 0) {
		for (int i = 0; i < Array_size; i++) {
			arr1[i] = i + 1;
			arr2[i] = i + 1;
		}
	}

	// scatter the original arrays to all processes
	int chunk_size = Array_size / processes;
	int* local_arr1 = new int[chunk_size];
	int* local_arr2 = new int[chunk_size];

	MPI_Scatter(&arr1, chunk_size, MPI_INT, local_arr1, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&arr2, chunk_size, MPI_INT, local_arr2, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	// doing the same operation for each process
	int partial_sum = 0;
	for (int i = 0; i < chunk_size; i++) {
		int term1 = (local_arr1[i] * local_arr1[i]);
		int term2 = local_arr2[i];
		partial_sum += (term1 * term2);
	}

	// reduce all these partial_sums to one final sum
	int final_sum;
	MPI_Reduce(&partial_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		// handling the remainder of the original arrays
		int rem = Array_size - (Array_size % processes);

		for (int i = rem; i < Array_size; i++) {
			int term1 = (arr1[i] * arr1[i]);
			int term2 = arr2[i];
			final_sum += (term1 * term2);
		}

		cout << "Result = " << final_sum << "\n";
	}

	// finalize MPI
	MPI_Finalize();
}
```
