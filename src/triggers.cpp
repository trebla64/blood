#include "stdafx.h"
#include "game.h"
#include "view.h"
#include "trig.h"
#include "debug.h"


#define MAX_BUSY 128


// Sector types (From mapedit)
#define kSectorTypeZMotion       600
#define kSectorTypeZMotionSprite 602
#define kSectorTypeWarp          603
#define kSectorTypeTeleporter    604
#define kSectorTypePath          612
#define kSectorTypeStepRotate    613
#define kSectorTypeSlideMarked   614
#define kSectorTypeRotateMarked  615
#define kSectorTypeSlide         616
#define kSectorTypeRotate        617
#define kSectorTypeDamageSector  618
#define kSectorTypeCounterSector 619


// Event command for sector defines
#define kSectorCmdOff        0
#define kSectorCmdOn         1
#define kSectorCmdState      2
#define kSectorCmdToggle     3
#define kSectorCmdNotState   4
#define kSectorCmdLink       5
#define kSectorCmdLock       6
#define kSectorCmdUnlock     7
#define kSectorCmdToggleLock 8
#define kSectorCmdStopOff    9
#define kSectorCmdStopOn     10
#define kSectorCmdStopNext   11
	// The rest appears to be unused


struct BUSY
{
	int   nIndex;
	int   nDelta;
	int   nBusy;
	uint8 busyProc;
};

struct EVENT
{
	uint32 unknown0 : 8;
	uint32 command  : 8;
	uint32 unknown1 : 16;
};
CT_ASSERT(sizeof(EVENT) == 4);

static int  gBusyCount;
static BUSY gBusy[MAX_BUSY];

typedef int (*BusyProc_t)(uint32, uint32);

static int VCrushBusy(uint32, uint32);
static int VSpriteBusy(uint32, uint32);
static int VDoorBusy(uint32, uint32);
static int HDoorBusy(uint32, uint32);
static int RDoorBusy(uint32, uint32);
static int StepRotateBusy(uint32, uint32);
static int GenSectorBusy(uint32, uint32);
static int PathBusy(uint32, uint32);

static const BusyProc_t gBusyProc[] =
{
	VCrushBusy,
	VSpriteBusy,
	VDoorBusy,
	HDoorBusy,
	RDoorBusy,
	StepRotateBusy,
	GenSectorBusy,
	PathBusy
};

static int32 velFloor[1024];
static int32 velCeil[1024];


static void OperateDoor(unsigned nSector, XSECTOR *pXSector, EVENT event, int busy)
{
}


static void OperateSector(unsigned nSector, XSECTOR *pXSector, EVENT event)
{
	dassert(nSector < numsectors);

	switch (event.command)
	{
		case kSectorCmdLock:       break;
		case kSectorCmdUnlock:     break;
		case kSectorCmdToggleLock: break;
		case kSectorCmdStopOff:    break;
		case kSectorCmdStopOn:     break;
		case kSectorCmdStopNext:   break;

		default:
		{
			switch (sector[nSector].lotag)	// TODO: type
			{
				case kSectorTypeZMotion:		// 600
					OperateDoor(nSector, pXSector, event, 2);
					break;

				case kSectorTypeZMotionSprite:	// 602
					OperateDoor(nSector, pXSector, event, 1);
					break;

				case kSectorTypeTeleporter:		// 604
					// Teleport
					// TODO: Add event.command check and other stuff
					break;

				case kSectorTypePath:			// 612
					// TODO: Add call to OperatePath()
					break;

				case kSectorTypeSlideMarked:	// 614
				case kSectorTypeSlide:			// 616
					OperateDoor(nSector, pXSector, event, 3);
					break;

				case kSectorTypeRotateMarked:	// 615
				case kSectorTypeRotate:			// 617
					OperateDoor(nSector, pXSector, event, 4);
					break;

				default:
					// TODO: Call OperateDoor() or SetSectorState()
					break;
			}
			break;
		}
	}
}


static int GetWaveValue(uint32 a, int b, int c)
{
	int result = 0;

	switch (b)
	{
		case 0:
			result = 32768 - (costable[((a << 10) >> 16) & 2047] >> 15);
			break;
		case 2:
			result = 65536 - (costable[((a << 9) >> 16) & 2047] >> 14);
			break;
		case 3:
			result = costable[((a << 9) >> 16) & 2047] >> 14;
			break;
	}

	return result;
}


static void ZTranslateSector(int nSector, XSECTOR *pXSector, int b, int c)
{
	dassert(nSector >= 0 && nSector < NUM_ELEMENTS(sector));
	SECTOR *pSector = &sector[nSector];

	uint8 *pInterpolate = &gInterpolateSector[nSector >> 3];
	if (!(*pInterpolate & (1 << (nSector & 7))))
	{
		viewAddInterpolation(&pSector->floorz, 0);
		viewAddInterpolation(&pSector->ceilingz, 0);
		viewAddInterpolation(&pSector->floorheinum, 1);
		*pInterpolate |= 1 << (nSector & 7);
	}

	int nDelta = pXSector->field_28 - pXSector->field_24;
	dprintf("ZTranslateSector: nSector = %d, nDelta = %d\n", nSector, nDelta);
	if (nDelta != 0)
	{
		int nWave = GetWaveValue(b, c, nDelta);
	}
}


static void InitPath(int nSector, XSECTOR *pXSector)
{
}


void trInit()
{
	gBusyCount = 0;

	// Get wall vertex positions
	for (int i = 0; i < numwalls; i++)
	{
		baseWall[i].x = wall[i].x;
		baseWall[i].y = wall[i].y;
	}

	// Get sprite positions
	for (int i = 0; i < MAXSPRITES; i++)
		if (sprite[i].statnum < 1024)
		{
			baseSprite[i].x = sprite[i].x;
			baseSprite[i].y = sprite[i].y;
			baseSprite[i].z = sprite[i].z;
		}

	// Init wall trigger masks
	for (int i = 0; i < numwalls; i++)
	{
		int nXWall = wall[i].extra;
		if (nXWall > 0)
		{
			dassert(nXWall < NUM_ELEMENTS(xwall));
			XWALL *pXWall = &xwall[nXWall];
			if (pXWall->state)
				pXWall->busy = 0;	// TODO: FIXME
		}
	}

	// Init sector trigger masks
	for (int i = 0; i < numsectors; i++)
	{
		SECTOR *pSector = &sector[i];

		baseFloor[i] = pSector->floorz;
		baseCeil[i]  = pSector->ceilingz;

		int nXSector = pSector->extra;
		if (nXSector <= 0)
			continue;
		dassert(nXSector < NUM_ELEMENTS(xsector));
		XSECTOR *pXSector = &xsector[nXSector];

		switch (pSector->lotag)
		{
			case 600:	// Z Motion
			case 602:	// Z Motion sprite
//				dprintf("Got untranslated Z in sector %d\n", i);
				ZTranslateSector(i, pXSector, 0, 1);	// TODO: Fix the 0 value
				continue;

			case 612:
				InitPath(i, pXSector);
				break;
		}
	}
}


void trTriggerSector(unsigned nSector, XSECTOR *pXSector, int command)
{
#if 0
	dprintf("TriggerSector: nSector=%d, command=%d\n", nSector, command);
	dassert(nSector < numsectors);
	if (!(pXSector->field_34 & 0x100))
	{
		uint8 foo = BYTE2(pXSector->field_14);
		if (!(foo & 0x40))
		{
			if (foo & 0x20)
				pXSector->field_14 |= 0x400000;
			if (pXSector->field_14 & 0x100000)
			{
				// TODO: Call evSend() here
			}
			else
			{
				// TODO: Other fields of event?
				EVENT e; e.command = command;
				OperateSector(nSector, pXSector, e);
			}
		}
	}
#endif
}


void AddBusy(int nIndex, uint8 busyProc, int nDelta)
{
	dassert(nDelta != 0);

	// Find a free busy entry
	int i;
	for (i = 0; i < gBusyCount && (nIndex != gBusy[i].nIndex || busyProc != gBusy[i].busyProc); i++);

	if (i == gBusyCount)
	{
		if (gBusyCount == MAX_BUSY)
		{
			dprintf("OVERFLOW: AddBusy() ignored\n");
			return;
		}

		BUSY *pBusy = &gBusy[gBusyCount];
		pBusy->nIndex   = nIndex;
		pBusy->busyProc = busyProc;
		pBusy->nBusy    = nDelta <= 0 ? 65536 : 0;

		gBusyCount++;
	}
	gBusy[i].nDelta = nDelta;
}


static void ProcessMotion()
{
}


static void AlignSlopes()
{
	SECTOR *pSector = sector;
	for (int i = 0; i < numsectors; i++)
	{
		if (pSector->filler)
		{
			WALL *pWall = &wall[pSector->filler + pSector->wallptr];
			int nNextSector = pWall->nextsector;
			WALL *pNextWall = &wall[pWall->point2];
			if (nNextSector >= 0)
			{
				int32 midx = (pNextWall->x - pWall->x) / 2;
				int32 midy = (pNextWall->y - pWall->y) / 2;
				uint8 *pLerpSector = &gInterpolateSector[i >> 3];
				if (!(*pLerpSector & (1 << (i & 7))))
				{
					viewAddInterpolation(&pSector->floorz, 0);
					viewAddInterpolation(&pSector->ceilingz, 0);
					viewAddInterpolation(&pSector->floorheinum, 1);
					*pLerpSector |= 1 << (i & 7);
				}
				int32 z = getflorzofslope(nNextSector, midx, midy);
				alignflorslope(i, midx, midy, z);
				z = getceilzofslope(nNextSector, midx, midy);
				alignceilslope(i, midx, midy, z);
			}
		}
		pSector++;
	}
}


void trProcessBusy()
{
	memset(velFloor, 0, sizeof(velFloor));
	memset(velCeil, 0, sizeof(velCeil));

	for (int i = gBusyCount; i >= 0; i--)
	{
		BUSY *pBusy = &gBusy[i];

		int nOldBusy = pBusy->nBusy;
		int nNewBusy = nOldBusy + 4 * pBusy->nDelta;
		if (nNewBusy < 0)     nNewBusy = 0;
		if (nNewBusy > 65536) nNewBusy = 65536;
		pBusy->nBusy = nNewBusy;

		dassert(pBusy->busyProc >= 0 && pBusy->busyProc < NUM_ELEMENTS(gBusyProc));
		int nRet = gBusyProc[pBusy->busyProc](pBusy->nIndex, pBusy->nBusy);

		switch (nRet)
		{
			case 1:
				pBusy->nBusy = nOldBusy;
				break;

			case 2:
				pBusy->nBusy  = nOldBusy;
				pBusy->nDelta = -pBusy->nDelta;
				break;

			case 3:
				gBusyCount--;
				// TODO: Lots of weird stuff happening here
				break;
		}
	}

	ProcessMotion();
	AlignSlopes();
}


void trMessageSector(unsigned int nSector, EVENT event)
{
	dassert(nSector < numsectors);
	int nXSector = sector[nSector].extra;
	dassert(nXSector > 0 && nXSector < NUM_ELEMENTS(xsector));
	XSECTOR *pXSector = &xsector[nXSector];

	if (pXSector->unknown_bits20)
	{
	}
}


static int VCrushBusy(uint32, uint32)
{
	return 0;
}


static int VSpriteBusy(uint32, uint32)
{
	return 0;
}


static int VDoorBusy(uint32, uint32)
{
	return 0;
}


static int HDoorBusy(uint32, uint32)
{
	return 0;
}


static int RDoorBusy(uint32, uint32)
{
	return 0;
}


static int StepRotateBusy(uint32, uint32)
{
	return 0;
}


static int GenSectorBusy(uint32, uint32)
{
	return 0;
}


static int PathBusy(uint32, uint32)
{
	return 0;
}
