/*
** Xtank
**
** Copyright 1988 by Terry Donahue
**
** 3d.c
*/

/*
$Author: lidl $
$Id: three-d.c,v 1.1.1.1 1995/02/01 00:25:49 lidl Exp $
*/

#include "malloc.h"
#include "sysdep.h"
#include "terminal.h"
#include "proto.h"

/*
** Displays the walls, bullets and vehicles in the animation window.
*/
/*ARGSUSED*/
display_anim_3d(status)
unsigned int status;
{
}


/*
** Initializes the 3d aspect ratio and view distance of the specified terminal.
*/
/*ARGSUSED*/
init_terminal_3d(t)
Terminal *t;
{
}

/*
** Transforms a (dx,dy) from screen coordinates to maze coordinates.
*/
/*ARGSUSED*/
transform_3d(dx, dy)
int *dx, *dy;
{
}

/*
** Toggles 3d, wide view, distance view, extend walls, and clipping.
*/
/*ARGSUSED*/
toggle_3d(mask)
int mask;
{
}
