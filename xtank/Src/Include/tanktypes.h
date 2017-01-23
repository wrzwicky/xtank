/*
** Xtank
**
** Copyright 1993 by Pix Technologies Corp.
**
** Comment: simple typedefs that are used in many places
**
** tanktypes.h
*/

/*
$Author: lidl $
$Id: tanktypes.h,v 1.1.1.1 1995/02/01 00:25:44 lidl Exp $
*/

#ifndef _TANKTYPES_H_
#define _TANKTYPES_H_

  typedef enum {
	  False = 0, True = 1
  } Boolean;

#define FALSE ((int)False)		/* lint likes these better :-) */
#define TRUE ((int)True)
typedef unsigned char Byte;
typedef unsigned int Flag;		/* a set of bits */
typedef FLOAT Angle;

  typedef struct {
	  short x, y;
  }
Coord;

  typedef struct {
	  long x, y;
  }
lCoord;

/* directions of rotation */
  typedef enum {
	  COUNTERCLOCKWISE = -1, NO_SPIN = 0, CLOCKWISE = 1, TOGGLE = 2
  } Spin;

#endif /* _TANKTYPES_H_ */
