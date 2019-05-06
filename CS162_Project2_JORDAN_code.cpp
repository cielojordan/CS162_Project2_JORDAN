/**********************************************************************
* Project           : Project 2
*
* Program name      : Deadlocks
*
* Author            : Cielo Jordan
*
* Date created      : 28 April 2016
*
* Purpose           : The program determines if a deadlock has or will most likely occur (unsafe state). Otherwise, outputs the order of processes in a safe state. 
**********************************************************************/
#include <iostream>
#include <vector>

int P, R, numGranted = 0, numSafe = 0, testCaseSolution = 0;
bool deadlocked;
bool* isGranted;

using namespace std;

struct Process {
    	struct Resource {
		int numHeld; // 0
       	int numNeeded; // 1
    	};
   	vector<Resource> resources = vector<Resource> (R) ;
   	bool isChecking; // 2
};

void buildProcess( Process processes[], int indicator ) {
	for( int j=0; j<P; j++ ) {
		if ( indicator == 2 ) {
			processes[j].isChecking = 0;
		} else {
			for( int k=0; k<R; k++ ) {
				if ( indicator ==  0 ){
					cin >> processes[j].resources[k].numHeld;
				} else if ( indicator ==  1 ) {
					cin >> processes[j].resources[k].numNeeded;
				}
			}
		}			
	}
}

// Function to check if the process is available for allocation or not
bool requestGranted(int currIndex, vector<Process> processes, int numAvailable[] ) {
	bool granted = 1; // assume tha the request has beeb granted
	// check if the amount of resources requested is less than or equal to the amount available
	for ( int i=0; i<R; i++ ) { // Deadlock AVOIDANCE
		if ( processes[currIndex].resources[i].numNeeded > numAvailable[i] )  {
			granted = 0; // if not, decline the request
		}
	}
	return granted;
}

void printSafeSequence( vector<Process> processes, int numAvailable[], vector<int> safeSequences ) {
	deadlocked = 1;
	for ( int i=0; i<P; i++ ) { 
		// Check if the process is not currently being executed and can be granted resources requested
		// If not, let the process wait until resources are available
		if( !(processes[i].isChecking) && requestGranted( i, processes, numAvailable ) ) {
			processes[i].isChecking = 1;
			cout << "\nProcess #" << i+1 << " currently executing." << endl;

			cout << "RESOURCE COUNT\n" << "---------------------" << endl;
			for ( int j = 0; j < R; j++ ) {
                numAvailable[j] += processes[i].resources[j].numHeld; // increase the number of available resources by deallocating from current process
				cout << "PROCESS RESOURCE #" << j+1 << ": " << processes[i].resources[j].numHeld << "   ";
				cout << "AVAILABLE RESOURCE #" << j+1 << ": " << numAvailable[j] << "\n";
			}
			cout << endl;
			
			safeSequences.push_back(i); // find safe sequence by taking the current process
			printSafeSequence( processes , numAvailable, safeSequences );
            safeSequences.pop_back();
			

            processes[i].isChecking = 0;

            // cout << "Resource count after REALLOCATION:" << endl;
            for ( int j = 0; j < R; j++ ) {
                numAvailable[j] -= processes[i].resources[j].numHeld; // decrease the number of available resources
                // cout << "Process Resource #" << j+1 << ": " << processes[i].resources[j].numHeld << "   ";
				// cout << "Available Resource #" << j+1 << ": " << numAvailable[j] << "\n";
            }
            // cout << endl;
		} else if (!requestGranted( i, processes, numAvailable )){
			cout << "Process #" << i+1 << " unable to be executed." << endl;
			if ( deadlocked ) {
				cout << "The system is not in a safe state. A deadlock may occur.\n   Attempting to solve deadlock via RECOVERY. Initiating rollback." << endl;
			}
		}
	}

	// Print output if a safe sequence has been found
	if ( safeSequences.size() == P ) { 
        deadlocked = 0;
        cout << "Safe sequence #" << numSafe++ + 1 << ":" << endl;
        for ( int i = 0; i < P; i++ ) { 
            cout << "Process " << safeSequences[i] + 1; 
            if ( i != (P - 1) ) { // add arrow if not last element 
                cout << " → "; 
            } else {
            	cout << endl;
            }
        }
        cout << endl;
    }

    if ( deadlocked ) {
    	int safeIndex;
    	for( int j=0; j<safeSequences.size(); j++ ) {
    		safeIndex = safeSequences[j];
    		isGranted[safeIndex] = 1;
    		numGranted++;
    	}
    }
}

int main( ) {
	int numTestCases;
	cin >> numTestCases; // get number of test cases

	for( int i=0; i<numTestCases; i++ ) {
		cin >> P >> R; // get number of process and type of resources

		vector <Process> processes (P); // Initialize vector structure for processes

		isGranted = (bool*) malloc (sizeof(isGranted) * P); // Initialize array structure for the processes whose requests have been granted

		int numAvailable[R]; 
		for( int j=0; j<R; j++ ) { 
			cin >> numAvailable[j]; // get the number of each available resources
		}
		
		buildProcess( processes.data(), 0 ); // getting the input of processes and corresponding number of resources currently ALLOCATED 
		buildProcess( processes.data(), 1 ); // getting the input of processes and corresponding number of resources NEEDED; Deadlock PREVENTION (Hold and Wait)
		buildProcess( processes.data(), 2 ); // initializing the boolean variable to be used for safe sequence checking
		
		vector <int> safeSequences; // Initialize vector structure for safe sequences

		cout << "\nTest Case #" << i+1 << ":" << endl;

    	printSafeSequence( processes, numAvailable, safeSequences );
    	if ( deadlocked ) {
			cout << "The system is not in a safe state. A deadlock has occurred and could not be solved." << endl;
			if ( numGranted != 0 ) {
				cout << "The following process/es' request/s has been granted: " << endl;
				for( int j=0; j<P; j++ ) {
					if ( isGranted[j]) {
						cout << "Process " << j+1 << endl;
					}	
				}
			} else {
					cout << "None of the processes' requests has been granted." << endl;
				}
    		cout << endl;
    	} else {
    		cout << "\nThere is a total of " << numSafe << " safe-sequences" << endl;
    		if ( testCaseSolution == 0 ) {
    			cout << "Deadlock issue solved using PREVENTION and AVOIDANCE.\n" << endl;
    		} else {
    			cout << "Deadlock issue solved using RECOVERY.\n" << endl;
    		}
    	}
    	numSafe = 0;
		processes.clear();
		safeSequences.clear();
		free(isGranted);
	}
	return 0;
}
