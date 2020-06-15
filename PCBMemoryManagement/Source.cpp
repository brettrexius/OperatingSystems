//Brett Rexius 018795528
//CECS 326 03
//9/18/2019
//Assignment 1
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <deque>
#include <string>

using namespace std;

struct PCB { //Process control block with PID, starting block, ending block, page table size, page table pointer, page table array
	int PID; //Unique ID given to each process
	int blockA; //Starting point of the blocks taken up by the current process
	int blockB; //Ending point of the blocks taken up by the current process
	int tableSize; //Size of page table will be equal to the random generated number (the amount of space it takes up)
	int *tablePointer; //Pointer to the page table
	//int pageTable[]; //Integer array for table pointers

	PCB(int pid, int ts, int *tp, int bA, int bB) { //Assign values for PCB
		this->PID = pid;
		this->tablePointer = new int[ts]; //Create an array of pointers the size of the page table
		//this->pageTable = new int[ts];
		this->blockA = bA;
		this->blockB = bB;
		this->tableSize = ts;
		for (int i = 0; i < ts; i++) { //Assign pointers to table pointer array
			*(tablePointer + i) = tp[i]; //Give values to table pointer array
		}
	}
	PCB() { //Default Constructor
	}
	~PCB() {
		//delete tablePointer; //Destructor (Causing Crash)
	}	
};

deque<PCB>q; //Double ended empty ready queue

bool boolArray[1024]; //Array of 1024 1s and 0s (boolean)
int *pageTable; //page table pointer
int PID = 1; //Starting PID = 1
int *MBT(bool *boolArray, int number) { //Creates boolArray (Memory Cobtrol Block) with the random number we are going to generate
	int count = 0; //Variable to make the table 16 rows and 64 columns
	int j = 0; //Index iterator for page table
	int i = 0; //Array iterator
	pageTable = new int[number]; //Create a page table array of size random number

	while (number > 0) { //Start at the first true (free) block and make it false (taken) until the generated number decrements to 0
		if (boolArray[i] == true) { //Change MBT block from free to taken
			boolArray[i] = false;
			pageTable[j++] = i; //Add index to page table array
			//cout << i << " "; //Check page table correctness
			i++;
			number--;
		}
		else {
			i++;
		}
		if (i == 1024) {
			break;
		}		
	}
	cout << endl;
	return pageTable;
}

void printMBT() {
	cout << endl << "	Key: 0 = taken		1 = free" << endl; //Key for MBT 0s and 1s
	int count = 0; //Make table look nice
	for (int i = 0; i < 1024; i++) { //Priny out boolArray MBT
		cout << boolArray[i];
		count++;
		if (count == 64) //Make table look nice
		{
			cout << endl;
			count = 0;
		}
	}
	cout << endl;
}

int main()
{
	for (int i = 0; i < 31; i++) { //Create MBT with 32 blocks of memory taken (false) by the OS
		boolArray[i] = false;
	}
	for (int i = 32; i < 1024; i++) { //The remaining blocks are free (true)
		boolArray[i] = true;
	}

	int choice = 0; //User input
	int tableSize;
	int blockA = 32; //Starting block starts at slot 32 (33rd block)
	int blockB;

	while (choice != -1) {
		cout << "1: Initiate a process" << endl << "2: Print system state" << endl << "3: Terminate process with a specific PID" << endl << "4: Exit" << endl; //Menu
		cin >> choice;
		if (choice < 1 || choice > 4) //bounds
		{
			cout << "Choose a number 1 - 4" << endl;
			continue;
		}
		cout << endl;
		if (choice == 1) { //Initiate a process
			srand(unsigned(time(0))); //Set clock to 0 to get different numbers for different processes
			int number = (rand() % (241) + 10); //Generate random number
			cout << "Required memory blocks: " << number << " blocks" << endl;
			int space;
			for (int i = 0; i < 1024; i++) { //Define space as how much free space is in the array
				if (boolArray[i] == 1)
				{
					space = 1024 - i; //Calculate free space
					break;
				}
			}
			cout << "Available space: " << space << " blocks" << endl << endl;
			if (number > space) { //Not enough space
				cout << "Error: Insufficient Space" << endl;
			}
			else {	
				int *PageTable = MBT(boolArray, number); //Create MBT for the generated number.  Add 0s to the array to represent blocks that are not free
				/*for (int i = 0; i < number; i++) { //Check if page table is working correctly
					cout << PageTable[i] << " ";
				}*/
				int *pt = pageTable; //Point to page table				
				tableSize = number; //Page table is as big as the generated number			
				blockB = blockA + number; //Calculate end block for current process
				PCB newPCB = { PID, tableSize, pt, blockA, blockB }; //Create PCB for current process
				PCB *pPointer = &newPCB; //Point to address of current PCB
				q.push_back(newPCB); //Add PCB to the ready queue
				PID++; //Increment PID
				blockA = blockB + 1; //Starting point for next process
			}
		}
		else if (choice == 2) { //Print system state		
			for (int i = 0; i < q.size(); i++) { //Iterate through the queue and output all processes' PID's, sizes, and which blocks they takes up
				cout << endl;
				cout << "PID: " << q[i].PID << "		"; //Current PID
				cout << "Size: " << q[i].tableSize << "		"; //Current table size
				cout << endl;
				int count = 0; //Make table look nice
				for (int j = 0; j < q[i].tableSize; j++) {
					cout << q[i].tablePointer[j] << "	"; //Print out each block of the MBT that is in the current page table of each queue
					count++;
					if (count == 14) { //Make page table look nice
						cout << endl;
						count = 0;
					}
				}
				cout << endl;
				//cout << "Blocks #: " << q[i].blockA << " through " << q[i].blockB << endl;
			}
	
			printMBT(); //Print the MBT
		}
		else if (choice == 3) { //Terminate process with a specific PID
			int numPID; //PID of process to be terminated
			cout << "PID of process to be terminated: ";
			cin >> numPID;
			int check = 0; //Variable to see if process exists
			for (int i = 0; i < q.size(); i++) {
				if (numPID == q[i].PID) {
					cout << "Process with PID " << q[i].PID << " erased." << endl;
					//cout << "Blocks " << q[i].blockA << " through " << q[i].blockB << " are now free." << endl << endl;
					int a = q[i].blockA;
					int b = q[i].blockB;
					int j = i;
					for (j = a - 1; j < b; j++) { //Memory deallocation.  Free up the blocks previously allocated by terminated process
						boolArray[j] = true;
					}
					q.erase(q.begin() + i); //Delete process
					check = 1; //Verify that process chosen existed					
					break;
				}
			}
			if (check == 0) { //If process doesn't exist
				cout << "Error: The process with PID " << numPID << " was not found." << endl;
			}
		}
		else if (choice == 4) { //Exit
			while (true) {
				if (q.size() == 0) { //Exit if ready queue is empty
					cout << "Ready queue is empty. Good bye." << endl;
					system("pause");
					return 0;
				}
				cout << "Ready Queue: "; //Show PID's of all processes currently in the ready queue
				for (int i = 0; i < q.size(); i++) {
					cout << "PID " << q[i].PID << " ";
				}
				string confirm;
				cout << endl << "Are you sure you would like to exit?" << endl; //Exit confirmation
				cin >> confirm;
				if (confirm == "yes") {
					for (int i = 0; i < q.size(); i++) {
						q.erase(q.begin() + i); //Delete all processes in ready queue
					}
					for (int i = 0; i < 31; i++) { //Create MBT with 32 blocks of memory taken (false) by the OS
						boolArray[i] = false;
					}
					for (int i = 32; i < 1024; i++) { //The remaining blocks are now free (true)
						boolArray[i] = true;
					}
					cout << endl <<  "All memory has been deallocated. Goodbye." << endl;
					system("pause");
					return 0;
				}
				else if (confirm == "no") {
					cout << endl; //If no, return to menu
					break;
				}
				else {
					cout << endl << "Please answer 'yes' or 'no'." << endl << endl; //User has to answer 'yes' or 'no'
				}
			}
		}
	}
}