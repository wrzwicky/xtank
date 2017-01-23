/*
** Xtank
**
** Copyright 1993 by Pix Technologies Corp.
**
** setup.h
*/

/*
$Author: lidl $
$Id: setup.h,v 1.1.1.1 1995/02/01 00:25:43 lidl Exp $
*/

#ifndef _SETUP_H_
#define _SETUP_H_

  typedef struct {
	  char vehicle_name[MAX_STRING];
	  Byte player;				/* %%% only one? */
	  Byte program;				/* %%% only one? */
	  Team team;
  }
Comb;


  typedef struct {
	  char name[MAX_STRING];	/* name of the setup */
	  char maze_name[MAX_STRING];	/* name of the maze used in the setup
					   */
	  Comb comb[MAX_VEHICLES];	/* short descriptions of each combatant */
	  Settings settings;		/* the settings for the game */
  }
Sdesc;


#endif /* _SETUP_H_ */
