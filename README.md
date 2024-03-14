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
