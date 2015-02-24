/*
 *  Copyright (C) 2015  Samo Pogacnik
 */

/*
 * The daemonize module
 */

#ifndef daemonize_h
#define daemonize_h

#ifdef daemonize_c
/* PRIVATE usage of the PUBLIC part. */
#	undef EXTERN
#	define EXTERN
#else
/* PUBLIC usage of the PUBLIC part. */
#	undef EXTERN
#	define EXTERN extern
#endif
/*
 * Here starts the PUBLIC stuff:
 *	Enter PUBLIC declarations using EXTERN!
 */

EXTERN int daemonize(void);

#ifdef daemonize_c
/*
 * Here is the PRIVATE stuff (within above ifdef).
 * It is here so we make sure, that the following PRIVATE stuff get included into own source ONLY!
 */

#endif /*daemonize_c*/
/*
 * Here continues the PUBLIC stuff, if necessary.
 */

#endif /*daemonize_h*/

