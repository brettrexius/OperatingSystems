#include <iostream>	// includes ios, streambuf, istream, ostream, and iosfwd
					// Needed for cin, cout, cerr, clog, wcin, wcout, wcerr, wclog
#include <algorithm> // includes necessary functions for certain algorithms
#include <iterator> // includes functions that are necessary for certain iterations
#include <ctime> // contains definitions to get and manipulate date and time information
#include <time.h> // declares the structure tm, which includes, seconds, minutes, hours, day of the month, months,
					// years since 1900, days of the week, days of the year, and daylight savings flag
#include <cstdlib> // defines several general purpose funcitons, including dynamic memory management,
					// random number generation, communication with the environment, integer arithmetics,
					// searching, sorting, and converting
#include <sys/types.h> // file access and modification times structure
						// tms structure returned by times()
						// includes clock_t tms_utime for user CPU time
								// clock_t tms_stime for system CPU time
								// clock_t tms_cutime for user CPU time of terminated child process
								// clock_t tms_cstime for system CPU time of terminated child process
#include <sys/ipc.h> // for ftok
					 // interprocess communication access structure
					// Used by messages, semaphores, and shared memory for interprocess communication
					// by means of ipc_perm structure to pass information used to determine
					// permission to perform IPC operation
					// ipc_perm contains uid_t uid for owner's user ID
									// gid_t gid for owner's group ID
									// uid_t cuid for creator's user ID
									// cgid_t uid for creator's group ID
									// mode_t mode for read/write permission
#include <sys/msg.h> // message queue structures
					// defines members of the msqid_ds structure
					// Uses msgqnum for the number of messages in the message queue
					// Uses msglen for the number of bytes allowed in a message queue
					// Uses MSG_NOERROR as a message operation flag to catch an error
					// msqid_ds contains msgqnum_t msg_qnum for number of messages currently on queue
										// msglen_t msg_qbytes for maximum number of bytes allowed on queue
										// pid_t msg_lspid for process ID of last msgsnd()
										// pid_t msg_lrpid for process ID of last msgrcv()
										// time_t msg_stime for time of last msgsnd()
										// time_t msg_rtime for time of last msgrcv()
										// time_t msg_ctime for time of last change
					// Needed for msgctl, msgget, msgrcv, and msgsnd
#include <cstring> // defines several funcions to manipulate C strings and arrays
#include <sys/wait.h> // declarations for waiting
					// Defines the symbolic constants for use with waitpid()
#include <unistd.h> // standard sumbolic constants and types		
					// Defines miscellaneous symbolic constants and types and delcares miscellaneous functions
					// Needed for access, alarm, brk, chdir, chroot, chown, close, confstr, crypt, cstermid,
					// cuserid, dup, dup2, encrypt, execle, execle, execlp, execv, execve, execvp, _exit, 
					// fchown, fchdir, fdatasync, fork, fpathconf, fsync, ftruncate, getcwd, gettablesize, etegif,
					// getuid, getid, getgroups, gethostid, getlogin, getlogin_r, getopt, getpagesize, getpass,
					// getpgid, getpgrp, getpid, getppid, getdis, getuid, getwd, isatty, lchown, link, lockf, lseek,
					// nice, pathconf, pause, pipe, pread, pthread_atfork, pwrite, read, readlink, rmdir, sbrk,
					// setgid, setpgrp, setregid, setreuid, setsid, setuid, setsid, setuid, sleep, swab, symlink,
					// sync, sysconf, tcgetpgrp, truncate, ttyname, ttyname_r, ualarm, unlink, unsleep, vfork, write
#include <string> // string operations
					// Defines Null for a null pointer constant and size_t
					// Needed for memccpy, memchr, memcmp, memcpy, memmove, memset,strcat, strchr, strcmo,
					// strcoll, strcpy, strcspn, strdup, strerror, strlen, strncat, strncmp, strncpy,
					// strpbrk, strrchr, strspn, strstr, strtok, strtok_r, and strxfrm
					// Makes all symbols from stddef.h visible (if appicable)

using namespace std;

int main() {
	int qid = msgget(ftok(".", 'u'), IPC_EXCL | IPC_CREAT | 0600); // will return the message queue identifier of the qid
																// key being equal to IPC_PRIVATE means the message queue identifier,
																// associated message queue, and data structure will be created for the argument key
																// IPC_CREAT ensures the above statement will be true considering the argument
																// key does not already have a message queue identifier associated with it
																// ftok returns a key based on the path and id which is usable in msgget()
																// path is '.' and id is 'u'.

	cout << "Master, PID " << getpid() << ", has created message queue: " << qid << "." << endl; // Master (parent) announces that it has created message queue
	cout << "Master, PID " << getpid() << ", is spawning two child processes, 'Sender' and 'Receiver." << endl; // Master announces it is creating child processes
	//spawning two child processes
	pid_t cpid = fork(); // create first child process
	if (cpid < 0) {
		cout << "Fork failed." << endl; // If the cpid is < 0, the creation process has failed and a child processes was not spawned
	}
	if (cpid == 0) { // the first child process (Sender) is created and will be returned the value of 0. PID being one greater than the PID of Master (parent) will be sent to Master.
		execlp("./sender", "sender", NULL); // execlp function replaces current process with a new process
											// When I compile sender.cpp I name it sender so an executable file named has been created, which allows for Master to run the executable file at this point in the program.
											// The last argument must be a null pointer
		exit(0);
	}
	sleep(4); // wait for four seconds to give Sender process a chance to finish execution, since it will take some time for user to enter message.
	cout << "Master, PID " << getpid() << ", acknowledges that Sender, PID " << cpid << ", sends this message." << endl; // cpid returned to master from the first fork is output as the sender's PID
	
	cpid = fork(); // create second child processes
	if (cpid < 0) {
		cout << "Fork failed." << endl;// If the cpid is < 0, the creation process has failed and a child processes was not spawned
	}
	if (cpid == 0) { // the second child process (Receiver) is created and will be returned the value of 0. PID of this child being one greater than the PID of Sender will be sent to Master.
		execlp("./receiver", "receiver", NULL); // execlp function replaces current process with a new process
											// When I compile receiver.cpp I name it receiver so an executable file named has been created, which allows for Master to run the executable file at this point in the program.
											// The last argument must be a null pointer
		exit(0);
	}
	sleep(1); // wait for one second to give Receiver a change to finish execution
	cout << "Master, PID " << getpid() << ", acknowledges that Receiver, PID " << cpid << ", receives this message." << endl; // cpid returned to master from the second fork is output as the sender's PID

	while (wait(NULL) != -1); // waiting for both child processes to terminate

	cout << "Message queue: " << qid << ", has been terminated by the Master, PID " << getpid() << "." << endl; // Master announces that it has terminated the message queue

	msgctl(qid, IPC_RMID, NULL); // Removes the message queue identifier based on qid
								// IPC_RMID removes the interprocess communication identifier

	cout << "Master, PID " << getpid() << ", now exits." << endl; // Master announces its exit

	exit(0); // successful termination of Master
}