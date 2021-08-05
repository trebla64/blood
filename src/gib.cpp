#include "stdafx.h"
#include "game.h"
#include "debug.h"


#define kGibMax 31


class CGibPosition
{
};


class CGibVelocity
{
};


struct GIBFX
{
	uint8 unknown0[5];
	int   chance;
	uint8 unknown1[12];
};


struct GIBLIST
{
	const GIBFX *pFX;
	int          nNumFX;
	int          field_8;
	int          field_C;
	int          field_10;
};


static const GIBFX gibFxGlassT[] =
{
	{ { 0x12, 0, 0, 0, 0 }, 65536, { 3, 0, 0, 0, 0xC8, 0, 0, 0, 0x90, 1, 0, 0 } },	// 0
	{ { 0x1F, 0, 0, 0, 0 }, 32768, { 5, 0, 0, 0, 0xC8, 0, 0, 0, 0x90, 1, 0, 0 } }	// 1
};
CT_ASSERT(NUM_ELEMENTS(gibFxGlassT) == 2);


static const GIBFX gibFxGlassS[] =
{
	{ { 0x12, 0, 0, 0, 0 }, 65536, { 0xA, 0, 0, 0, 0xC8, 0, 0, 0, 0x90, 1, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxGlassS) == 1);


// TODO: Fix the remaining GIBFX arrays, I got lazy
static const GIBFX gibFxBurnShard[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxBurnShard) == 1);


static const GIBFX gibFxWoodShard[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxWoodShard) == 1);


static const GIBFX gibFxMetalShard[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxMetalShard) == 1);


static const GIBFX gibFxFireSpark[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxFireSpark) == 1);


static const GIBFX gibFxShockSpark[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxShockSpark) == 1);


static const GIBFX gibFxBloodChunks[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxBloodChunks) == 1);


static const GIBFX gibFxBubblesS[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxBubblesS) == 1);


static const GIBFX gibFxBubblesM[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxBubblesM) == 1);


static const GIBFX gibFxBubblesL[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxBubblesL) == 1);


static const GIBFX gibFxIcicles[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxIcicles) == 1);


static const GIBFX gibFxGlassCombo1[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 1
};
CT_ASSERT(NUM_ELEMENTS(gibFxGlassCombo1) == 2);


static const GIBFX gibFxGlassCombo2[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 1
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 2
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 3
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 4
};
CT_ASSERT(NUM_ELEMENTS(gibFxGlassCombo2) == 5);


static const GIBFX gibFxWoodCombo[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 1
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 2
};
CT_ASSERT(NUM_ELEMENTS(gibFxWoodCombo) == 3);


static const GIBFX gibFxMedicCombo[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 1
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 2
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 3
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 4
};
CT_ASSERT(NUM_ELEMENTS(gibFxMedicCombo) == 5);


static const GIBFX gibFxFlareSpark[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxFlareSpark) == 1);


static const GIBFX gibFxBloodBits[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 0
};
CT_ASSERT(NUM_ELEMENTS(gibFxBloodBits) == 1);


static const GIBFX gibFxRockShards[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 1
};
CT_ASSERT(NUM_ELEMENTS(gibFxRockShards) == 2);


static const GIBFX gibFxPaperCombo1[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 1
};
CT_ASSERT(NUM_ELEMENTS(gibFxPaperCombo1) == 2);


static const GIBFX gibFxPlantCombo1[] =
{
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 0
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } },	// 1
	{ { 0x0, 0, 0, 0, 0 }, 0, { 0x0, 0, 0, 0, 0x0, 0, 0, 0, 0x0, 0, 0, 0 } }	// 2
};
CT_ASSERT(NUM_ELEMENTS(gibFxPlantCombo1) == 3);


static const GIBLIST gibList[] =
{
	{ gibFxGlassT,      2,        0, 0, 300 },	// 0
	{ gibFxGlassS,      1,        0, 0, 300 },	// 1
	{ gibFxBurnShard,   1,        0, 0,   0 },	// 2
	{ gibFxWoodShard,   1,        0, 0,   0 },	// 3
	{ gibFxMetalShard,  1,        0, 0,   0 },	// 4
	{ gibFxFireSpark,   1,        0, 0,   0 },	// 5
	{ gibFxShockSpark,  1,        0, 0,   0 },	// 6
	{ gibFxBloodChunks, 1,        0, 0,   0 },	// 7
	{ gibFxBubblesS,    1,        0, 0,   0 },	// 8
	{ gibFxBubblesM,    1,        0, 0,   0 },	// 9
	{ gibFxBubblesL,    1,        0, 0,   0 },	// 10
	{ gibFxIcicles,     1,        0, 0,   0 },	// 11
	{ gibFxGlassCombo1, 2,        0, 0, 300 },	// 12
	{ gibFxGlassCombo2, 5,        0, 0, 300 },	// 13
	{ gibFxWoodCombo,   3,        0, 0,   0 },	// 14
	{ NULL,             0, 0x107690, 7,   0 },	// 15	// What is this?
	{ gibFxMedicCombo,  5,        0, 0,   0 },	// 16
	{ gibFxFlareSpark,  1,        0, 0,   0 },	// 17
	{ gibFxBloodBits,   1,        0, 0,   0 },	// 18
	{ gibFxRockShards,  2,        0, 0,   0 },	// 19
	{ gibFxPaperCombo1, 2,        0, 0,   0 },	// 20
	{ gibFxPlantCombo1, 3,        0, 0,   0 },	// 21

	// Next 5 are exactly the same
	{ gibFxBloodChunks, 1,        0, 0,   0 },	// 22
	{ gibFxBloodChunks, 1,        0, 0,   0 },	// 23
	{ gibFxBloodChunks, 1,        0, 0,   0 },	// 24
	{ gibFxBloodChunks, 1,        0, 0,   0 },	// 25
	{ gibFxBloodChunks, 1,        0, 0,   0 },	// 26

	{ NULL,             0, 0x107848, 1,   0 },	// 27
	{ NULL,             0, 0x10771C, 6,   0 },	// 28
	{ NULL,             0, 0x107794, 4,   0 },	// 29
	{ NULL,             0, 0x1077E4, 5,   0 }	// 30
};
CT_ASSERT(NUM_ELEMENTS(gibList) == kGibMax);


static void GibFX(SPRITE *pSprite, const GIBFX *pFX, CGibPosition *pPos, CGibVelocity *pVel)
{
}


void GibSprite(SPRITE *pSprite, uint8 nGibType, CGibPosition *pPos, CGibVelocity *pVel)
{
	dassert(pSprite != NULL);
	dassert(nGibType >= 0 && nGibType < kGibMax);

	const GIBLIST *pGibList = &gibList[nGibType];
	for (int i = 0; i < pGibList->nNumFX; i++)
	{
		dassert(pGibList->pFX[i].chance > 0);
		GibFX(pSprite, &pGibList->pFX[i], pPos, pVel);
	}
}
