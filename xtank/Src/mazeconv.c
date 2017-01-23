static int external_type[] =
{
    NORMAL,
    FUEL,
    AMMO,
    ARMOR,
    GOAL,
    OUTPOST,
    SCROLL_N,
    SCROLL_NE,
    SCROLL_E,
    SCROLL_SE,
    SCROLL_S,
    SCROLL_SW,
    SCROLL_W,
    SCROLL_NW,
    SLIP,
    SLOW,
    START_POS,
    NORTH_SYM,
    WEST_SYM,
    NORTH_DEST_SYM,
    WEST_DEST_SYM,
    PEACE,
	-1
};

#define TO_INTERNAL_TYPE       0
#define TO_EXTERNAL_TYPE       1

convert_maze(d, convtype)
	Mdesc *d;
	int convtype;
{
	Byte flags, *dptr;
	int i, j, ctr;

    /* For each box there is a byte that contains 8 flags.  If EMPTY_BOXES
       is set, the remaining 7 bits give the number of empty boxes
       (excluding this one) to make before reading the next byte.
       Otherwise, If TYPE_EXISTS is set, the next byte is the box type.
       Otherwise, the type is 0. If TEAM_EXISTS is set, the next byte is
       the box team.  Otherwise, the team is 0. */
	dptr = d->data;
	while (*dptr)
	{
		flags = *(dptr++);

		/* Check for empty box flag */
		if (! (flags & EMPTY_BOXES))
		{
			if (flags & TYPE_EXISTS)
			{
				if (convtype == TO_INTERNAL_TYPE)
					*dptr = external_type[(LandmarkType) *dptr];
				else
				{
					ctr = 0;
					while (external_type[ctr] != -1)
					{
						if (external_type[ctr] == (int)*dptr)
						{
							*dptr = ctr;
							break;
						}
						ctr++;
					}
					dptr++;
				}

				if (flags & TEAM_EXISTS)
				{
					dptr++;
				}
			}
		}
	}
}
