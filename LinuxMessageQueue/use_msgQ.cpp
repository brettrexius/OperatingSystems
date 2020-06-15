// use_msgQ.cpp
// Brett Rexius
// CECS 326
// Assignment 2

#include <sys/types.h> // file access and modification times structure
						// tms structure returned by times()
						// includes clock_t tms_utime for user CPU time
								// clock_t tms_stime for system CPU time
								// clock_t tms_cutime for user CPU time of terminated child process
								// clock_t tms_cstime for system CPU time of terminated child process
			
#include <sys/ipc.h> // interprocess communication access structure
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

#include <string.h> // string operations
					// Defines Null for a null pointer constant and size_t
					// Needed for memccpy, memchr, memcmp, memcpy, memmove, memset,strcat, strchr, strcmo,
					// strcoll, strcpy, strcspn, strdup, strerror, strlen, strncat, strncmp, strncpy,
					// strpbrk, strrchr, strspn, strstr, strtok, strtok_r, and strxfrm
					// Makes all symbols from stddef.h visible (if appicable)

#include <iostream>	// includes ios, streambuf, istream, ostream, and iosfwd
					// Needed for cin, cout, cerr, clog, wcin, wcout, wcerr, wclog

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

#include <sys/wait.h> // declarations for waiting
					// Defines the symbolic constants for use with waitpid()
					
#include <stdlib.h> // standard library definitions
					// Used for exits, division, randomizations, sorting, memory allocation, and nulls
					// Needed for _Exit, a64l, abort, abs, atexit, atof, atoi, atol, atoll, bsearch, calloc, div,
					// drand48,ecvt, erand48, exit, fcvt, free, gcvt, getenv, getsubopt, grantpt, initstate, jrand48,
					// l64a, labs, lcong48, ldiv, llabs, lldiv, lrand48, malloc, mblen, mbstowcs, mbtowc, mktemp,
					// mkstemp, mrand48, nrand48, posix_memalign, posix_openpt, ptsname, putenv, qsort, rand, rand_r,
					// random, realloc, realpath, setenv, setkey, setstate, srand, srand48, srandom, strtod, strtof,
					// strtol, strtold, strtoll, strtoul, strtoull, system, unlockpt, unsetenv, wcstombs, wctomb

using namespace std;

// declare my global message buffer
struct buf {
	long mtype; // required
	char greeting[50]; // message content
};

// cout << getpid() outputs PID of current process

void child_proc_one(int qid) { // Create Child Process One

	buf msg; // declaring a message type buf
	int size = sizeof(msg) - sizeof(long); // defines an integer valued at the size in bytes of type of message -
											// size in bytes of a long (my linux will value this at 4 bytes)
											

	msgrcv(qid, (struct msgbuf *)&msg, size, 113, 0); //msgrcv() system call receives a message from the queue from child process two
														// specified by the qid and places it in the buffer
														// pointed to by the msgbuf
														// Reads the message from the messag queue and places it in the message buffer
														// msgtyp = 113 meaning that the first message of this type is received
														// msgflg = 0 which means that the it will not execute until a message of the desired type
														// is placed on the queue, the message queue identifier defined in qid is removed, returning -1, or
														// this thread receives a signal from another process

	cout << getpid() << ": gets message" << endl; // getpid() returns the PID of the calling process
												
	cout << "message: " << msg.greeting << endl; // Calls the message "greeting" from message queue
												// greeting = "Hello there" from msgsnd in child process two

	strcat(msg.greeting, " and Adios."); // source " and Adios." is added to destination string "greeting" to create "Hello there and Adios."
	cout << getpid() << ": sends reply" << endl;
	msg.mtype = 114; 	// prepare message with type mtype = 114
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); // Sends "Hello there and Adios." to message queue
												// sends a message on the message queue of qid
												// of size msg - mtype
												// msgflg = 0 means execution will wait until the other process is finished

	cout << getpid() << ": now exits" << endl; // Child process one announces it exits
}

void child_proc_two(int qid) { // Create Child Process Two

	buf msg; // declaring a message type buf
	int size = sizeof(msg) - sizeof(long); //size in bytes of msg type - size in bytes of a long

	// sending a message that will never be read
	msg.mtype = 12; // Recieved message's type is of type 12
	strcpy(msg.greeting, "Fake message"); // Would copy "Fake message" to "greeting"
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); // Would send the above message to the message queue
												// would a message on the message queue of qid
												// of size msg - mtype
												// msgflg = 0 means execution will wait until the other process is finished

	// prepare my message to send
	strcpy(msg.greeting, "Hello there"); // copies "Hello there" to greeting

	cout << getpid() << ": sends greeting" << endl; // Beginning of child process one calls this line
													// before "Hello there" is put in the queue
	msg.mtype = 113; 	// set message type mtype = 113	
	msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sends the message "Hello there" to child process one from strcpy function
												// sends a message on the message queue of qid
												// of size msg - mtype
												// msgflg = 0 means execution will wait until the other process is finished

	msgrcv(qid, (struct msgbuf *)&msg, size, 114, 0); // Receives "Hello there and Adios." from the message queue
														//msgrcv() system call receives a message from the queue from child process one
														// specified by the qid and places it in the buffer
														// pointed to by the msgbuf
														// Reads the message from the messag queue and places it in the message buffer
														// msgtyp = 114 meaning that the first message of this type is received
														// msgflg = 0 which means that the it will not execute until a message of the desired type
														// is placed on the queue, the message queue identifier defined in qid is removed, returning -1, or
														// this thread receives a signal from another process
	cout << getpid() << ": gets reply" << endl; // Child process two announces it got the reply from child process one
	cout << "reply: " << msg.greeting << endl; // Outputs "Hello there and Adios."
	cout << getpid() << ": now exits" << endl; // Child process two announces it exits
}

int main() {

	int qid = msgget(IPC_PRIVATE, IPC_EXCL | IPC_CREAT | 0600); // will return the message que identifier of the qid
																// key being equal to IPC_PRIVATE means the message queue identifier,
																// associated message queue, and data structure will be created for the argument key
																// IPC_CREAT ensures the above statement will be true considering the argument
																// key does not already have a message queue identifier associated with it

	//spawning two child processes
	pid_t cpid = fork();
	if (cpid == 0) {
		child_proc_one(qid); // Create a child process based on parent process qid
							// pid of child process one will be one greater than parent process
		exit(0);
	}
	cpid = fork();
	if (cpid == 0) {
		child_proc_two(qid); // Create a child process based on parent process qid
							// pid of child two will be one greater than child process one
		exit(0);
	}

	while (wait(NULL) != -1); // waiting for both children to terminate

	msgctl(qid, IPC_RMID, NULL); // Removes the message queue identifier based on qid
								// IPC_RMID removes the interprocess communication identifier

	cout << "parent proc: " << getpid() 
		<< " now exits" << endl; // Parent process announces it exits

	exit(0); // Exit main program
}
