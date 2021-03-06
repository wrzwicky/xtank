/*
** Xtank
**
** Copyright 1993 by Pix Technologies Corp.
**
** scroll.h
*/

/*
$Author: lidl $
$Id: scroll.h,v 1.1.1.1 1995/02/01 00:25:43 lidl Exp $
*/

#define MAXSPAN 23

  typedef struct {
	  int win;					/* xtank window id */
	  int x, y;					/* x & y pos of scroll bar */
	  int w, h;					/* Width & Height of bar */
	  int pos;					/* Index of item at top of bar */
	  int span;					/* #of items that fit in the onscreen menu */
	  int total;				/* #of items that fit in the virtual menu */
  }
scrollbar;
