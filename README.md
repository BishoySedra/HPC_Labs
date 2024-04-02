# HPC Labs - Parallel Programming with MPI

This repository contains materials for the High-Performance Computing (HPC) course, focusing on hands-on labs and talks about Parallel Programming using the MPI (Message Passing Interface) package.

## Setting up the Environment

Follow these steps to set up the environment for working with MPI in your project:

1. **Create a Project**: Create an empty project or console application in your preferred Integrated Development Environment (IDE).

2. **Configure Project Properties**:

   - Navigate to the project properties and access the Configuration Properties for C/C++.
   - Under "Additional Include Directories," add the path: `C:\Program Files (x86)\Microsoft SDKs\MPI\Include`.

3. **Configure Linker Properties**:

   - Still in project properties, go to the Linker section.
   - Under "Additional Library Directories," add the path: `C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64` or `C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86` based on your system architecture.

4. **Configure Linker Input**:

   - Inside the Linker section, navigate to Input.
   - Under "Additional Dependencies," add: `msmpi.lib`.

5. **Apply Changes**:

   - Press Apply and then OK to save the changes.

6. **Write Your Code**:

   - Create or open a CPP file within the source files folder of your project.
   - Write your MPI code in the CPP file.

7. **Build the Solution**:

   - Build the solution from the Build tab in your IDE.

8. **Run the Executable**:
   - Navigate to the executable file path in your project directory.
   - Open Command Prompt (cmd) in that directory.
   - Run the following command to execute the program:
     ```bash
     mpiexec "Project_Name"
     ```
     Replace "Project_Name" with the name of your executable file.

## Most Commonly Used MPI Functions

Here are some of the most commonly used MPI functions along with explanations for their parameters:

1. **MPI_Init**

   - Function: Initializes the MPI execution environment.
   - Parameters:
     - `argc`: Pointer to the number of arguments.
     - `argv`: Pointer to the argument vector.

2. **MPI_Finalize**

   - Function: Terminates MPI execution environment.
   - Parameters: None.

3. **MPI_Comm_size**

   - Function: Determines the size of a communicator.
   - Parameters:
     - `comm`: Communicator (group of processes).
     - `size`: Pointer to an integer where the size of the communicator will be stored.

4. **MPI_Comm_rank**

   - Function: Determines the rank of the calling process in the communicator.
   - Parameters:
     - `comm`: Communicator (group of processes).
     - `rank`: Pointer to an integer where the rank of the calling process will be stored.

5. **MPI_Send**

   - Function: Sends a message from one process to another.
   - Parameters:
     - `buf`: Pointer to the send buffer.
     - `count`: Number of elements in the send buffer.
     - `datatype`: Data type of elements in the send buffer.
     - `dest`: Rank of the destination process.
     - `tag`: Message tag (user-defined).

6. **MPI_Recv**

   - Function: Receives a message from another process.
   - Parameters:
     - `buf`: Pointer to the receive buffer.
     - `count`: Number of elements in the receive buffer.
     - `datatype`: Data type of elements in the receive buffer.
     - `source`: Rank of the source process.
     - `tag`: Message tag (user-defined).
     - `status`: Status object to receive status information.

7. **MPI_Isend**

   - Function: Initiates a non-blocking send operation.
   - Parameters:
     - `buf`: Pointer to the send buffer.
     - `count`: Number of elements in the send buffer.
     - `datatype`: Data type of elements in the send buffer.
     - `dest`: Rank of the destination process.
     - `tag`: Message tag (user-defined).
     - `comm`: Communicator (group of processes).
     - `request`: Pointer to a request object that will be created and used to manage the communication.

8. **MPI_Irecv**

   - Function: Initiates a non-blocking receive operation.
   - Parameters:
     - `buf`: Pointer to the receive buffer.
     - `count`: Number of elements in the receive buffer.
     - `datatype`: Data type of elements in the receive buffer.
     - `source`: Rank of the source process.
     - `tag`: Message tag (user-defined).
     - `comm`: Communicator (group of processes).
     - `request`: Pointer to a request object that will be created and used to manage the communication.

9. **MPI_Scatter**

   - Function: Scatters data from the root process to all processes in a communicator.
   - Parameters:
     - `sendbuf`: Pointer to the send buffer (root process).
     - `sendcount`: Number of elements to send from the send buffer (root process).
     - `sendtype`: Data type of elements in the send buffer (root process).
     - `recvbuf`: Pointer to the receive buffer (all processes).
     - `recvcount`: Number of elements to receive in the receive buffer (all processes).
     - `recvtype`: Data type of elements in the receive buffer (all processes).
     - `root`: Rank of the root process.
     - `comm`: Communicator (group of processes).

10. **MPI_Gather**
    - Function: Gathers data from all processes to the root process in a communicator.
    - Parameters:
      - `sendbuf`: Pointer to the send buffer (all processes).
      - `sendcount`: Number of elements to send from the send buffer (all processes).
      - `sendtype`: Data type of elements in the send buffer (all processes).
      - `recvbuf`: Pointer to the receive buffer (root process).
      - `recvcount`: Number of elements to receive in the receive buffer (root process).
      - `recvtype`: Data type of elements in the receive buffer (root process).
      - `root`: Rank of the root process.
      - `comm`: Communicator (group of processes).
