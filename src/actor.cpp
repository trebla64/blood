#include "stdafx.h"
#include "game.h"
//#include "db.h"
#include "engine_access.h"
#include "ai.h"
#include "debug.h"
#include "error.h"


struct POSTPONE
{
	int16 nSprite;
	int16 nStatus;
};


static int      gPostCount = 0;
static POSTPONE gPost[MAXSPRITES];


void actProcessSprites(void)
{
	aiProcessDudes();
}


SPRITE *actSpawnSprite(int nSector, int x, int y, int z, int nStatus, bool bAddXSprite)
{
	dprintf("Spawning sprite in sector %d at (%d, %d, %d)\n", nSector, x, y, z);
	int nSprite = InsertSprite(nSector, nStatus);
	if (nSprite >= 0)
		sprite[nSprite].extra = -1;
	else
	{
		nSprite = headspritestat[9];
		dassert(nSprite >= 0);
		dassert(nSector >= 0 && nSector < MAXSECTORS);
		ChangeSpriteSect(nSprite, nSector);
		// TODO: Call actPostSprite()
	}

	SPRITE *pSprite = &sprite[nSprite];
	dprintf("    Returning sprite ptr %p at index %d\n", pSprite, nSprite);
	setsprite(nSprite, x, y, z);
	pSprite->type = 0;
	if (bAddXSprite && pSprite->extra == -1)
	{
		// TODO: More stuff here
	}

	dassert(pSprite >= &sprite[0] && pSprite < &sprite[MAXSPRITES]);
	return pSprite;
}


void actInit()
{
	for (int i = headspritestat[11]; i >= 0; i = nextspritestat[i])
	{
		SPRITE *pSprite = &sprite[i];
		XSPRITE *pXSprite = NULL;
		if (pSprite->extra > 0 && pSprite->extra < NUM_ELEMENTS(xsprite))
			pXSprite = &xsprite[pSprite->extra];
		if (pSprite->type == 459)
		{
			// TODO: Some more messing with XSPRITE members

			// Make invisible
			pSprite->cstat |= 0x8000;
			// TODO: Some more cstat stuff here
		}
	}

	for (int i = headspritestat[4]; i >= 0; i = nextspritestat[i])
	{
		SPRITE *pSprite = &sprite[i];
		if (pSprite->extra <= 0 || pSprite->extra >= NUM_ELEMENTS(xsprite))
			ThrowError("WARNING: Sprite %d is on the wrong status list!\n", i);

		// How do we know this is a thing?  The stat number?
		int nThingType = pSprite->type - kThingBase;
		pSprite->clipdist = thingInfo[nThingType].clipdist;
		pSprite->flags    = thingInfo[nThingType].flags;

		// Not moving
		xvel[i] = 0;
		yvel[i] = 0;
		zvel[i] = 0;
	}

	// Monsters active?
	if (gGameOptions.nMonsterSettings)
	{
		for (int i = headspritestat[6]; i >= 0; i = nextspritestat[i])
		{
		}
		aiInit();
	}
	else
	{
		// No monsters
		while (headspritestat[6] >= 0)
		{
			// Delete the sprite
			DeleteSprite(headspritestat[6]);
		}
	}
}


void actPostSprite(int nSprite, int nStatus)
{
	dassert(gPostCount < MAXSPRITES);
	dassert(nSprite < MAXSPRITES && sprite[nSprite].statnum < MAXSTATUS);
	dassert(nStatus >= 0 && nStatus <= 1024);	// 1024 = kStatFree

	int i;
	if (sprite[nSprite].flags & 0x20)
	{
		for (i = 0; i < gPostCount && gPost[i].nSprite != nSprite; i++);
		dassert(i < gPostCount);
	}
	else
	{
		i = gPostCount;
		sprite[nSprite].flags |= 0x20;
		gPostCount++;
	}

	gPost[i].nSprite = nSprite;
	gPost[i].nStatus = nStatus;
}


void actPostProcess( void )
{
	for (int i = 0; i < gPostCount; i++)
	{
		POSTPONE *pPost = &gPost[i];
		int nSprite = pPost->nSprite;
		int nStatus = pPost->nStatus;

		sprite[nSprite].flags &= 0xDF;
		if (nStatus == 1024)	// TODO: 1024 = kStatFree
		{
			// TODO: Add call to evKill
			// TODO: Add call to seqKill
			DeleteSprite(nSprite);
		}
		else
		{
			ChangeSpriteStat(nSprite, nStatus);
		}
	}
	gPostCount = 0;
}
