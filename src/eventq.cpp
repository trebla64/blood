#include "stdafx.h"
#include "game.h"
#include "eventq.h"
#include "error.h"


PriorityQueue::PriorityQueue()
{
}


PriorityQueue::~PriorityQueue()
{
}


void evInit()
{
	for (int i = 0; i < numsectors; i++)
	{
		int nXSector = sector[i].extra;
		if (nXSector >= NUM_ELEMENTS(xsector))
			ThrowError("Invalid xsector reference in sector %d\n", i);

		if (nXSector > 0)
		{
		}
	}

	for (int i = 0; i < numwalls; i++)
	{
	}

	for (int i = 0; i < MAXSPRITES; i++)
	{
		if (sprite[i].statnum >= 1024)
			continue;

		int nXSprite = sprite[i].extra;
		if (nXSprite >= NUM_ELEMENTS(xsprite))
			ThrowError("Invalid xsprite reference in sprite %d", i);

		if (nXSprite > 0)
		{
		}
	}
}
