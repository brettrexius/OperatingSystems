/* shmp1.cpp */

#include "registration.h"// include header file
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
#include <stdlib.h> // standard library definitions
					// Used for exits, division, randomizations, sorting, memory allocation, and nulls
					// Needed for _Exit, a64l, abort, abs, atexit, atof, atoi, atol, atoll, bsearch, calloc, div,
					// drand48,ecvt, erand48, exit, fcvt, free, gcvt, getenv, getsubopt, grantpt, initstate, jrand48,
					// l64a, labs, lcong48, ldiv, llabs, lldiv, lrand48, malloc, mblen, mbstowcs, mbtowc, mktemp,
					// mkstemp, mrand48, nrand48, posix_memalign, posix_openpt, ptsname, putenv, qsort, rand, rand_r,
					// random, realloc, realpath, setenv, setkey, setstate, srand, srand48, srandom, strtod, strtof,
					// strtol, strtold, strtoll, strtoul, strtoull, system, unlockpt, unsetenv, wcstombs, wctomb
					// FILE, fpos_t, va_list, size_t
#include <iostream>	// includes ios, streambuf, istream, ostream, and iosfwd
					// Needed for cin, cout, cerr, clog, wcin, wcout, wcerr, wclog
#include <stdio.h> // defines the following macro names as positive integral constant expressions
					// BUFSIZ, FILENAME_MAX, FOPEN_MAX, _IOFBF, L_ctermid, L_cuserid, L_tmpnam, SEEK_CUR, SEEK_END,
					// SEEK_SET, TMP_MAX, 
					// defines the following as a negative integram constant expression
					// EOF, NULL, P_tmpdir
					// defines the following as expressions of type pointer to file
					// stderr, stdin, stdout
					// defines the following through typedef
#include <memory.h> // contains declarations for memory operations
#include <semaphore.h> // Works with fcnt.h and sys/types.h to make symbols visible
						// Needed for POSIX implementaion of sem_t
						// sem_open, sem_close, sem_destroy, sem_getvalue, sem_init, sem_post, sem_trywait,
						// sem_unlink, sem_wait
#include <fcntl.h> // File control options.  Values for cmd are F_DUPFD, F-GETFD, F_SETFD, F_GETFL, F_SETLK,
					// F_SETLKW, F_GETOWN, F_SETOWN. File descriptor flags are FD_CLOEXEC.  Values for record locking
					// are F_RDLCK, F_UNLCK, AND F_WRLCK.  Values for advice are POSIX_FADV_NORMAL, POSIX_FADV_SEQUENTIAL,
					// POSIX FADV_RANDOM, POSIX_FADV_WILLNEED, POSIX_FADV_DONTNEED, and POSIX_FADV_NOREUSE.
#include <pthread.h> // Thread header used to link to shared memory.  Defines and describes pthread_attr_t,
					// pthread_cond_t, pthread_condattr_t, pthread_key_t, pthread_mutex_t, pthread_mutexattr_t,
					// pthread_once_t, pthread_rwlock_t, pthread_rwlockattr_t and pthread_t.
#include <sys/shm.h> // XSI shared memory facility needed for SHM_RDNONLY, SHM_RND, SHMLBA, shmatt_t, shmid_ds,
					// shm_segsz, shm_lpid, shmid_cpid, shm_nattch, shm_atime, shm_dtime, shm_ctime


using namespace std;

CLASS myclass = { "1001", "120119", "Operating Systems", 15 }; // class struct initializing
																// class_number = 1001
																// date = 120119
																// title = Operating Systems
																// seats_left = 15

#define NCHILD	3 // we are going to create 3 child processes
#define s "s" // global variable s


int	shm_init(void *); // declares function shm_init
void	wait_and_wrap_up(int[], void *, int, sem_t *); // declares function wait_and_wrap_up
void	rpterror(char *, char *); // declares function rpterror

main(int argc, char *argv[])// use argc and argv[] to deal ewith arguments passed from the command line
							// argc holds the number of arguments
							// argv is an array with holds arguments passed to the program
{
	int 	child[NCHILD], i, shmid; // child[NCHILD] stores the pid of the child process
									// declare i for a process counter
									// declare shmid as shared memory id
	sem_t *sem; // declare a semaphore pointer
	void	*shm_ptr;				// declare shared memory pointer
	char	ascshmid[10], pname[14]; // declare a char array ascshmid size 10
									// declare a char array pname size 14
	strcpy(pname, argv[0]);
	shmid = shm_init(shm_ptr); // initializes shared memory with identifier assigned to shmid
	sprintf(ascshmid, "%d", shmid); // prints line size aschmid with message 'shmid'
	sem = sem_open(s, O_CREAT, 0644, 1); // opens a named semaphore named s
										// O_CREAT creates a path as a regular file
										// 0644 allows permission of user to read and write from file
										// 1 is true flag
	for (i = 0; i < NCHILD; i++) { //create 3 child processes
		child[i] = fork(); // fork() system call creates child process. returns 0 to child, and unique pid
							// of child to parent process
		switch (child[i]) { // switch case for current child
		case -1: // if pid returned to child = -1, fork failed
			rpterror("fork failure", pname); // print error message with process name
			exit(1); // unsuccessful termination, return 1 as error status
		case 0: // if pid returned to child = 0, fork successful
			sprintf(pname, "shmc%d", i + 1); // print out 'shmc' of current process, size pname
			execl("shmc1", pname, ascshmid, s, (char *)0); // execlp system call replaces current process with a new process
														// new process is shmc1, passing pname and ascshim
														// as parameters, with a null pointer
			perror("execl failed");	//'execl failed' is the message output as last encountered error
			exit(2); // exit returns error status 2
		}
	}
	wait_and_wrap_up(child, shm_ptr, shmid, sem); // call function wait_and_wrap_up passing in
											// child, shmptr, shmid, and sem
}

int shm_init(void *shm_ptr) //parameter is shm_ptr
{
	int	shmid; //declare int shmid, shard memory id

	shmid = shmget(ftok(".", 'u'), sizeof(CLASS), 0600 | IPC_CREAT); //shmget gets a shared memory segment with size = size of CLASS
																	// Key is made of: "." is the path and "u" is id associated with shared memory
																	// 0600 
																	// IPC_CREAT creates a new data segment.  IF this flag is not used,
																	// shmget() will find the segment associated with "." and "u" and check to see
																	// if the user has permission to access the segment
	if (shmid == -1) { // print error message if attempt at acquiring shared memory segment failed
		perror("shmget failed");
		exit(3); // exit returns error status 3
	}
	shm_ptr = shmat(shmid, (void *)0, 0); //shmat attaches shared memory to shm_ptr
	if (shm_ptr == (void *)-1) { // if the shared memory does not attach, print "shmat failed"
		perror("shmat failed");
		exit(4); // exit returns error status 4
	}
	memcpy(shm_ptr, (void *)&myclass, sizeof(CLASS)); // memcpy copies the data of myclass into shm_ptr
	return (shmid); // returns shared memory id
}

void wait_and_wrap_up(int child[], void *shm_ptr, int shmid, sem_t *sem) // parameters are an integer array, shared memory pointer, and a shared memory id
{
	int wait_rtn, w, ch_active = NCHILD; // define wait_rtn and ch_active to NCHILD, which is defined as 3

	while (ch_active > 0) { // while there are still acive child processes
		wait_rtn = wait((int *)0); // wait for child process to terminate
		for (w = 0; w < NCHILD; w++) // for loop to loop through number of children
			if (child[w] == wait_rtn) { // check if the current child is waiting to terminate
				ch_active--; // Decrement the number of active child processes
				break; // if decremented, break out of loop
			}
	}
	cout << "Parent removing shm" << endl; // announcing removal of shared memory
	shmdt(shm_ptr); // detach  shared memory segment with shm_ptr
	shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0); // Remove shared memory identifier and terminate its shared memory segment
													// shmctl() performs the control operation specified by cmd on the
													// System V shared memory segment whose identifier is given in shmid.
													// IPC_RMID parameter IPC_RMID marks the segment to be destroyed, actually
													// destroying it only after the last process detaches it
													// (struct shmid_ds *) 0 is a pointer to shmid_ds structure
													// The 0 argument is a pointer to a shmid_ds structure, defined in
													// <sys / shm.h> as follows :

													// 	struct shmid_ds {
													// 		struct ipc_perm shm_perm;    /* Ownership and permissions */
													// 		size_t          shm_segsz;   /* Size of segment (bytes) */
													// 		time_t          shm_atime;   /* Last attach time */
													// 		time_t          shm_dtime;   /* Last detach time */
													// 		time_t          shm_ctime;   /* Last change time */
													// 		pid_t           shm_cpid;    /* PID of creator */
													// 		pid_t           shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
													// 		shmatt_t        shm_nattch;  /* No. of current attaches */
													// 		...
													// 	};
													// credit: http://man7.org/linux/man-pages/man2/shmctl.2.html
	sem_unlink(s); // remove named semaphore s.  The semaphore name is removed immediately.The semaphore is destroyed once
					// all other processes that have the semaphore open close it.
	sem_close(sem); // indicates that the calling process is finished using the named semaphore indicated by sem.
					// 0 is returned
	
	exit(0); // successful termination
}

void rpterror(char *string, char *pname) // rpterror passes a char pointer called "string" and "pname"
{
	char errline[50]; // declare a char array called "errline" size 50

	sprintf(errline, "%s %s", string, pname); //prints a line of size errline, "'string' 'pname'"
	perror(errline); // will output last encountered error within errline
}

