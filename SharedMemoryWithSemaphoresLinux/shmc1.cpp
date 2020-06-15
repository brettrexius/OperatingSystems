/* shmc1.cpp */

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

CLASS *class_ptr; // declare a pointer to class called class_ptr
void 	*memptr; // declare a function pointer called memptr
char	*pname; // declare a char pointer called pname
int	shmid, ret; // declare two ints called shmid and ret
sem_t *sem; // declare a semaphore pointer
char *semChar; // declare a char pointer
void rpterror(char *), srand(), perror(), sleep(); // declare functions called rpterror, srand, perror, sleep
													// srand() initializes random number generator
													// perror() produces a message on standard error describing
													// the last error encountered suring a cal to a system or
													// library function
													// sleep() delays further execution of a program until the specified
													// time in seconds (parameter) has passed
void sell_seats(); // declare a function sell_seats

main(int argc, char* argv[])// use argc and argv[] to deal ewith arguments passed from the command line
							// argc holds the number of arguments
							// argv is an array with holds arguments passed to the program
{
	if (argc < 3) { // check to make sure there are at least 3 passed arguments
		fprintf(stderr, "Usage:, %s shmid\n", argv[0]); // if there are less than 2 parameters,
														// standard error message will be output with the first argv
														// as the error in the message with the shared memory id
		exit(1); //terminated unsuccessfully returning error status 1
	}

	pname = argv[0]; // define pname as first argv parameter
	sscanf(argv[1], "%d", &shmid); // stores the second argv parameter as an integer into shmid
	semChar = argv[2]; // stores the third argv parameter as semChar
	memptr = shmat(shmid, (void *)0, 0); //shmat() system call attaches shared memory segment identified by shmid to the address
										// space.  Parameter 0 is a null pointer
	if (memptr == (char *)-1) { // if memptr = -1, then the shared memory did not attach
		rpterror("shmat failed"); // uses "shmat failed" as parameter to print error message in rpterror
		exit(2); // exit returns error status 2
	}

	class_ptr = (struct CLASS *)memptr; // class_ptr of type CLASS is defined as memptr to CLASS

	sem = sem_open(semChar, 0); // sem_open opens a named semaphore of name semChar (s). 0 is null because nothing is being created.

	sell_seats(); // calls sell_seats() function

	ret = shmdt(memptr);// shmdt system call detatches memptr from shared memory segment
	exit(0); // fully portable, successful termination
}

void sell_seats() //define sell_seats
{
	int all_out = 0; // declare/define integer all_out to 0

	srand((unsigned)getpid()); // set seed as pid of this process
	while (!all_out) {   /* loop to sell all seats */
		sem_wait(sem); // sem_wait() decrements (locks) the semaphore pointed to by sem.  If
						// the semaphore's value is greater than zero, then the decrement
						// proceeds, and the function returns, immediately.If the semaphore
						// currently has the value zero, then the call blocks until either it
						// becomes possible to perform the decrement(i.e., the semaphore value
						// rises above zero), or a signal handler interrupts the call.
						// Credit goes to http://man7.org/linux/man-pages/man3/sem_timedwait.3.html
		if (class_ptr->seats_left > 0) { // make sure there are available seats before it sells a seat
			sleep((unsigned)rand() % 5 + 1); // postpone execution between 1 and 5 seconds, chosen randomly
			class_ptr->seats_left--; // decrement seat count as one seat is removed
			sleep((unsigned)rand() % 5 + 1); // // postpone execution between 1 and 5 seconds, chosen randomly
			cout << pname << " SOLD SEAT -- " // prints that the process (pname) sold a seat
				<< class_ptr->seats_left << " left" << endl; // prints how mant seats are left
		}
		else { //if there are no more seats
			all_out++; // increment all_out
			cout << pname << " sees no seats left" << endl; // prints that process "pname sees no seats left"
		}
		sem_post(sem); // sem_post() increments (unlocks) the semaphore pointed to by sem
		sleep((unsigned)rand() % 10 + 1); // postpone execution between 1 and 10 seconds, chosen randomly
	}
}

void rpterror(char* string) // rpterror passes a char pointer called "string"
{
	char errline[50]; // declare a char array called "errline" size 50

	sprintf(errline, "%s %s", string, pname); //prints a line of size errline, "'string' 'pname'"
	perror(errline); // will output last encountered error within errline
}
