#include "stdafx.h"
#include "game.h"
#include "view.h"
#include "debug.h"
#include "error.h"


#define MAX_MIRRORS 16


struct MIRROR
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
};


static int mirrorsector;
static int mirrorcnt;
static int mirrorwall[8192];	// Count
static MIRROR mirror[MAX_MIRRORS];


void InitMirrors()
{
	mirrorcnt = 0;
	tilesizx[504] = 0;
	tilesizy[504] = 0;

	for (int i = 0; i < 16; i++)
	{
		tilesizx[4079 + i - 1] = 0;
		tilesizy[4079 + i - 1] = 0;
	}

	// Handle wall mirrors
	for (int i = (numwalls - 1); i >= 0; i--)
	{
		if (mirrorcnt == 16)
		{
			dprintf("Maximum mirror count reached.\n");
			break;
		}

		WALL *pWall = &wall[i];
		if (pWall->overpicnum == 504)
		{
			if (pWall->extra > 0 && pWall->lotag == 516)
			{
				dprintf("Initializing wall overpicnum mirror for wall %d\n", i);
				pWall->cstat |= 0x20;
				pWall->overpicnum = mirrorcnt + 4080;
			}
		}
	}

	// Handle sector mirrors
	for (int i = (numsectors - 1); i >= 0; i--)
	{
		if (mirrorcnt >= 15)
		{
			dprintf("Maximum mirror count reached.\n");
			break;
		}

		SECTOR *pSector = &sector[i];

		if (pSector->floorpicnum == 504)
		{
			int nUpperLink = gUpperLink[i];
			if (nUpperLink >= 0)
			{
				dprintf("Got sector %d with floorpicnum == 504 and upper link %d\n", i, nUpperLink);
				SPRITE *pUpper = &sprite[nUpperLink];
				int nOwner = pUpper->owner & 0xFFF;
				SPRITE *pOwner = &sprite[nOwner];
				int nSector = pOwner->sectnum;
				if (sector[nSector].ceilingpicnum != 504)
					ThrowError("Lower link sector %d doesn't have mirror picnum!");
				dprintf("Initializing floor mirror for sector %d\n", i);
				MIRROR *pMirror = &mirror[mirrorcnt];
				pMirror->field_0  = 2;
				pMirror->field_4  = nSector;
				pMirror->field_8  = pOwner->x - pUpper->x;
				pMirror->field_C  = pOwner->x - pUpper->x;
				pMirror->field_10 = pOwner->z - pUpper->z;
				sector[i].floorpicnum = mirrorcnt + 4080;
				mirrorcnt++;

				dprintf("Initializing ceiling mirror for sector %d\n", nSector);
				pMirror = &mirror[mirrorcnt];
				pMirror->field_0  = 1;
				pMirror->field_4  = i;
				pMirror->field_8  = pUpper->x - pOwner->x;
				pMirror->field_C  = pUpper->y - pOwner->y;
				pMirror->field_10 = pUpper->z - pOwner->z;
				sector[nSector].ceilingpicnum = mirrorcnt + 4080;
				mirrorcnt++;
			}
		}
	}

	mirrorsector = numsectors;
	for (int i = 0; i < 4; i++)
	{
		int nNewWall = numwalls + i;
		mirrorwall[i] = nNewWall;
		WALL *pMirror = &wall[nNewWall];
		pMirror->picnum     = 504;
		pMirror->overpicnum = 504;
		pMirror->cstat      = 0;
		pMirror->nextsector = -1;
		pMirror->nextwall   = -1;
		pMirror->point2     = numwalls;
	}

	wall[mirrorwall[3]].point2 = mirrorwall[0];
	sector[mirrorsector].ceilingpicnum = 504;
	sector[mirrorsector].floorpicnum   = 504;
	sector[mirrorsector].wallnum       = 4;
	sector[mirrorsector].wallptr       = mirrorwall[0];

	dprintf("%d mirrors initialized:\n", mirrorcnt);
	for (int i = 0; i < mirrorcnt; i++)
	{
		const MIRROR *pMirror = &mirror[i];
		dprintf(
			"Mirror %d:\n"
			"    field_0 : %d\n"
			"    field_4 : %d\n"
			"    field_8 : %d\n"
			"    field_C : %d\n"
			"    field_10: %d\n"
			, i
			, pMirror->field_0
			, pMirror->field_4
			, pMirror->field_8
			, pMirror->field_C
			, pMirror->field_10
		);
	}
}


void DrawMirrors(int posx, int posy, int posz, int ang, int horiz)
{
	for (int i = (mirrorcnt - 1); i >= 0; i--)
	{
		const MIRROR *pMirror = &mirror[i];

		uint8 *gp = (uint8 *)&gotpic[(i + 4080) >> 3];
		if (*gp & (1 << ((i - 16) & 7)))
		{
			*gp &= ~(1 << ((i + 4080) & 7));

			switch (pMirror->field_0)
			{
				case 0:
					// TODO: Implement
					break;

				case 1:
					drawrooms(posx + pMirror->field_8, posy + pMirror->field_C, posz + pMirror->field_10, ang, horiz, pMirror->field_4 + MAXSECTORS);
					viewProcessSprites(posx + pMirror->field_8, posy + pMirror->field_C, posz + pMirror->field_10);
					drawmasks();
					break;

				case 2:
					drawrooms(posx + pMirror->field_8, posy + pMirror->field_C, posz + pMirror->field_10, ang, horiz, pMirror->field_4 + MAXSECTORS);
					viewProcessSprites(posx + pMirror->field_8, posy + pMirror->field_C, posz + pMirror->field_10);
					drawmasks();
					break;
			}
			return;
		}
	}
}
