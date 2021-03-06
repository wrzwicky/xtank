/*
** Xtank
**
** Copyright 1988 by Dan Schmidt
**
** artful.c
*/

/*
$Author: lidl $
$Id: artful.c,v 1.1.1.1 1995/02/01 00:25:46 lidl Exp $
*/

#include "xtanklib.h"
#include <math.h>

#define artful_square(x) ((x)*(x))

#define SE 0
#define S  1
#define SW 2
#define W  3
#define NW 4
#define N  5
#define NE 6
#define E  7

static void main();

Prog_desc artful_prog = {
	"artful",
	"artful",
	"Unlike other programs, artful is not a real fighter; it is a fast cycle \
which attempts to avoid walls, bullets, vehicles, and fighting in general. \
It's ultimate speed and manouverability make it  difficult to hit.Its one \
    flaw is that it will never win a battle, only survive it.",
	"Dan Schmidt",
	USES_MESSAGES,
	4,
	main
};


FLOAT artful_tw();

static Location myloc;
static FLOAT dir;
static FLOAT myspeed;

void message(string)
char *string;
{
	send_msg(RECIPIENT_ALL, OP_TEXT, (Byte *)string);
}

static void main()
{
	int i = 512;
	int stuckframes = 0;

	set_rel_drive(9.0);
	while (1)
	{
		get_location(&myloc);
		dir = heading();
		myspeed = speed();
		if (!i--)
		{
			i = 512;
			switch (random() % 51)
			{
				case 0:
					message("Running away");
					break;
				case 1:
					message("Avoiding contact");
					break;
				case 2:
					message("Chickening out");
					break;
				case 3:
					message("Weapon overheat");
					break;
				case 4:
					message("How come they get ammo?");
					break;
				case 5:
					message("Entering increase_kills()");
					break;
				case 6:
					message("Eating hot lead");
					break;
				case 7:
					message("Error in shoot_weapon()");
					break;
				case 8:
					message("Entering increase_armor()");
					break;
				case 9:
					message("Entering make_excuse_for_losing()");
					break;
				case 10:
					message("Pursuing vehicle 2");
					break;
				default:
					break;
			}
		}
		if (myspeed == 0)
			artful_get_unstuck(&stuckframes);
		else
		{
			if (stuckframes > 0)
				--stuckframes;
			else
			{
				if (i & 1)
					artful_dodge_walls();
				else
					artful_dodge_bullets();
			}
		}
	}
}

artful_get_unstuck(stuckframes)
int *stuckframes;
{
	turn_vehicle(dir + PI);
	set_rel_drive(9.0);
	*stuckframes = 3;
}

int artful_dodge_walls()
{
	int dirtype;
    WallSide cturn;
	int x, y;

	x = myloc.grid_x;
	y = myloc.grid_y;

	dirtype = (int) (dir * 4 / PI);
    if (ABS(dirtype * PI / 4) - dir < .02)
		switch (dirtype)
		{
			case S:
				if (wall(EAST, x, y))
					dirtype = SW;
				break;
			case W:
				if (wall(SOUTH, x, y))
					dirtype = NW;
				break;
			case N:
				if (wall(WEST, x, y))
					dirtype = NE;
				break;
			case E:
				if (wall(NORTH, x, y))
					dirtype = SE;
				break;
		}

    cturn = NO_DIR;

	switch (dirtype)
	{
		case SE:
			if (wall(SOUTH, x, y))
			{
				if (wall(EAST, x, y))
				{
					if (wall(NORTH, x, y))
						cturn = WEST;
					else
						cturn = NORTH;
				}
				else
					cturn = EAST;
			}
			else if (wall(EAST, x, y))
				cturn = SOUTH;
			else if (wall(SOUTH, x + 1, y))
				cturn = EAST;
			else if (wall(EAST, y + 1, x))
				cturn = SOUTH;
			break;
		case S:
			if (wall(EAST, x, y))
			{
				if (wall(SOUTH, x, y))
				{
					if (wall(WEST, x, y))
						cturn = NORTH;
					else
						cturn = WEST;
				}
				else
					cturn = SOUTH;
			}
			else if (wall(SOUTH, x, y))
				cturn = EAST;
			else if (wall(EAST, x, y + 1))
				cturn = SOUTH;
			else if (wall(SOUTH, x + 1, y))
				cturn = EAST;
			break;
		case SW:
			if (wall(WEST, x, y))
			{
				if (wall(SOUTH, x, y))
				{
					if (wall(EAST, x, y))
						cturn = NORTH;
					else
						cturn = EAST;
				}
				else
					cturn = SOUTH;
			}
			else if (wall(SOUTH, x, y))
				cturn = WEST;
			else if (wall(WEST, x, y + 1))
				cturn = SOUTH;
			else if (wall(SOUTH, x - 1, y))
				cturn = WEST;
			break;
		case W:
			if (wall(SOUTH, x, y))
			{
				if (wall(WEST, x, y))
				{
					if (wall(NORTH, x, y))
						cturn = EAST;
					else
						cturn = NORTH;
				}
				else
					cturn = WEST;
			}
			else if (wall(WEST, x, y))
				cturn = SOUTH;
			else if (wall(SOUTH, x - 1, y))
				cturn = WEST;
			else if (wall(WEST, x, y + 1))
				cturn = SOUTH;
			break;
		case NW:
			if (wall(NORTH, x, y))
			{
				if (wall(WEST, x, y))
				{
					if (wall(SOUTH, x, y))
						cturn = EAST;
					else
						cturn = SOUTH;
				}
				else
					cturn = WEST;
			}
			else if (wall(WEST, x, y))
				cturn = NORTH;
			else if (wall(NORTH, x - 1, y))
				cturn = WEST;
			else if (wall(WEST, x, y - 1))
				cturn = NORTH;
			break;
		case N:
			if (wall(WEST, x, y))
			{
				if (wall(NORTH, x, y))
				{
					if (wall(EAST, x, y))
						cturn = SOUTH;
					else
						cturn = EAST;
				}
				else
					cturn = NORTH;
			}
			else if (wall(NORTH, x, y))
				cturn = WEST;
			else if (wall(WEST, x, y - 1))
				cturn = NORTH;
			else if (wall(NORTH, x - 1, y))
				cturn = WEST;
			break;
		case NE:
			if (wall(EAST, x, y))
			{
				if (wall(NORTH, x, y))
				{
					if (wall(WEST, x, y))
						cturn = SOUTH;
					else
						cturn = WEST;
				}
				else
					cturn = NORTH;
			}
			else if (wall(NORTH, x, y))
				cturn = EAST;
			else if (wall(EAST, x, y - 1))
				cturn = NORTH;
			else if (wall(NORTH, x + 1, y))
				cturn = EAST;
			break;
		case E:
			if (wall(NORTH, x, y))
			{
				if (wall(EAST, x, y))
				{
					if (wall(SOUTH, x, y))
						cturn = WEST;
					else
						cturn = SOUTH;
				}
				else
					cturn = EAST;
			}
			else if (wall(EAST, x, y))
				cturn = NORTH;
			else if (wall(NORTH, x + 1, y))
				cturn = EAST;
			else if (wall(EAST, y - 1, x))
				cturn = NORTH;
	}

	switch (cturn)
	{
		case NORTH:
			turn_vehicle(3 * PI / 2);
			break;
		case SOUTH:
			turn_vehicle(PI / 2);
			break;
		case WEST:
			turn_vehicle(PI);
			break;
		case EAST:
			turn_vehicle(0.0);
			break;
        case NO_DIR:
			return (0);
	}
	set_rel_drive(6.0);
	return (1);
}

artful_dodge_bullets()
{
	Bullet_info bullet_info[MAX_BULLETS + MAX_VEHICLES];
	Vehicle_info vehicle_info[MAX_VEHICLES];
	int num_bullets, num_veh_alive;
	int i;
	int closestb = 0, closestd = 99999, d;
	int isabullet = 0;

	get_bullets(&num_bullets, bullet_info);
	get_vehicles(&num_veh_alive, vehicle_info);

	for (i = 0; i < num_veh_alive; ++i)
	{
		bullet_info[i + num_bullets].loc = vehicle_info[i].loc;
		bullet_info[i + num_bullets].xspeed = vehicle_info[i].xspeed;
		bullet_info[i + num_bullets].yspeed = vehicle_info[i].yspeed;
	}

	num_bullets += num_veh_alive;

	for (i = 0; i < num_bullets; ++i)
        if ((ABS(bullet_info[i].loc.grid_x - myloc.grid_x) < 3) &&
                (ABS(bullet_info[i].loc.grid_y - myloc.grid_y) < 3))
		{

			bullet_info[i].loc.box_x +=
				(bullet_info[i].loc.grid_x - myloc.grid_x) * BOX_WIDTH;
			bullet_info[i].loc.box_y +=
				(bullet_info[i].loc.grid_y - myloc.grid_y) * BOX_HEIGHT;

			d = artful_square(bullet_info[i].loc.box_x - myloc.box_x) +
				artful_square(bullet_info[i].loc.box_y - myloc.box_y);

			if (i > num_bullets - num_veh_alive)
				d /= 2;

			if (d < closestd)
			{
				closestd = d;
				closestb = i;
			}
		}
	if (closestb < num_bullets)
		isabullet = 1;
	if (closestd < 99999)
		artful_avoid(&bullet_info[closestb], isabullet);
}

artful_avoid(b, isabullet)
Bullet_info *b;
int isabullet;
{
	FLOAT movedir;
	FLOAT theta;
	FLOAT phi;
	int bulx, buly, mex, mey;
	int closeness;
	int oldcloseness;
	int i;
	unsigned int important = 0;
	int myxspeed, myyspeed;
	int behindme = 0;
	int bvectx, bvecty;
	int test;

	bulx = (int) b->loc.box_x;
	buly = (int) b->loc.box_y;
	mex = (int) myloc.box_x;
	mey = (int) myloc.box_y;
	myxspeed = (int) myspeed *cos((double) dir);
	myyspeed = (int) myspeed *sin((double) dir);

	oldcloseness = 99999;

	if (isabullet)
	{
		for (i = 0; i < 10; ++i)
		{
			bulx += (int) b->xspeed;
			buly += (int) b->yspeed;
			mex += myxspeed;
			mey += myyspeed;
			closeness = (bulx - mex) * (bulx - mex) + (buly - mey) * (buly - mey);
			if (oldcloseness < closeness)
			{
				bvectx = mex - bulx;
				bvecty = mey - buly;
				test = myxspeed * bvectx + myyspeed * bvecty;

				if (test > 0)
					behindme = 1;
				break;
			}
			oldcloseness = closeness;
			if (closeness < 4000)
				important = 1;
		}
		if (important)
		{
            theta = (FLOAT) ATAN2(b->yspeed, b->xspeed);
			phi = dir - theta - PI / 2;

            if ((ABS(sin((double) (dir - theta))) > .7) && !behindme)
				movedir = dir + PI;
			else
			{
                if ((cos((double) phi) < 0) &&
		    (artful_ok(artful_tw(theta - PI / 2))))
					movedir = theta - PI / 2;
				else
					movedir = theta + PI / 2;
			}
			set_rel_drive(6.0);
			turn_vehicle(movedir);
		}
	}
	else
	{
        theta = (FLOAT) ATAN2(myloc.box_y - b->loc.box_y,
                              myloc.box_x - b->loc.box_x);
		if (artful_ok(theta - PI / 2))
			movedir = dir - PI / 2;
		else
			movedir = dir + PI / 2;
		set_rel_drive(6.0);
		turn_vehicle(movedir);
	}
}

FLOAT artful_tw(angle)
FLOAT angle;
{
	FLOAT answer = angle;

	while (answer < 0)
		answer += 2 * PI;
	return (answer);
}

artful_ok(angle)
FLOAT angle;
{
	if ((myloc.box_x < 100) && (wall(WEST, myloc.grid_x, myloc.grid_y))
			&& (angle > PI / 2) && (angle < 3 * PI / 2))
		return (0);
	else if ((myloc.box_y < 100) &&
			 (wall(NORTH, myloc.grid_x, myloc.grid_y))
			 && (angle > PI))
		return (0);
	else if ((myloc.box_x > BOX_WIDTH - 100) &&
			 (wall(EAST, myloc.grid_x, myloc.grid_y))
			 && ((angle > 3 * PI / 2) || (angle < PI / 2)))
		return (0);
	else if ((myloc.box_y > BOX_HEIGHT - 100) &&
			 (wall(SOUTH, myloc.grid_x, myloc.grid_y))
			 && (angle < PI))
		return (0);
	else
		return (1);
}
