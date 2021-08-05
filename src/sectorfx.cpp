#include "stdafx.h"
#include "game.h"
#include "debug.h"


static int shadeCount;
static int panCount;
static int wallPanCount;

static int16 panList[512];
static int16 shadeList[512];	// Size?


void InitSectorFX()
{
	shadeCount   = 0;
	panCount     = 0;
	wallPanCount = 0;

	// Init sectors with motion or lighting effects
	for (int i = 0; i < numsectors; i++)
	{
		int nXSector = sector[i].extra;
		if (nXSector > 0)
		{
			XSECTOR *pXSector = &xsector[nXSector];

			if (pXSector->lightingAmplitude)
			{
				dassert(shadeCount >= 0 && shadeCount < NUM_ELEMENTS(shadeList));
				shadeList[shadeCount++] = nXSector;
			}

			if (pXSector->motionfx_speed)
			{
				dassert(panCount >= 0 && panCount < NUM_ELEMENTS(panList));
				panList[panCount++] = nXSector;
			}
		}
	}

	// Init walls with effects
	for (int i = 0; i < numwalls; i++)
	{
		int nXWall = wall[i].extra;
		if (nXWall > 0)
		{
			XWALL *pXWall = &xwall[nXWall];
		}
	}
}


static const uint8 flicker2[64] =
{
	1, 2, 4, 2, 3, 4, 3, 2,
	0, 0, 1, 2, 4, 3, 2, 0,
	2, 1, 0, 1, 0, 2, 3, 4,
	3, 2, 1, 1, 2, 0, 0, 1,
	1, 2, 3, 4, 4, 3, 2, 1,
	2, 3, 4, 4, 2, 1, 0, 1,
	0, 0, 0, 0, 1, 2, 3, 4,
	3, 2, 1, 2, 3, 4, 3, 2
};


void DoSectorLighting()
{
	static bool once = false;

	for (int i = 0; i < shadeCount; i++)
	{
		int nXSector = shadeList[i];
		XSECTOR *pXSector = &xsector[nXSector];
		int nSector = pXSector->reference;
		int8 nUnknown = pXSector->shade_value;
//		dprintf("Shading sector %d, value %d\n", nSector, nUnknown);
		if (nUnknown)
		{
			if (pXSector->shade_floor)
				sector[nSector].floorshade -= nUnknown;
			if (pXSector->shade_ceiling)
				sector[nSector].ceilingshade -= nUnknown;
			if (pXSector->shade_walls)
			{
				// Shade all the walls of the sector
				WALL *pWall = &wall[sector[nSector].wallptr];
				for (int j = 0; j < sector[nSector].wallnum; j++)
				{
					pWall->shade -= nUnknown;
					pWall++;
				}
			}
			pXSector->shade_value = 0;
		}

		if (pXSector->shade_always)	// TODO: Or condition
		{
			int nAmplitude = pXSector->lightingAmplitude;
			if (!pXSector->shade_always)
			{
			}

			uint32 nBleh = (8 * uint32(pXSector->lightingPhase) + gGameClock * uint32(pXSector->lightingFrequency)) & 2047;

			int nShadeValue = 0;
			switch (pXSector->waveLighting)
			{
				case 0:  break;	// None
				case 1:  break;
				case 2:  break;	// Saw
				case 3:  break;
				case 4:  break;
				case 5:  break;
				case 6:  break;

				// Flicker2
				case 7:
					nShadeValue = (nAmplitude * int(flicker2[nBleh >> 5])) >> 2;
					break;

				case 8:  break;
				case 9:  break;
				case 10: break;
				case 11: break;
			}

			if (pXSector->shade_floor)
			{
				int nNewShade = sector[nSector].floorshade + nShadeValue;
				if (nNewShade < -128) nNewShade = -128;
				if (nNewShade >  127) nNewShade =  127;
				sector[nSector].floorshade = nNewShade;
			}

			if (pXSector->shade_ceiling)
			{
				int nNewShade = sector[nSector].ceilingshade + nShadeValue;
				if (nNewShade < -128) nNewShade = -128;
				if (nNewShade >  127) nNewShade =  127;
				sector[nSector].ceilingshade = nNewShade;
			}

			if (pXSector->shade_walls)
			{
				WALL *pWall = &wall[sector[nSector].wallptr];
				for (int j = 0; j < sector[nSector].wallnum; j++)
				{
					int nNewShade = pWall->shade + nShadeValue;
					if (nNewShade < -128) nNewShade = -128;
					if (nNewShade >  127) nNewShade =  127;
					pWall->shade = nNewShade;
					pWall++;
				}
			}

			pXSector->shade_value = nShadeValue;
		}
	}

	once = true;
}


void DoSectorPanning()
{
	for (int i = 0; i < panCount; i++)
	{
		int nXSector = panList[i];
		XSECTOR *pXSector = &xsector[nXSector];
		int nSector = pXSector->reference;
		dassert(nSector >= 0 && nSector < NUM_ELEMENTS(sector));
		SECTOR *pSector = &sector[nSector];
		dassert(pSector->extra == nXSector);

		if (pXSector->pan_always || pXSector->unknown_bits0)
		{
			int nPanAngle = pXSector->motionfx_angle + 1024;

			if (!pXSector->pan_always)
			{
			}

			// Floor panning
			if (pXSector->pan_floor)
			{
//				if (pSector->floorstat & 0x40)
//					nPanAngle = pXSector->motionfx_angle + 512;
//				int ypanning = pXSector->unknown_bits14 + (pSector->floorypanning << 8)
//					- costable[(nPanAngle - 512) & 2047]
//				pSector->floorypanning = ypanning;
			}
		}
	}

	// TODO: Wall panning
}
