|**********************************************************************;
* Project           : Project 2
*
* Program name      : Deadloks
*
* Author            : Cielo Jordan
*
* Date created      : 28 April 2016
*
* Purpose           : The program determines if a deadlock has or will most likely occur (unsafe state). Otherwise, outputs the order of processes in a safe state.
|**********************************************************************;
1. Global Variables
	1.A. int P: Number of processes
	1.B. int R: Number of resource types
	1.C. int numSafe: Number of safe sequences for each test case
	1.D. int numGranted: Number of processes with granted request/s
	1.E. int testCaseSolution: indicates which solution was used to solve the deadlock; if more than 0, then recovery has been attempted
	1.F. bool deadlocked: Indicates if deadlock has occured or may occur
	1.G. bool* isGranted: array structure for the processes whose requests have been granted

2. struct Process
	2.A. struct Resource
		2.A.i. int numHeld: number of currently held resources
		2.A.ii. int numNeeded: number of resources needed
	2.B. vector<Resource> resources: resources for each process
	2.C. bool isChecking: determines if process is currently being checked as a possible process in safe sequence

3. void buildProcess: function builds each process
	3.A. Outter for-loop (0 to P): Depending on the indicators, each process (data) is updated
		3.A.i. Indicator 3: Initialize isChecking array to FALSE
		3.A.ii. Inner for-loop (0 to R): Depending on the indicators, each resource (data) is updated
			3.A.ii.a. Indicator 0: Get input for the processes and their corresponding resources currently HELD
			3.A.ii.a. Indicator 2: Get input for the processes and their corresponding resources NEEDED

4. bool requestGranted: checks the safety of possible allocation of resources to current process that has requested for resources
	4.A. For-loop: Check if the process' AVAILABLE resources are greater than the number of resources NEEDED
		4.A.i. If TRUE, return TRUE: Process is avaiable
		4.A.ii. If FALSE, return FALSE: Process is not available

5. void printSafeSequence: Checks if there are processes that may run free of deadlock and prints out the safe sequences if found. Otherwise, a deadlock is declared.
		5.A. Outter For-loop (0 to P): For each process, check its availability and eligibility to be in the safe sequence
			5.A.i. Conditional: If process has not been executed and requested resources may be allocated to it, run the process;
				5.A.i.a. First Inner For-loop (0 to R): Free the resources once processes has finished executing.
				5.A.i.b. Add this process to the safe sequence (at the end of the vector)
				5.A.i.c. Find the safe sequence by taking the current (top) process
				5.A.i.d. Second Inner For-loop (0 to R): Reallocate the resources back to the process
		5.B. Conditional: If the safe sequence is complete, print it out
			5.B.i. Increment numSafe which is the total number of safe sequences
			5.B.ii. Set deadlocked to 0 as there is now at least one safe sequence available.
		5.C. Conditional: If the system is in deadlock, add the granted processes to the isGranted array (see Main Function)
6. int main: Main Function
	5.A. Variables
		5.A.i. int numTestCases: the number of test cases
		5.A.ii. vector <Process> processes (P): vector structure for the processes derived from the Struct Process 
		5.A.iii. int numAvailable[R]: number of available resources with the size of R
		5.A.iv. vector <int> safeSequences: vector structure for the safe sequences containing processes
	5.B. Outter For-Loop: Run until all the test cases are done
		5.B.i. First Inner for-loop: Input the number of each available resources
		5.B.ii. buildProcess( processes.data(), 0 ): Get inputs of processes and corresponding number of resources currently ALLOCATED
		5.B.iii. buildProcess( processes.data(), 1 ): Get inputs processes and corresponding number of resources NEEDED; Deadlock PREVENTION (Hold and Wait)
		5.B.iv. buildProcess( processes.data(), 2 ): Initialize the boolean variable to be used for safe sequence checking
		5.B.v. printSafeSequence( processes, numAvailable, safeSequences: solve for possible safe sequence(s)
		5.B.vi. Conditional: If deadlock has occured do the following,
			5.B.vi.a. Declare state of deadlock.
			5.B.vi.b. Conditional: If a process has been terminated: Indicate which processes were granted their requests for resource allocation.
				5.B.vi.b.1. If one of more of the processes has been granted, output processes. Otherwise, indicate that none has been granted.
			5.B.vi.c. If there was no deadlock, indicate the number of total safe sequences and how the deadlock was solved.
		5.B.vii. Otherwise, declare total number of safe sequences acquired.
		5.B.viii. For the next test case: Revert the number of safe sequences to 0, clear processes and safe sequences vectors, free isGranted array.

