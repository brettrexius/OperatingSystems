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
#include <sys/types.h>// file access and modification times structure
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

struct buf {
	long mtype; // required
	char greeting[50]; // message content
};

int main() {
	cout << "Receiver, PID " << getpid() << ", begins execution." << endl; // Sender announces its execution.  PID was created at spawning process
	int qid = msgget(ftok(".", 'u'), 0); // Same path and key as Master and Sender so it will be receiveing the message on the same message queue. 0 means that it will not be creating a message queue.

	buf msg; // declaring a message type buf
	int size = sizeof(msg) - sizeof(long); // defines an integer valued at the size in bytes of type of message -
											// size in bytes of a long (my linux will value this at 4 bytes)


	msgrcv(qid, (struct msgbuf *)&msg, size, 113, 0); //msgrcv() system call receives a message from the queue sent from Sender
														// specified by the qid and places it in the buffer
														// pointed to by the msgbuf
														// Reads the message from the messag queue and places it in the message buffer
														// msgtyp = 113 meaning that the first message of this type is received
														// msgflg = 0 which means that the it will not execute until a message of the desired type
														// is placed on the queue, the message queue identifier defined in qid is removed, returning -1, or
														// this thread receives a signal from another process
	cout << "Receiver, PID " << getpid() << ", has received the message: " << msg.greeting << endl; // Receiver announces that it has received the user's message from the message queue and outputs the message
	
	cout << "Receiver, PID " << getpid() << ", now exits." << endl; // Receiver announces it exits


	exit(0); // Receiver terminates successfully

	

}