/*
 *  Copyright (C) 2012  Samo Pogacnik
 */

/*
 * The daemonize module
 */
#ifndef daemonize_c
#define daemonize_c
#else
#error Preprocesor macro daemonize_c conflict!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "daemonize/daemonize.h"

static int daemonize_retval = 0;
static int daemonize_exit = 0;

static void controll_handler(int signum, siginfo_t *siginfo, void *context)
{
	if (signum == SIGRTMIN) {
		daemonize_retval = siginfo->si_value.sival_int;
		daemonize_exit = 1;
	}
}

static void controll_process(void)
{
	while (daemonize_exit == 0) {
		pause();
	}

	exit(EXIT_SUCCESS - daemonize_retval);
}

int daemonize(void)
{
	pid_t pid, sid;
	pid_t controll_pid;
	FILE *file;
	struct sigaction sact;
	union sigval sval;

	/* Get own PID */
	controll_pid = getpid();
	sact.sa_flags = SA_SIGINFO;
	sact.sa_sigaction = controll_handler;
	sigemptyset(&sact.sa_mask);
	if (sigaction(SIGRTMIN, &sact, NULL) == -1) {
		return -1; /* sigaction error */
	}

	/* Initial fork to controll deamonization from initial process. */
	pid = fork();
	if (pid < 0)
		return -2; /* initial fork error */

	/* PARENT process: terminate! */
	if (pid > 0) {
		waitpid(pid, NULL, 0);
		controll_process();
	}

	/* CHILD process: proceed! */
	{
		pid_t parent_pid;

		/* Get own PID */
		parent_pid = getpid();

		/* Fork for the first time to be able to setsid() in a child after parent termination. */
		pid = fork();
		if (pid < 0) {
			kill(controll_pid, SIGTERM);
			return -3; /* first fork error */
		}

		/* PARENT process: terminate! */
		if (pid > 0)
			_exit(EXIT_SUCCESS);

		/* CHILD process: proceed! */
		{
			/* Do not continue until INIT becomes the parent PID (not necessary 1)! */
			while (getppid() == parent_pid);

			/* Create a new SID for the child process */
			sid = setsid();
			if (sid < 0) {
				sval.sival_int = -11; /* setsid error */
				sigqueue(controll_pid, SIGRTMIN, sval);
				return sval.sival_int;
			}

			/* Fork for the second time not to be able to regain controlling terminal in a child. */
			pid = fork();
			if (pid < 0) {
				sval.sival_int = -12; /* second fork error */
				sigqueue(controll_pid, SIGRTMIN, sval);
				return sval.sival_int; /* fork error */
			}

			/* PARENT process: terminate! */
			if (pid > 0)
				_exit(EXIT_SUCCESS);

			/* CHILD process: proceed! */
			{
				/* Ensure default handlers */
				signal(SIGRTMIN, SIG_DFL);
				signal(SIGTERM, SIG_DFL);

				/* Cancel certain signals */
				signal(SIGCHLD, SIG_IGN); /* A child process dies */
				signal(SIGTSTP, SIG_IGN); /* Various TTY signals */
				signal(SIGTTOU, SIG_IGN);
				signal(SIGTTIN, SIG_IGN);
				signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */

				/* Change the current working directory to root "/" to prevent the current
				 * directory from being in use by the daemon. */
				if ((chdir("/")) < 0) {
					sval.sival_int = -21; /* chdir error */
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}

				/* Change the file mode mask: take controll over files we create */
				umask(0);

				/* Redirect standard files to /dev/null */
				if ((file = freopen("/dev/null", "r", stdin)) == NULL) {
					sval.sival_int = -22;
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}
				if (fileno(file) != 0) {
					sval.sival_int = -23;
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}
				if ((file = freopen("/dev/null", "w", stdout)) == NULL) {
					sval.sival_int = -24;
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}
				if (fileno(file) != 1) {
					sval.sival_int = -25;
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}
				if ((file = freopen("/dev/null", "w", stderr)) == NULL) {
					sval.sival_int = -26;
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}
				if (fileno(file) != 2) {
					sval.sival_int = -27;
					sigqueue(controll_pid, SIGRTMIN, sval);
					return sval.sival_int;
				}
			}
		}
	}

	sval.sival_int = 0;
	sigqueue(controll_pid, SIGRTMIN, sval);
	return sval.sival_int;
}

