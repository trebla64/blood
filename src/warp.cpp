#include "stdafx.h"
#include "game.h"
#include "engine_access.h"
#include "debug.h"


void warpInit()
{
	for (int i = 0; i < MAXSECTORS; i++)
	{
		gUpperLink[i] = -1;
		gLowerLink[i] = -1;
	}

	for (int i = 0; i < MAXSPRITES; i++)
	{
		if (sprite[i].statnum >= 1024)
			continue;

		SPRITE *pSprite = &sprite[i];
		if (pSprite->extra > 0)
		{
			XSPRITE *pXSprite = &xsprite[pSprite->extra];
			switch (pSprite->type)
			{
				// TODO: No magic
				// Player start
				case 1:
					if (gGameOptions.nGameType == 0 || gGameOptions.nGameType == 2)
					{
						int nIndex = pXSprite->data1;
//						dprintf("Found spawn flag, nIndex = %d\n", nIndex);
						if (nIndex >= 0 && nIndex < 8)
						{
							ZONE *pZone = &gStartZone[nIndex];
							pZone->x      = pSprite->x;
							pZone->y      = pSprite->y;
							pZone->z      = pSprite->z;
							pZone->sector = pSprite->sectnum;
							pZone->ang    = pSprite->ang;
						}
					}
					DeleteSprite(i);	// Delete the flag
					break;

				// Bloodbath start
				case 2:
					if (gGameOptions.nGameType == 1)
					{
						int nIndex = pXSprite->data1;
						if (nIndex >= 0 && nIndex < 8)
						{
							ZONE *pZone = &gStartZone[nIndex];
							pZone->x      = pSprite->x;
							pZone->y      = pSprite->y;
							pZone->z      = pSprite->z;
							pZone->sector = pSprite->sectnum;
							pZone->ang    = pSprite->ang;
						}
					}
					DeleteSprite(i);	// Delete the flag
					break;

				case 7:
					gUpperLink[pSprite->sectnum] = i;
					// TODO: Rewrite cstat magic
					pSprite->cstat |= 0x8000;	// Make invisible
					pSprite->cstat &= 0xFEFE;	// Clear blocking bits
					break;

				case 6:
					gLowerLink[pSprite->sectnum] = i;
					// TODO: Rewrite cstat magic
					pSprite->cstat |= 0x8000;
					pSprite->cstat &= 0xFEFE;
					break;

				case 9:
				case 11:	// Upper stack
				case 13:
					gUpperLink[pSprite->sectnum] = i;
					// TODO: Rewrite cstat magic
					pSprite->cstat |= 0x8000;
					pSprite->cstat &= 0xFEFE;
					pSprite->z = getflorzofslope(pSprite->sectnum, pSprite->x, pSprite->y);
					break;

				case 10:
				case 12:	// Lower stack
				case 14:
					gLowerLink[pSprite->sectnum] = i;
					// TODO: Rewrite cstat magic
					pSprite->cstat |= 0x8000;
					pSprite->cstat &= 0xFEFE;
					pSprite->z = getceilzofslope(pSprite->sectnum, pSprite->x, pSprite->y);
					break;
			}
		}
	}

	for (int nUpperIndex = 0; nUpperIndex < MAXSECTORS; nUpperIndex++)
	{
		int nUpperLink = gUpperLink[nUpperIndex];
		if (nUpperLink < 0)
			continue;

		SPRITE *pUpperSprite = &sprite[nUpperLink];
		int nXSprite = sprite[nUpperLink].extra;
		dassert(nXSprite > 0 && nXSprite < NUM_ELEMENTS(xsprite));
		int nIndex = xsprite[nXSprite].data1;
		for (int nLowerIndex = 0; nLowerIndex < MAXSECTORS; nLowerIndex++)
		{
			int nLowerLink = gLowerLink[nLowerIndex];
			if (nLowerLink < 0)
				continue;

			SPRITE *pLowerSprite = &sprite[nLowerLink];
			int nLowerXSprite = pLowerSprite->extra;
			dassert(nLowerXSprite > 0 && nLowerXSprite < NUM_ELEMENTS(xsprite));
			if (xsprite[nLowerXSprite].data1 == nIndex)
			{
				dprintf("*** Linking lower %d with upper %d\n", nLowerIndex, nUpperIndex);
				pUpperSprite->owner = gLowerLink[nLowerIndex];
				pLowerSprite->owner = gUpperLink[nUpperIndex];
			}
		}
	}

	// Temp hack so we don't start in the tomb on E1M1
	const ZONE *pSrc = &gStartZone[1];
	ZONE *pDst = &gStartZone[0];
	memcpy(pDst, pSrc, sizeof(ZONE));
}
