/* limits.h - fixed sizes for arrays and the like */

/*
$Author: lidl $
$Id: limits.h,v 2.7 1992/01/29 08:39:11 lidl Exp $

$Log: limits.h,v $
 * Revision 2.7  1992/01/29  08:39:11  lidl
 * post aaron patches, seems to mostly work now
 *
 * Revision 2.6  1992/01/08  06:55:51  lidl
 * upped MAX_ENTRIES to 47
 *
 * Revision 2.5  1991/12/03  20:13:34  lidl
 * updated to handle 12 body types
 *
 * Revision 2.4  1991/09/15  09:24:51  lidl
 * removed vestiges of config.h file, now all configuration is done in
 * the Imakefile, and propogated via compile-time -D flags
 *
 * Revision 2.3  1991/02/10  13:50:58  rpotter
 * bug fixes, display tweaks, non-restart fixes, header reorg.
 *
 * Revision 2.2  91/01/20  09:58:14  rpotter
 * complete rewrite of vehicle death, other tweaks
 * 
 * Revision 2.1  91/01/17  07:12:03  rpotter
 * lint warnings and a fix to update_vector()
 * 
 * Revision 2.0  91/01/17  02:09:50  rpotter
 * small changes
 * 
 * Revision 1.1  90/12/29  21:02:40  aahz
 * Initial revision
 * 
*/

#ifndef _LIMITS_H_
#define _LIMITS_H_

#define NUM_BOXES	4	/* a map area NUM_BOXES x NUM_BOXES is visible
				   to the player in the animation window */

/*
** Maximum values for arrays of landmarks, blips, vehicles, and bullets
** These should be used as the sizes of the landmark_info, blip_info,
** vehicle_info, and bullet_info arrays.
*/
#define MAX_VEHICLES	16
#define MAX_BLIPS	(MAX_VEHICLES + 6)
#define MAX_BULLETS	300
#define MAX_DISCS       16
#define MAX_LANDMARKS   50
#define MAX_EXPS	100
#define MAX_PROGRAMS    3	/* maximum number of programs allowed on any
				   one vehicle */
#define MAX_LINES       256	/* number of lines drawn in 3d mode */
#define MAX_SEGMENTS	6
#ifdef NO_NEW_RADAR
#define MAX_ENTRIES	47	/* t'port made 47... */
#else /* NO_NEW_RADAR */
#define MAX_ENTRIES	54	/* size of console_word array in console.c */
#endif
#define MAX_STRING	24
#define MAX_VIEWS	32
#define MAX_SPEED	25	/* fastest tanks are expected to go, though
				   going over is allowed */

#define MAX_WEAPONS     6	/* number of weapons allowed on any one tank */

#define MAX_MESSAGES    8


#define MAX_ENGINES     16
#define MAX_ARMORS       7
#define MAX_BODIES      12
#define MAX_SUSPENSIONS  4
#define MAX_BUMPERS      4

#define MAX_VEHICLE_OBJS	MAX_BODIES
#define MAX_TURRET_OBJS		1
#define MAX_EXP_OBJS		9
#define MAX_LANDMARK_OBJS	3

#define MAXPNAME	12	/* length of a player's name */

#ifdef UNIX
#define MAX_TERMINALS	10
#endif

#ifdef AMIGA
#define MAX_TERMINALS	1
#endif


#endif ndef _LIMITS_H_
