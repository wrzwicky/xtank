#include "malloc.h"
/*
** Xtank
**
** Copyright 1988 by Terry Donahue
**
** radar.c
*/

/*
$Author: lidl $
$Id: radar.c,v 2.6 1992/01/29 08:37:01 lidl Exp $

$Log: radar.c,v $
 * Revision 2.6  1992/01/29  08:37:01  lidl
 * post aaron patches, seems to mostly work now
 *
 * Revision 2.5  1991/12/02  10:34:41  lidl
 * fixed a small bug in setting the inital radar map position's idea of
 * what the last radar position was (I think)
 *
 * Revision 2.4  1991/09/24  14:07:28  lidl
 * fix from arrone@sail.labs.tek.com, makes fading blips correct
 *
 * Revision 2.3  1991/02/10  13:51:31  rpotter
 * bug fixes, display tweaks, non-restart fixes, header reorg.
 *
 * Revision 2.2  91/01/20  09:58:49  rpotter
 * complete rewrite of vehicle death, other tweaks
 * 
 * Revision 2.1  91/01/17  07:12:47  rpotter
 * lint warnings and a fix to update_vector()
 * 
 * Revision 2.0  91/01/17  02:10:22  rpotter
 * small changes
 * 
 * Revision 1.1  90/12/29  21:03:00  aahz
 * Initial revision
 * 
*/

#include "xtank.h"
#include "graphics.h"
#include "gr.h"
#include "map.h"
#include "vehicle.h"
#include "globals.h"

extern Map real_map;
Boolean traceaction = FALSE;

/* Names for radar blip life numbers */
#define RADAR_born	23
#define RADAR_size5	20
#define RADAR_size4	17
#define RADAR_size3	14
#define RADAR_size2	11
#define RADAR_size1	8
#define RADAR_dead	5

#define draw_number(v,loc) \
  draw_char(loc,(char) ('0' + v->number),v->color)

/*
** Machine dependent values here based on MAP_BOX_SIZE = 9.
*/
static Coord radar_sweeper[24] = {
	{63, -7}, {63, 7}, {58, 24}, {49, 39}, {39, 49}, {24, 58},
	{7, 63}, {-7, 63}, {-24, 58}, {-39, 49}, {-49, 39}, {-58, 24},
	{-63, 7}, {-63, -7}, {-58, -24}, {-49, -39}, {-39, -49}, {-24, -58},
	{-7, -63}, {7, -63}, {24, -58}, {39, -49}, {49, -39}, {58, -24}
};

static Coord radar_swept[24][8] = {
	{{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
	{{3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {5, 2}, {6, 2}, {7, 2}},
	{{2, 1}, {3, 2}, {4, 2}, {4, 3}, {5, 3}, {6, 3}, {6, 4}},
	{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 4}, {4, 5}, {5, 5}},
	{{1, 2}, {2, 3}, {2, 4}, {3, 4}, {3, 5}, {3, 6}, {4, 6}},
	{{1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {2, 5}, {2, 6}, {2, 7}},

	{{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
	{{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}, {-1, 7}, {-2, 5}, {-2, 6}, {-2, 7}},
	{{-1, 2}, {-2, 3}, {-2, 4}, {-3, 4}, {-3, 5}, {-3, 6}, {-4, 6}},
	{{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 4}, {-4, 5}, {-5, 5}},
	{{-2, 1}, {-3, 2}, {-4, 2}, {-4, 3}, {-5, 3}, {-6, 3}, {-6, 4}},
	{{-3, 1}, {-4, 1}, {-5, 1}, {-6, 1}, {-7, 1}, {-5, 2}, {-6, 2}, {-7, 2}},

	{{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
	{{-3, -1}, {-4, -1}, {-5, -1}, {-6, -1}, {-7, -1}, {-5, -2}, {-6, -2}, {-7, -2}},
	{{-2, -1}, {-3, -2}, {-4, -2}, {-4, -3}, {-5, -3}, {-6, -3}, {-6, -4}},
	{{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -4}, {-4, -5}, {-5, -5}},
	{{-1, -2}, {-2, -3}, {-2, -4}, {-3, -4}, {-3, -5}, {-3, -6}, {-4, -6}},
	{{-1, -3}, {-1, -4}, {-1, -5}, {-1, -6}, {-1, -7}, {-2, -5}, {-2, -6}, {-2, -7}},

	{{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
	{{1, -3}, {1, -4}, {1, -5}, {1, -6}, {1, -7}, {2, -5}, {2, -6}, {2, -7}},
	{{1, -2}, {2, -3}, {2, -4}, {3, -4}, {3, -5}, {3, -6}, {4, -6}},
	{{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -4}, {4, -5}, {5, -5}},
	{{2, -1}, {3, -2}, {4, -2}, {4, -3}, {5, -3}, {6, -3}, {6, -4}},
	{{3, -1}, {4, -1}, {5, -1}, {6, -1}, {7, -1}, {5, -2}, {6, -2}, {7, -2}}
};

static int radar_num_swept[24] = {
	7, 8, 7, 7, 7, 8, 7, 8, 7, 7, 7, 8, 7, 8, 7, 7, 7, 8, 7, 8, 7, 7, 7, 8
};


special_radar(v, record, action)
Vehicle *v;
char *record;
unsigned int action;
{
    Radar *r;
    Blip *b;
    Coord *offset;
    unsigned int vehicle_flags;
    int init_x, init_y;
    int x, y;
    int i;

    r = (Radar *) record;

    switch (action)
    {
      case SP_update:
	/* If there are any blips, decrement their lives and remove the
	   dead */
	i = 0;
	while (i < r->num_blips)
	{
	    b = &r->blip[i];
	    b->old_view = b->view;
	    switch (--b->life)
	    {
	      case RADAR_size5:
	      case RADAR_size4:
	      case RADAR_size3:
	      case RADAR_size2:
	      case RADAR_size1:
		b->view++;
		break;
	      case RADAR_dead:
		if (i < r->num_blips - 1)
		    *b = r->blip[r->num_blips - 1];
		r->num_blips--;
		i--;
		break;
	    }
	    i++;
	}

	/* Check the swept boxes for any vehicles */
	init_x = v->loc->grid_x;
	init_y = v->loc->grid_y;
	offset = radar_swept[r->pos];

	for (i = 0; i < radar_num_swept[r->pos]; i++)
	{
	    /* Make sure blip is inside the grid */
	    x = init_x + offset[i].x;
	    if (x < 0 || x >= GRID_WIDTH)
		continue;
	    y = init_y + offset[i].y;
	    if (y < 0 || y >= GRID_HEIGHT)
		continue;

	    vehicle_flags = real_map[x][y].flags & ANY_VEHICLE;

	    /* If there is a vehicle in this box, make a blip */
	    if (vehicle_flags)
	    {
		b = &r->blip[r->num_blips++];
		b->x = grid2map(x) + MAP_BOX_SIZE / 4;
		b->y = grid2map(y) + MAP_BOX_SIZE / 4;
		b->life = RADAR_born;
		b->view = 0;
		b->flags = vehicle_flags;
	    }
	}

	/* Increment the position counter modulo 24 */
	if (++r->pos == 24)
	    r->pos = 0;

	/* Save the old sweeper position, and compute the new one */
	r->old_start_x = r->start_x;
	r->old_start_y = r->start_y;
	r->old_end_x = r->end_x;
	r->old_end_y = r->end_y;
	r->start_x = grid2map(v->loc->grid_x) + MAP_BOX_SIZE / 2;
	r->start_y = grid2map(v->loc->grid_y) + MAP_BOX_SIZE / 2;
	r->end_x = r->start_x + radar_sweeper[r->pos].x;
	r->end_y = r->start_y + radar_sweeper[r->pos].y;
	break;
      case SP_redisplay:
	/* redraw the sweeper line */
	draw_line(MAP_WIN, r->old_start_x, r->old_start_y,
		  r->old_end_x, r->old_end_y, DRAW_XOR, CUR_COLOR);
	draw_line(MAP_WIN, r->start_x, r->start_y,
		  r->end_x, r->end_y, DRAW_XOR, CUR_COLOR);

	/* redraw the blips that have changed size */
	for (i = 0; i < r->num_blips; i++)
	{
	    b = &r->blip[i];

	    /* Erase old view */
	    if (b->life < RADAR_born)
		draw_filled_square(MAP_WIN, b->x, b->y, 6 - b->old_view,
				   DRAW_XOR, CUR_COLOR);

	    /* Draw new view */
	    if (b->life > RADAR_dead + 1)
		draw_filled_square(MAP_WIN, b->x, b->y, 6 - b->view,
				   DRAW_XOR, CUR_COLOR);
	}
	break;
      case SP_draw:
      case SP_erase:
	/* draw/erase the sweeper line */
	draw_line(MAP_WIN, r->start_x, r->start_y,
		  r->end_x, r->end_y, DRAW_XOR, CUR_COLOR);

	/* draw/erase the blips */
	for (i = 0; i < r->num_blips; i++)
	{
	    b = &r->blip[i];
#ifdef NO_BROKEN_BLIP_FIX
	    if (b->view < 5)
#else /* NO_BROKEN_BLIP_FIX */
	    if (b->view < 6 && b->life != RADAR_dead + 1) 
#endif /* NO_BROKEN_BLIP_FIX */
		draw_filled_square(MAP_WIN, b->x, b->y, 6 - b->view,
				   DRAW_XOR, CUR_COLOR);
	}
	break;
      case SP_activate:
#ifndef NO_NEW_RADAR
	if (v->special[(SpecialType) NEW_RADAR].status == SP_on)
	    do_special(v, (SpecialType) NEW_RADAR, SP_off);
#endif /* !NO_NEW_RADAR */
	/* clear the blips left over from before */
	r->num_blips = 0;

	/* compute the starting location of the sweeper */
	r->start_x = grid2map(v->loc->grid_x) + (int) MAP_BOX_SIZE / 2;
	r->start_y = grid2map(v->loc->grid_y) + (int) MAP_BOX_SIZE / 2;
	r->end_x = r->start_x + radar_sweeper[r->pos].x;
	r->end_y = r->start_y + radar_sweeper[r->pos].y;
	/* make the old radar position the same as the newly computed one */
	r->old_start_x = r->start_x;
	r->old_start_y = r->start_y;
	r->old_end_x = r->end_x;
	r->old_end_y = r->end_y;
	break;
      case SP_deactivate:
	break;
    }
}

/*
** Puts numbers of vehicles on the map, and moves them around.
*/
full_radar(status)
unsigned int status;
{
    Vehicle *v;
    int i;

    if (status == ON)
    {
	for (i = 0; i < num_veh_alive; i++) {
	    v = live_vehicles[i];
	    draw_number(v, v->loc);
	}
    }
    else if (status == REDISPLAY)
    {
	for (i = 0; i < num_veh_alive; i++) {
	    v = live_vehicles[i];

	    /* Only draw the new number location if he is alive */
	    if (tstflag(v->status, VS_is_alive)) {
		if (!(grid_equal(v->loc, v->old_loc)))
		{
		    draw_number(v, v->old_loc);
		    draw_number(v, v->loc);
		}
	    }
	    else
		draw_number(v, v->old_loc);
	}
    }
}

/*
** Xors the character c on the full map at location loc.
*/
draw_char(loc, c, color)
Loc *loc;
char c;
int color;
{
	int x, y;
	char buf[2];

	x = grid2map(loc->grid_x) + MAP_BOX_SIZE / 2;
	y = grid2map(loc->grid_y) + MAP_BOX_SIZE / 2 - 4;
	buf[0] = c;
	buf[1] = '\0';
	draw_text(MAP_WIN, x, y, buf, S_FONT, DRAW_XOR, color);
}

#ifndef NO_NEW_RADAR

#define TAC_UPDATE_INTERVAL 1 /* 2 is normal */
#define RAD_UPDATE_INTERVAL 1 /* 6 is normal */

/*
 * The idea of the blip is somewhat perverted in that
 * a blip can be invisible, which means that it is out
 * of range. A lowlib function provides for copying
 * only visible blips into a robot readable blip
 * array.
 */

#define BLIP_SIZE 5 /* 7, 5, 3, 1 */
#define shft ((7 - BLIP_SIZE)/2)

#define THRESHOLD 0.00001

extern int frame;

/*
 * This is a combination of draw_number and draw_char
 * that reads out of the blip info instead of calculating from
 * the absolute vehicle position
 */

nr_draw_number(v, c)
Vehicle *v;
Coord *c;
{
	int x, y;
	char buf[2];

	buf[0] = '0' + v->number;
	buf[1] = '\0';
	draw_text(MAP_WIN, c->x, c->y, buf, S_FONT, DRAW_XOR, v->color);
}

/*
 * note that if a draw follows an activate, there must not have been an 
 * intervening redisplay, though an update is OK.
 */

special_new_radar(v, record, action)
Vehicle *v;
char *record;
unsigned int action;
{
    int veh, veh2, x, y;
    newRadar *r;
    newBlip *b;
    Vehicle *bv, *tv;
    float dx, dy, dist_2;
 
    r = (newRadar *) record;

    switch (action) {
	case SP_redisplay:
	    if (traceaction) if (v->number == 0) printf("snr: %i redisplay ", frame);
	    if (r->need_redisplay) {
		nr_t_redisplay(r);
		r->need_redisplay = FALSE;
		if (traceaction) if (v->number == 0) printf("-- redisplayed");
	    }
	    if (traceaction) if (v->number == 0) printf("\n");
	    break;
	case SP_update:
	    if (traceaction) if (v->number == 0) printf("snr: %i update    ", frame);
	    if (frame - r->rad_frame_updated >= RAD_UPDATE_INTERVAL) {
		r->rad_frame_updated = frame;
		for (veh = 0; veh < MAX_VEHICLES; veh++) {
		    b = &r->blip[veh];
		    bv = &actual_vehicles[veh];
		    b->draw_radar = FALSE;
		    b->draw_tactical = FALSE;
		    if ( (bv == v) || !tstflag(bv->status, VS_is_alive) )
			continue;
		    b->draw_friend = v->have_IFF_key[bv->number] || (bv->team == v->team);
		    dx = v->loc->x - bv->loc->x;
		    dy = v->loc->y - bv->loc->y;
		    dist_2 = SQR(dx) + SQR(dy); /* !!! Change this to the new fast routine !!! */
		    if ((bv->rcs / dist_2) > THRESHOLD)
			b->draw_radar = TRUE;
		    if (b->draw_radar) {
			b->draw_loc.x = grid2map(bv->loc->grid_x) + shft + MAP_BOX_SIZE / 4;
			b->draw_loc.y = grid2map(bv->loc->grid_y) + shft + MAP_BOX_SIZE / 4;
			b->draw_grid.x = bv->loc->grid_x;
			b->draw_grid.y = bv->loc->grid_y;
		    }
		} 
		r->need_redisplay = TRUE;
		if (traceaction) if (v->number == 0) printf("-- passed");
	    }
	    if (traceaction) if (v->number == 0) printf("\n");
	    break;
	case SP_draw:
  	    if (traceaction) if (v->number == 0) printf("snr: %i draw\n", frame);
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		bv = &actual_vehicles[veh];
		if (b->drawn_radar)
		    if (b->drawn_friend)
			nr_draw_number(bv, b->drawn_loc);
		    else
			draw_filled_square(MAP_WIN, b->drawn_loc.x, b->drawn_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
	    }
	    break;
	case SP_erase:
  	    if (traceaction) if (v->number == 0) printf("snr: %i erase\n", frame);
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		bv = &actual_vehicles[veh];
		if (b->drawn_radar)
		    if (b->drawn_friend)
			nr_draw_number(bv, b->drawn_loc);
		    else {
			draw_filled_square(MAP_WIN, b->drawn_loc.x, b->drawn_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
			r->map[b->drawn_grid.x][b->drawn_grid.y] = NULL;
		    }
		    b->drawn_radar = FALSE;
	    }
	    break;
	case SP_activate:
  	    if (traceaction) if (v->number == 0) printf("snr: %i activate\n", frame);
	    if (v->special[(SpecialType) RADAR].status == SP_on)
		do_special(v, (SpecialType) RADAR, SP_off);
	    r->need_redisplay = TRUE;
	    r->rad_frame_updated = frame - RAD_UPDATE_INTERVAL;
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		b->drawn_radar = FALSE;
		b->draw_radar = FALSE;
		if (v->special[(SpecialType) TACLINK].status != SP_on) {
		    b->draw_tactical = FALSE;
		    b->drawn_tactical = FALSE;
		}
	    }
	    if (v->special[(SpecialType) TACLINK].status != SP_on)
		for (x = 0; x < GRID_WIDTH; x++)
		    for (y = 0; y < GRID_WIDTH; y++)
			r->map[x][y] = NULL;
	    else if (v->special[(SpecialType) TACLINK].status == SP_on)
		for (x = 0; x < GRID_WIDTH; x++)
		    for (y = 0; y < GRID_WIDTH; y++)
                        if ( r->map[x][y] && !(r->map[x][y])->drawn_radar)
			    r->map[x][y] = NULL;
	    break;
	case SP_deactivate:
  	    if (traceaction) if (v->number == 0) printf("snr: %i deactivate\n", frame);
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		b->draw_radar = FALSE;
	    }
	    break;
	default:
	    break;
    }
}

special_taclink(v, record, action)
Vehicle *v;
char *record;
unsigned int action;
{
    int veh, veh2, x, y;
    newRadar *r;
    newBlip *b;
    Vehicle *bv, *tv;
    float dx, dy, dist_2;

    r = (newRadar *) record;

    switch (action) {
	case SP_redisplay:
	    if (traceaction) if (v->number == 0) printf(" st: %i redisplay  ", frame);
	    if (r->need_redisplay) {
		nr_t_redisplay(r);
		r->need_redisplay = FALSE;
		if (traceaction) if (v->number == 0) printf("-- redisplayed");
	    }
	    if (traceaction) if (v->number == 0) printf("\n");
	    break;
	case SP_update:
	    if (traceaction) if (v->number == 0) printf(" st: %i update    ", frame);
	    if (frame - r->tac_frame_updated >= TAC_UPDATE_INTERVAL) {
		r->tac_frame_updated = frame;
		for (veh = 0; veh < MAX_VEHICLES; veh++) {
		    b = &r->blip[veh];
		    bv = &actual_vehicles[veh];
		    b->draw_tactical = FALSE;
		    if ( (bv == v) || !tstflag(bv->status, VS_is_alive) )
			continue;
		    b->draw_friend = v->have_IFF_key[bv->number] || (bv->team == v->team);
		    if ( b->draw_friend && bv->special[(SpecialType) TACLINK].status == SP_on)
			b->draw_tactical = TRUE;
		    else {
			for (veh2 = 0; (veh2 < MAX_VEHICLES) && !b->draw_tactical; veh2++) {
			    tv = &actual_vehicles[veh2];
			    b->draw_tactical = (tstflag(tv->status, VS_is_alive) &&
		                      (v->have_IFF_key[tv->number] || (tv->team == v->team)) &&
				      tv != v &&
				      tv->special[(SpecialType) TACLINK].status == SP_on &&
				      ((newRadar *)(tv->special[(SpecialType) NEW_RADAR].record))->blip[veh].draw_radar);
			}
		    }
		    if (b->draw_tactical) {
		    /*
		     * clear the draw_radar flag if we have tactical data on it and it's moved
		     * from the radar position (ie, we can have more up-to-date tac info than
		     * our radar is displaying)
		     *
		     * code depends on updates all happening before any displays, as this modifies
		     * a blip that is as of yet undrawn
		     *
		     * note that we have already updated friend even if it is a radar blip
		     *
		     * kindof silly only updating the draw_ stuff if it is different as it could
		     * be moved out of the tests cause if it's the same it won't matter anyway!
		     * 
		     */
			if (b->draw_radar) {
			    if (b->draw_grid.x != bv->loc->grid_x || b->draw_grid.y != bv->loc->grid_y) {
				b->draw_radar = FALSE;
				b->draw_loc.x = grid2map(bv->loc->grid_x) + shft + MAP_BOX_SIZE / 4;
				b->draw_loc.y = grid2map(bv->loc->grid_y) + shft + MAP_BOX_SIZE / 4;
				b->draw_grid.x = bv->loc->grid_x;
				b->draw_grid.y = bv->loc->grid_y;
			    } else {
				; /* cut me some slack, I'm thinking about it! */
			    }
			} else {
			    b->draw_loc.x = grid2map(bv->loc->grid_x) + shft + MAP_BOX_SIZE / 4;
			    b->draw_loc.y = grid2map(bv->loc->grid_y) + shft + MAP_BOX_SIZE / 4;
			    b->draw_grid.x = bv->loc->grid_x;
			    b->draw_grid.y = bv->loc->grid_y;
			}
		    }
		} 
		r->need_redisplay = TRUE;
		if (traceaction) if (v->number == 0) printf("-- passed");
	    }
	    if (traceaction) if (v->number == 0) printf("\n");
	    break;
	case SP_draw:
  	    if (traceaction) if (v->number == 0) printf(" st: %i draw\n", frame);
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		bv = &actual_vehicles[veh];
		if (b->drawn_tactical)
		    if (b->drawn_friend)
			    nr_draw_number(bv, b->drawn_loc);
		    else
			draw_square(MAP_WIN, b->drawn_loc.x, b->drawn_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
	    }
	    break;
	case SP_erase:
  	    if (traceaction) if (v->number == 0) printf(" st: %i erase\n", frame);
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		bv = &actual_vehicles[veh];
		if (b->drawn_tactical)
		    if (b->drawn_friend)
			nr_draw_number(bv, b->drawn_loc);
		    else {
			draw_square(MAP_WIN, b->drawn_loc.x, b->drawn_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
			r->map[b->drawn_grid.x][b->drawn_grid.y] = NULL;
		    }
		    b->drawn_tactical = FALSE;
	    }
	    break;
	case SP_activate:
  	    if (traceaction) if (v->number == 0) printf(" st: %i activate\n", frame);
	    r->need_redisplay = TRUE;
	    r->tac_frame_updated = frame - TAC_UPDATE_INTERVAL;
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		b->drawn_tactical = FALSE;
		b->draw_tactical = FALSE;
		if (v->special[(SpecialType) NEW_RADAR].status != SP_on) {
		    b->draw_radar = FALSE;
		    b->drawn_radar = FALSE;
		}
	    }
	    if (v->special[(SpecialType) NEW_RADAR].status != SP_on)
		for (x = 0; x < GRID_WIDTH; x++)
		    for (y = 0; y < GRID_WIDTH; y++)
			r->map[x][y] = NULL;
	    else if (v->special[(SpecialType) NEW_RADAR].status == SP_on)
		for (x = 0; x < GRID_WIDTH; x++)
		    for (y = 0; y < GRID_WIDTH; y++)
                        if ( r->map[x][y] && !(r->map[x][y])->drawn_radar)
			    r->map[x][y] = NULL;
	    break;
	case SP_deactivate:
  	    if (traceaction) if (v->number == 0) printf(" st: %i deactivate\n", frame);
	    for (veh = 0; veh < MAX_VEHICLES; veh++) {
		b = &r->blip[veh];
		b->draw_tactical = FALSE;
	    }
	    break;
	default:
	    break;
    }
}

nr_t_redisplay(r)
newRadar *r;
{
    int veh;
    newBlip *b;
    Vehicle *bv;

/*
 * erases anything drawn not scheduled to be drawn OR that is different
 *
 * note that draw_tactical can != drawn_tactical even though nothing has
 * changed if it is a radar blip too, as radar draws preempt tactical
 * draws
 */

    for (veh = 0; veh < MAX_VEHICLES; veh++) {
	b = &r->blip[veh];
	bv = &actual_vehicles[veh];
	if ( b->draw_loc.x != b->drawn_loc.x
	  || b->draw_loc.y != b->drawn_loc.y
	  || b->draw_friend != b->drawn_friend
	  || b->draw_radar != b->drawn_radar
	  || ((b->draw_tactical != b->drawn_tactical) && !b->draw_radar) ) 
	    if (b->drawn_radar) {
		if (b->drawn_friend)
		    nr_draw_number(bv, b->drawn_loc);
		else {
		    draw_filled_square(MAP_WIN, b->drawn_loc.x, b->drawn_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
		    r->map[b->drawn_grid.x][b->drawn_grid.y] = NULL;
		}
		b->drawn_radar = FALSE;
	    } else if (b->drawn_tactical) {
		if (b->drawn_friend)
		    nr_draw_number(bv, b->drawn_loc);
		else {
		    draw_square(MAP_WIN, b->drawn_loc.x, b->drawn_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
		    r->map[b->drawn_grid.x][b->drawn_grid.y] = NULL;
		}
		b->drawn_tactical = FALSE;
	    }
    }

/*
 *    draws everything supposed to be drawn that's not drawn
 */

    for (veh = 0; veh < MAX_VEHICLES; veh++) {
	b = &r->blip[veh];
	bv = &actual_vehicles[veh];
	if (b->draw_radar && !b->drawn_radar) {
	    if (b->draw_friend) {
		nr_draw_number(bv, b->draw_loc);
		b->drawn_radar = TRUE;
	    } else {
		if (r->map[b->draw_grid.x][b->draw_grid.y] == NULL) {
		    draw_filled_square(MAP_WIN, b->draw_loc.x, b->draw_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
		    r->map[b->draw_grid.x][b->draw_grid.y] = b;
		    b->drawn_radar = TRUE;
		} else if ( (r->map[b->draw_grid.x][b->draw_grid.y])->drawn_tactical ) {
		    (r->map[b->draw_grid.x][b->draw_grid.y])->drawn_tactical = FALSE;
		    draw_square(MAP_WIN, (r->map[b->draw_grid.x][b->draw_grid.y])->draw_loc.x, 
		     (r->map[b->draw_grid.x][b->draw_grid.y])->draw_loc.y, BLIP_SIZE, DRAW_XOR,
		     CUR_COLOR);
		    draw_filled_square(MAP_WIN, b->draw_loc.x, b->draw_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
		    r->map[b->draw_grid.x][b->draw_grid.y] = b;
		    b->drawn_radar = TRUE;
		}
	    }
	    if (b->drawn_radar) {
		b->drawn_loc = b->draw_loc;
		b->drawn_friend = b->draw_friend;
		b->drawn_grid = b->draw_grid;
	    }
	}
    }


    for (veh = 0; veh < MAX_VEHICLES; veh++) {
	b = &r->blip[veh];
	bv = &actual_vehicles[veh];
	if (b->draw_tactical && !b->drawn_tactical && !b->drawn_radar && !b->draw_radar) {
	    if (b->draw_friend) {
		    nr_draw_number(bv, b->draw_loc);
		    b->drawn_tactical = TRUE;
	    } else {
		if (r->map[b->draw_grid.x][b->draw_grid.y] == NULL) {
		    draw_square(MAP_WIN, b->draw_loc.x, b->draw_loc.y, BLIP_SIZE, DRAW_XOR, CUR_COLOR);
		    r->map[b->draw_grid.x][b->draw_grid.y] = b;
		    b->drawn_tactical = TRUE;
	        }
	    }
	    if (b->drawn_tactical) {
		b->drawn_loc = b->draw_loc;
		b->drawn_friend = b->draw_friend;
		b->drawn_grid = b->draw_grid;
	    }
	}
    }


}



#endif /* !NO_NEW_RADAR */

