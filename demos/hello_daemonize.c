/*
 * The hello_daemonize demo program
 *
 * Copyright (C) 2015 Samo Pogacnik <samo_pogacnik@t-2.net>
 * All rights reserved.
 *
 * This file is part of the DAEMONIZE software project.
 * This file is provided under the terms of the BSD 3-Clause license,
 * available in the LICENSE file of the "daemonize" software project.
*/

/*
 * This demo shows daemonize library usage.
*/

#ifndef hello_daemonize_c
#define hello_daemonize_c
#else
#error Preprocesor macro hello_daemonize_c conflict!
#endif

#include <stdlib.h>

int main(int argc, char *argv[])
{
	int ret;

	if ((ret = daemonize()) < 0) {
		exit(EXIT_FAILURE);
	}

	/* Leave daemon alive for some time. */
	sleep(10);

	exit(EXIT_SUCCESS);
}

