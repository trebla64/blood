#include "stdafx.h"
#include "resource.h"
#include "game.h"
#include "crc32.h"
#include "debug.h"
#include "error.h"

#include "jfbuild/compat.h"
#include "jfbuild/build.h"

#define kMaxStatus 1024

#define BLM_SIG 0x1A4D4C42

#pragma pack(push, 1)

struct MAPHEADER
{
	char   zSignature[4];	// "BLM\x1A"
	uint16 uVersion;
	int32  posx;
	int32  posy;
	int32  posz;
	int16  ang;
	int16  sector;
	int16  skybits;
	int32  visibility;
	int32  songid;
	int8   parallaxtype;
	uint32 unknown1;
	uint16 numsectors;
	uint16 numwalls;
	uint16 numsprites;
};
CT_ASSERT(sizeof(MAPHEADER) == (37 + 6));

#pragma pack(pop)


/**
 * Inserts a sprite into a sector
 */
void InsertSpriteSect( int nSprite, int nSector )
{
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	dassert(nSector >= 0 && nSector < MAXSECTORS);
	int nFoo = headspritesect[nSector];
	if (nFoo < 0)
	{
		prevspritesect[nSprite] = nSprite;
		headspritesect[nSector] = nSprite;
		nextspritesect[nSprite] = -1;
	}
	else
	{
		int nPrevSprite = prevspritesect[nFoo];
		nextspritesect[nSprite] = -1;
		prevspritesect[nSprite] = nPrevSprite;
		prevspritesect[nFoo] = nSprite;
		nextspritesect[nPrevSprite] = nSprite;
	}
	sprite[nSprite].sectnum = nSector;
}


/**
 * Removes a sprite from its sector
 */
void RemoveSpriteSect( int nSprite )
{
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	int nSector = sprite[nSprite].sectnum;
	dassert(nSector >= 0 && nSector < MAXSECTORS);
	int nFoo = nextspritesect[nSprite];
	if (nFoo < 0)
		nFoo = headspritesect[nSector];
	prevspritesect[nFoo] = prevspritesect[nSprite];
	if (headspritesect[nSector] == nSprite)
		headspritesect[nSector] = nextspritesect[nSprite];
	else
		nextspritesect[prevspritesect[nSprite]] = nextspritesect[nSprite];
	sprite[nSprite].sectnum = -1;
}


void InsertSpriteStat( int nSprite, int nStat )
{
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	dassert(nStat >= 0 && nStat <= MAXSTATUS);
	if (headspritestat[nStat] < 0)
	{
		prevspritestat[nSprite] = nSprite;
		headspritestat[nStat]   = nSprite;
		nextspritestat[nSprite] = -1;
	}
	else
	{
		nextspritestat[nSprite] = -1;
		int nMeh = prevspritestat[headspritestat[nStat]];
		prevspritestat[nSprite] = nMeh;
		prevspritestat[headspritestat[nStat]] = nSprite;
		nextspritestat[nMeh] = nSprite;
	}
	sprite[nSprite].statnum = nStat;
}


void RemoveSpriteStat( int nSprite )
{
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	int nStat = sprite[nSprite].statnum;
	dassert(nStat >= 0 && nStat <= MAXSTATUS);
	int nFoo = nextspritestat[nSprite];
	if (nFoo < 0)
		nFoo = headspritestat[nStat];
	prevspritestat[nFoo] = prevspritestat[nSprite];
	if (headspritestat[nStat] == nSprite)
		headspritestat[nStat] = nextspritestat[nSprite];
	else
		nextspritestat[prevspritestat[nSprite]] = nextspritestat[nSprite];
	sprite[nSprite].statnum = -1;
}


// TODO: Implement
void dbDeleteXSprite( int nXSprite )
{
}


extern "C" int InsertSprite( int nSector, int nStat )
{
	int nSprite = headspritestat[1024];
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	if (nSprite >= 0)
	{
		SPRITE *pSprite = &sprite[nSprite];
		memset(pSprite, 0, sizeof(SPRITE));
		InsertSpriteStat(nSprite, nStat);
		InsertSpriteSect(nSprite, nSector);
		pSprite->cstat    = 128;
		pSprite->clipdist = 32;
		pSprite->xrepeat  = 64;
		pSprite->yrepeat  = 64;
		pSprite->owner    = -1;
		pSprite->extra    = -1;
		pSprite->index    = nSprite;

		xvel[nSprite] = 0;
		yvel[nSprite] = 0;
		zvel[nSprite] = 0;
	}

	if (nSprite < 0)
	{
		// This is bad, show linked lists
		dprintf("sprite status linked list:\n");
		for (int i = 0; i <= 1024; i++)
			dprintf("head[%4d]=%4d\n", i, headspritestat[i]);
		for (int i = 0; i <= 4096; i++)
			dprintf("%4d: prev=%4d next=%4d\n", i, prevspritestat[i], nextspritestat[i]);
	}

	return nSprite;
}


extern "C" void DeleteSprite( int nSprite )
{
	if ( sprite[nSprite].extra > 0 )
		dbDeleteXSprite( sprite[nSprite].extra );
	dassert(sprite[nSprite].statnum >= 0 && sprite[nSprite].statnum < MAXSTATUS);
	RemoveSpriteStat( nSprite );
	dassert(sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < MAXSECTORS);
	RemoveSpriteSect(nSprite);
	InsertSpriteStat(nSprite, 1024);
}


extern "C" int ChangeSpriteSect( int nSprite, int nSector )
{
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	dassert(nSector >= 0 && nSector < MAXSECTORS);
	dassert(sprite[nSprite].statnum >= 0 && sprite[nSprite].statnum < MAXSTATUS);
	dassert(sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < MAXSECTORS);
	RemoveSpriteSect( nSprite );
	InsertSpriteSect( nSprite, nSector );
	return 0;
}


extern "C" int ChangeSpriteStat( int nSprite, int nNewStatus )
{
	dassert(nSprite >= 0 && nSprite < MAXSPRITES);
	dassert(nNewStatus >= 0 && nNewStatus < MAXSTATUS);
	dassert(sprite[nSprite].statnum >= 0 && sprite[nSprite].statnum < MAXSTATUS);
	dassert(sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < MAXSECTORS);
	RemoveSpriteStat( nSprite );
	InsertSpriteStat( 0, nNewStatus );	// TODO: FIXME
	return 0;
}


// Only used by "dbInit()"
void InitFreeList( ushort *ptr, int nCount )
{
	if (nCount > 1)
	{
		int i = 1;
		do
		{
			int nFoo = i++;
			ptr[i - 1] = nFoo - 1;
		}
		while (i < nCount);
	}
	*ptr = nCount - 1;
}


static ushort nextXSector[MAX_XSECTORS];
static ushort nextXWall[MAX_XWALLS];
static ushort nextXSprite[MAX_XSPRITES];


// Only used by "dbLoadMap()"
static uint16 dbInsertXSector( int nIndex )
{
	uint16 nFree = nextXSector[0];
	if (!nFree)
		ThrowError("Out of free XSectors");
	nextXSector[0] = nextXSector[nFree];
	XSECTOR *pXSector = &xsector[nFree];
	memset(pXSector, 0, sizeof(XSECTOR));
	sector[nIndex].extra = nFree;
	pXSector->reference = nIndex;
	return nFree;
}


// Only used by "dbLoadMap()"
static uint16 dbInsertXWall( int nIndex )
{
	uint16 nFree = nextXWall[0];
	if (!nFree)
		ThrowError("Out of free XWalls");
	nextXWall[0] = nextXWall[nFree];
	XWALL *pXWall = &xwall[nFree];
	memset(pXWall, 0, sizeof(XWALL));
	wall[nIndex].extra = nFree;
	pXWall->reference = nIndex;
	return nFree;
}


static uint16 dbInsertXSprite( int nIndex )
{
	uint16 nFree = nextXSprite[0];
	if (!nFree)
		ThrowError("Out of free XSprites");
	nextXSprite[0] = nextXSprite[nFree];
	XSPRITE *pXSprite = &xsprite[nFree];
	memset(pXSprite, 0, sizeof(XSPRITE));
	sprite[nIndex].extra = nFree;
	pXSprite->reference = nIndex;
	return nFree;
}


void dbInit()
{
	InitFreeList( nextXSprite, MAX_XSPRITES );
	for ( int i = 0; i < MAX_XSPRITES; i++ )
	{
		// Set some flags on xsprite
	}
	InitFreeList( nextXWall, MAX_XWALLS );
	for ( int i = 0; i < MAX_XWALLS; i++ )
	{
		// Set some flags on xwall
	}
	InitFreeList( nextXSector, MAX_XSECTORS );
	for ( int i = 0; i < MAX_XSECTORS; i++ )
	{
		// Set some flags on xsector
	}
	initspritelists();
	for ( int i = 0; i < MAXSPRITES; i++ )
		sprite[i].cstat = 128;
}


// TODO: Implement this hairy one
void PropogateMarkerReferences( int nSprite )
{
}


uint32 dbReadMapCRC( const char *map )
{
	return 0;
}


#define ABS(x)	(((x) < 0) ? -(x) : (x))
static void DrawLine(uint8 *base, int x0, int y0, int x1, int y1, int w, int h)
{
	int dx =  ABS(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -ABS(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	for (;;) {
		if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h)
		{
			uint8 *p = &base[(3*x0) + (3*y0 * w)];
			*p++ = 255;
			*p++ = 255;
			*p++ = 255;
		}
		if (x0 == x1 && y0 == y1) break;
		int e2 = err << 1;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}


#ifdef _MSC_VER
// Decryption function for v7 BLM maps
// TODO: Simplify this and convert to C++
__declspec(naked) static void __cdecl AnotherCrypt(uint8 *buffer, uint32 size, uint32 key)
{
	__asm {
		push eax	// eax|ret|buffer|size|key
		push edx	// edx|eax|ret|buffer|size|key
		push ebx	// ebx|edx|eax|ret|buffer|size|key
		push ecx	// ecx|ebx|edx|eax|ret|buffer|size|key
		push esi	// esi|ecx|ebx|edx|eax|ret|buffer|size|key

		// Normalize to __fastcall calling convention first
		mov eax, [esp + 24]
		mov edx, [esp + 28]
		mov ebx, [esp + 32]

		mov esi, eax	// esi = buffer
		mov ecx, edx	// ecx = size/counter
		mov edx, ebx
		xor eax, eax
		cmp eax, ecx
		jmp checkcond

looptop:
		mov bl, [esi+eax]
		inc eax
		xor bl, dl
		inc edx
		mov [esi+eax-1], bl

checkcond:
		cmp eax, ecx
		jl looptop

		pop esi
		pop ecx
		pop ebx
		pop edx
		pop eax

		ret
	}
}
#else

static void AnotherCrypt(uint8 *buffer, uint32 size, uint32 key)
{
	for (uint32 i = 0; i < size; i++)
	{
		buffer[i] ^= (key & 0xFF);
		key++;
	}
}

#endif


#if 0
static void DecodeSectorFlags(const XSECTOR *pXSector, char *buf, unsigned size)
{
	int pos = 0;
	int num_flags = 0;
#define TEST_FLAG(flag)	\
	if (pXSector->flags & flag)	\
	{	\
		if (num_flags > 0) pos += Bsnprintf(&buf[pos], size - pos, " | ");	\
		pos += Bsnprintf(&buf[pos], size - pos, #flag);	\
		num_flags++;	\
	}

	TEST_FLAG(SFL_UNKNOWN0);
	TEST_FLAG(SFL_UNKNOWN1);
	TEST_FLAG(SFL_UNKNOWN2);
	TEST_FLAG(SFL_UNKNOWN3);
	TEST_FLAG(SFL_UNKNOWN4);
	TEST_FLAG(SFL_UNKNOWN5);
	TEST_FLAG(SFL_PAN_FLOOR);
	TEST_FLAG(SFL_UNKNOWN7);
#undef TEST_FLAG
}
#endif


// This function does all the work of loading the map into our game structures
// Note: This modifies the buffer passed
static void dbLoadMemoryMap(void *pBuffer, int *posx, int *posy, int *posz, short *ang, short *sectnum, unsigned int *seed)
{
	dprintf("dbLoadMemoryMap()\n");

	// Ensure (again), that our types are the sizes we expect
	assert(sizeof(SECTOR) == 40);
	assert(sizeof(WALL)   == 32);
	assert(sizeof(SPRITE) == 44);

	// Cast to byte, so we can skip bytes easily
	uint8 *pMap = (uint8 *)pBuffer;

	// Clear game variables
	numsectors = 0;
	numwalls   = 0;

	memset(sector, 0, sizeof(sector));
	memset(wall, 0, sizeof(wall));
	memset(sprite, 0, sizeof(sprite));

	memset(xsector, 0, sizeof(xsector));
	memset(xwall, 0, sizeof(xwall));
	memset(xsprite, 0, sizeof(xsprite));

	// Decide what to load based on ID, either BLM or BUILD map version 7
	uint32 id = *(uint32 *)pBuffer;
	if (id == 7)
	{
		dprintf("    Loading standard BUILD v7 map\n");
	}
	else if (id == BLM_SIG)
	{
		MAPHEADER *pHdr = (MAPHEADER *)pMap;

		bool bEncrypted = false;
		switch (pHdr->uVersion & 0xFF00)
		{
			case 1536:
				// Beta version maps
				dprintf("    Loading pre-retail BLM map (version %04X)\n", pHdr->uVersion);
				break;

			case 1792:
				// Retail version maps
				dprintf("    Loading retail (encrypted) BLM map (version %04X)\n", pHdr->uVersion);
				bEncrypted = true;	// Sigh, more work
				break;

			default:
				// Unknown map
				// TODO: Throw errors etc.
				break;
		}

		// Note: I use the songid member of the structure
		// It appears this was used as a songid in the beta, but used
		// as a key for decryption now
		// Note also that a different decryption function is used here instead
		// of the same one used for the RFF stuff
		if (pHdr->songid != 0 && pHdr->songid != 0x7474614D && pHdr->songid != 0x4D617474)
		{
			// Just decrypt the stuff from posx onwards
			AnotherCrypt((uint8 *)&pHdr->posx, sizeof(MAPHEADER) - 6, 0x7474614D);
			// TODO: It sets some flag to true here
		}

		int numsprites = 0;

		*posx        = pHdr->posx;
		*posy        = pHdr->posy;
		*posz        = pHdr->posz;
		*ang         = pHdr->ang;
		*sectnum     = pHdr->sector;
		pskybits     = pHdr->skybits;
		visibility   = pHdr->visibility;
		parallaxtype = pHdr->parallaxtype;
		numsectors   = pHdr->numsectors;
		numsprites   = pHdr->numsprites;
		numwalls     = pHdr->numwalls;

		dprintf(
			"    Map statistics:\n"
			"        Sectors: %d\n"
			"        Walls  : %d\n"
			"        Sprites: %d\n"
			, numsectors
			, numwalls
			, numsprites
		);

		pMap += sizeof(MAPHEADER);	// Move over header

		// Clamp the number of sectors/walls/sprites
		if (numsectors > MAXSECTORS) numsectors = MAXSECTORS;
		if (numwalls   > MAXWALLS)   numwalls   = MAXWALLS;
		if (numsprites > MAXSPRITES) numsprites = MAXSPRITES;

		dbInit();

		// If encrypted, we read another 128 bytes here and decrypt it
		uint8 unknown_bytes[128];
		if (bEncrypted)
		{
//			fread(unknown_bytes, sizeof(unknown_bytes), 1, fp);
			pMap += 128;	// Just skip for now
			// TODO: Add decryption
		}

		// We read the skybits here (I just skip it for now)
		pMap += 2 * (1 << pskybits);

		uint32 xsectors_loaded = 0;
		uint32 xwalls_loaded   = 0;
		uint32 xsprites_loaded = 0;

		// Load the sectors
		dprintf("    Loading sectors...\n");
		for (int i = 0; i < numsectors; i++)
		{
			SECTOR *pSector = &sector[i];

			Bmemcpy(pSector, pMap, sizeof(SECTOR));
			pMap += sizeof(SECTOR);

			if (bEncrypted)
				AnotherCrypt((uint8 *)pSector, sizeof(SECTOR), 40 * pHdr->unknown1);

			if (i >= 0 && i <= 2)
			{
				dprintf("        Sector %d: %08X\n", i, CRC32(pSector, sizeof(SECTOR)));
			}

			if (pSector->extra > 0)
			{
				uint16 nXSector = dbInsertXSector(i);
				XSECTOR *pXSector = &xsector[nXSector];
				Bmemcpy(pXSector, pMap, sizeof(XSECTOR));
				pMap += sizeof(XSECTOR);

#if 0
//				char buf[128];
//				DecodeSectorFlags(pXSector, buf, sizeof(buf));

				// Hope we just have one sector
				dprintf(
					"XSector %d\n"
//					"    unknown_bits0    : %d\n"
					"    reference        : %d\n"

//					"    field_4          : %d\n"
					"    data             : %d\n"
					"    txID             : %d\n"
					"    waveType         : %d\n"
					"    unknown_bits1    : %d\n"
					"    waveType2        : %d\n"
					"    unknown_bits2    : %d\n"

//					"    field_8          : %d\n"
					"    rxID             : %d\n"
					"    command          : %d\n"
					"    sendAtOn         : %d\n"
					"    sendAtOff        : %d\n"
					"    busyTime         : %d\n"
					"    unknown_bits5    : %d\n"

//					"    field_C          : %d\n"
					"    waitTime         : %d\n"
					"    unknown_bits6    : %d\n"
					"    lightingAmplitude: %d\n"
					"    lightingFrequency: %d\n"
					"    haveWaitTime     : %d\n"
					"    haveWaitTime2    : %d\n"

//					"    field_10         : %d\n"
					"    lightingPhase    : %d\n"
					"    waveLighting     : %d\n"
					"    shade_always     : %d\n"
					"    shade_floor      : %d\n"
					"    shade_ceiling    : %d\n"
					"    shade_walls      : %d\n"
					"    shade_value      : %d\n"
					"    pan_always       : %d\n"
					"    pan_floor        : %d\n"
					"    pan_ceiling      : %d\n"
					"    drag             : %d\n"
					"    unknown_bits22   : %d\n"
//					"    flags            : %d\n"
					"    depth            : %d\n"

//					"    field_14         : %d\n"
					"    motionfx_speed   : %d\n"
					"    motionfx_angle   : %d\n"
					"    unknown_bits13   : %d\n"
					"    key              : %d\n"
					"    unknown_bits19   : %d\n"

//					"    field_18         : %d\n"
					"    colorLights      : %d\n"
					"    unknown_bits8    : %d\n"
					"    busyTime2        : %d\n"
					"    unknown_bits9    : %d\n"
					"    waitTime2        : %d\n"
					"    unknown_bits10   : %d\n"
					"    ceil_pal2        : %d\n"

					"    field_1C         : %d\n"
					"    field_20         : %d\n"
					"    field_24         : %d\n"
					"    field_28         : %d\n"
					"    field_2C         : %08X\n"

//					"    field_30         : %d\n"
					"    unknown_bits11   : %d\n"
					"    damageType       : %d\n"
					"    floor_pal2       : %d\n"

//					"    field_34         : %d\n"
					"    unknown_bits14   : %d\n"
					"    wind_velocity    : %d\n"
					"    wind_angle       : %d\n"
					"    wind_always      : %d\n"
					"    unknown_bits15   : %d\n"

//					"    field_38         : %d\n"
					"    z_theta          : %d\n"
					"    z_range          : %d\n"
					"    z_speed          : %d\n"
					"    z_motion_always  : %d\n"
					"    bob_floor        : %d\n"
					"    bob_ceiling      : %d\n"
					"    rotate           : %d\n"
					, nXSector
//					, pXSector->unknown_bits0
					, pXSector->reference

//					, pXSector->field_4
					, pXSector->data
					, pXSector->txID
					, pXSector->waveType
					, pXSector->unknown_bits1
					, pXSector->waveType2
					, pXSector->unknown_bits2

//					, pXSector->field_8
					, pXSector->rxID
					, pXSector->command
					, pXSector->sendAtOn
					, pXSector->sendAtOff
					, pXSector->busyTime
					, pXSector->unknown_bits5

//					, pXSector->field_C
					, pXSector->waitTime
					, pXSector->unknown_bits6
					, pXSector->lightingAmplitude
					, pXSector->lightingFrequency
					, pXSector->haveWaitTime
					, pXSector->haveWaitTime2

//					, pXSector->field_10
					, pXSector->lightingPhase
					, pXSector->waveLighting
					, pXSector->shade_always
					, pXSector->shade_floor
					, pXSector->shade_ceiling
					, pXSector->shade_walls
					, pXSector->shade_value
					, pXSector->pan_always
					, pXSector->pan_floor
					, pXSector->pan_ceiling
					, pXSector->drag
					, pXSector->unknown_bits22
					, pXSector->depth

//					, pXSector->field_14
					, pXSector->motionfx_speed
					, pXSector->motionfx_angle
					, pXSector->unknown_bits13
					, pXSector->key
					, pXSector->unknown_bits19

//					, pXSector->field_18
					, pXSector->colorLights
					, pXSector->unknown_bits8
					, pXSector->busyTime2
					, pXSector->unknown_bits9
					, pXSector->waitTime2
					, pXSector->unknown_bits10
					, pXSector->ceil_pal2

					, pXSector->field_1C
					, pXSector->field_20
					, pXSector->field_24
					, pXSector->field_28
					, pXSector->field_2C

//					, pXSector->field_30
					, pXSector->unknown_bits11
					, pXSector->damageType
					, pXSector->floor_pal2

//					, pXSector->field_34
					, pXSector->unknown_bits14
					, pXSector->wind_velocity
					, pXSector->wind_angle
					, pXSector->wind_always
					, pXSector->unknown_bits15

//					, pXSector->field_38
					, pXSector->z_theta
					, pXSector->z_range
					, pXSector->z_speed
					, pXSector->z_motion_always
					, pXSector->bob_floor
					, pXSector->bob_ceiling
					, pXSector->rotate
				);
				dprintf("unknown_bits22: ");
				for (int z = 0; z < 1; z++)
					dprintf("%d", (pXSector->unknown_bits22 >> z) & 1);
				dprintf("\n");
#endif

				pXSector->reference = i;
				xsectors_loaded++;
			}
		}

		// Load the walls
		for (int i = 0; i < numwalls; i++)
		{
			WALL *pWall = &wall[i];

			Bmemcpy(pWall, pMap, sizeof(WALL));
			pMap += sizeof(WALL);

			if (bEncrypted)
				AnotherCrypt((uint8 *)pWall, sizeof(WALL), 40 * pHdr->unknown1 | 0x7474614D);

			if (pWall->extra > 0)
			{
				uint16 nXWall = dbInsertXWall(i);
				XWALL *pXWall = &xwall[nXWall];
				Bmemcpy(pXWall, pMap, sizeof(XWALL));
				pMap += sizeof(XWALL);

				pXWall->reference = i;
				xwalls_loaded++;
			}
		}

		// Load the sprites
		initspritelists();
		for (int i = 0; i < numsprites; i++)
		{
			RemoveSpriteStat(i);

			SPRITE *pSprite = &sprite[i];

			Bmemcpy(pSprite, pMap, sizeof(SPRITE));
			pMap += sizeof(SPRITE);

			if (bEncrypted)
				AnotherCrypt((uint8 *)pSprite, sizeof(SPRITE), 44 * pHdr->unknown1 | 0x7474614D);

			if (i == 1)
			{
				// TODO: Check this on Windows
				// Sprite 1 should have sector 237?
				// If so, then endian problem :/
				int16_t bleh = ((pSprite->sectnum & 0xFF00) >> 8) | ((pSprite->sectnum & 0xFF) << 8);
				dprintf(
					"sector = %d\n"
					"bleh   = %d\n"
					, pSprite->sectnum
					, bleh
				);
			}

			InsertSpriteSect(i, sprite[i].sectnum);
			InsertSpriteStat(i, sprite[i].statnum);

			if (pSprite->extra > 0)
			{
				uint16 nXSprite = dbInsertXSprite(i);
				XSPRITE *pXSprite = &xsprite[nXSprite];
				Bmemcpy(pXSprite, pMap, sizeof(XSPRITE));
				pMap += sizeof(XSPRITE);

#if 0
				XTEST *pXTest = (XTEST *)pXSprite;
				dprintf(
					"XSprite %d:\n"
//					"    field_0       : %d\n"
					"    unknown_bits0 : %d\n"
					"    state         : %d\n"
					"    unknown_bits2 : %d\n"

//					"    field_4 : %d\n"
					"    txID            : %d\n"
					"    rxID            : %d\n"
					"    command         : %d\n"
					"    triggerOn       : %d\n"
					"    triggerOff      : %d\n"
					"    unknown_bits1   : %d\n"

//					"    field_8         : %d\n"
					"    busyTime        : %d\n"
					"    waitTime        : %d\n"
					"    restState       : %d\n"
					"    unknown_bits3   : %d\n"
					"    delete_team     : %d\n"

//					"    field_C         : %d\n"
					"    unknown_bits4   : %d\n"
					"    delete_diff1    : %d\n"
					"    delete_diff2    : %d\n"
					"    delete_diff3    : %d\n"
					"    delete_diff4    : %d\n"
					"    delete_diff5    : %d\n"
					"    delete_single   : %d\n"
					"    delete_bloodbath: %d\n"
					"    delete_coop     : %d\n"
					"    unknown_bits5   : %d\n"

//					"    field_10        : %d\n"
					"    data1           : %d\n"
					"    data2           : %d\n"

//					"    field_14        : %d\n"
					"    data3           : %d\n"
					"    unknown_bits6   : %d\n"

//					"    field_18        : %d\n"
					"    unknown_bits7   : %d\n"
					"    data4           : %d\n"
					"    unknown_bits8   : %d\n"

					"    field_1C        : %d\n"
					"    field_20        : %d\n"
					"    field_24        : %d\n"
					"    field_28        : %d\n"
					"    field_2C        : %d\n"
					"    field_30        : %d\n"
					"    field_34        : %d\n"
					, nXSprite

//					, pXTest->field_0
					, pXTest->unknown_bits0
					, pXTest->state
					, pXTest->unknown_bits2

//					, pXTest->field_4
					, pXTest->txID
					, pXTest->rxID
					, pXTest->command
					, pXTest->triggerOn
					, pXTest->triggerOff
					, pXTest->unknown_bits1

//					, pXTest->field_8
					, pXTest->busyTime
					, pXTest->waitTime
					, pXTest->restState
					, pXTest->unknown_bits3
					, pXTest->delete_team

//					, pXTest->field_C
					, pXTest->unknown_bits4
					, pXTest->delete_diff1
					, pXTest->delete_diff2
					, pXTest->delete_diff3
					, pXTest->delete_diff4
					, pXTest->delete_diff5
					, pXTest->delete_single
					, pXTest->delete_bloodbath
					, pXTest->delete_coop
					, pXTest->unknown_bits5

//					, pXTest->field_10
					, pXTest->data1
					, pXTest->data2

//					, pXTest->field_14
					, pXTest->data3
					, pXTest->unknown_bits6

//					, pXTest->field_18
					, pXTest->unknown_bits7
					, pXTest->data4
					, pXTest->unknown_bits8

					, pXTest->field_1C
					, pXTest->field_20
					, pXTest->field_24
					, pXTest->field_28
					, pXTest->field_2C
					, pXTest->field_30
					, pXTest->field_34
				);

//				dprintf("field_18: ");
//				for (int z = 0; z < 32; z++)
//					dprintf("%d", (pXTest->field_18 >> z) & 1);
//				dprintf("\n");
#endif

				pXSprite->reference = i;
				xsprites_loaded++;
			}
		}

		dprintf("    Loaded %u xsectors, %u xwalls, and %u xsprites\n"
			, xsectors_loaded, xwalls_loaded, xsprites_loaded);
	}
}


void dbLoadMap( const char *map, int *posx, int *posy, int *posz, short *ang, short *sectnum, uint32 *seed )
{
	dprintf("dbLoadMap(\"%s\")\n", map);

	char map_name[64];
	Bsnprintf(map_name, sizeof(map_name), "%s", map);
	for (char *c = map_name; *c; c++)
		if (*c == '.')
		{
			*c = '\0';
			break;
		}

	ResHandle h = gSysRes.Lookup(map_name, "MAP");
	if (h.isNull())
		ThrowError("Error opening map file %s", map_name);

	void *pTmp = gSysRes.Lock(h);
	dprintf("Map CRC: %08X\n", CRC32(pTmp, h.size()));

	dbLoadMemoryMap(/*gSysRes.Lock(h)*/pTmp, posx, posy, posz, ang, sectnum, seed);
	return;

	int numsprites;

	uint8 *pMapData = (uint8 *)gSysRes.Lock(h);
	const MAPHEADER *pHeader = (const MAPHEADER *)pMapData;
	dprintf(
		"    Start info:\n"
		"        x      = %d\n"
		"        y      = %d\n"
		"        z      = %d\n"
		"        ang    = %d\n"
		"        sector = %d\n"
		, pHeader->posx
		, pHeader->posy
		, pHeader->posz
		, pHeader->ang
		, pHeader->sector
	);
	*posx        = pHeader->posx;
	*posy        = pHeader->posy;
	*posz        = pHeader->posz;
	*ang         = pHeader->ang;
	*sectnum     = pHeader->sector;
	pskybits     = pHeader->skybits;
	visibility   = pHeader->visibility;
	parallaxtype = pHeader->parallaxtype;
	numsectors   = pHeader->numsectors;
	numsprites   = pHeader->numsprites;
	numwalls     = pHeader->numwalls;

	dbInit();
	pMapData += sizeof(MAPHEADER);
	Bmemcpy(pskyoff, pMapData, 2 * (1 << pskybits));
	pMapData += 2 * (1 << pskybits);

	// Remove this later
	uint32 sectors_loaded  = 0;
	uint32 xsectors_loaded = 0;
	uint32 walls_loaded    = 0;
	uint32 xwalls_loaded   = 0;
	uint32 sprites_loaded  = 0;
	uint32 xsprites_loaded = 0;
	int32 xmin = INT_MAX;
	int32 xmax = INT_MIN;
	int32 ymin = INT_MAX;
	int32 ymax = INT_MIN;
	int32 translate_x;
	int32 translate_y;

	// Load sectors
	dassert(sizeof(SECTOR) == 40);
	dassert(sizeof(XSECTOR) == 60);
	for (int i = 0; i < numsectors; i++)
	{
		SECTOR *pSector = &sector[i];
		Bmemcpy(pSector, pMapData, sizeof(SECTOR));
		pMapData += sizeof(SECTOR);
		sectors_loaded++;

		if (pSector->extra > 0)
		{
			uint16 nXSector = dbInsertXSector(i);
			dprintf("Linked sector %d to XSector %d\n", i, nXSector);
			XSECTOR *pXSector = &xsector[nXSector];
			Bmemcpy(pXSector, pMapData, sizeof(XSECTOR));
			pXSector->reference = i;
			pMapData += sizeof(XSECTOR);
			xsectors_loaded++;
		}
	}

	// Load walls
	dassert(sizeof(WALL) == 32);
	dassert(sizeof(XWALL) == 24);
	for (int i = 0; i < numwalls; i++)
	{
		WALL *pWall = &wall[i];
		Bmemcpy(pWall, pMapData, sizeof(WALL));
		pMapData += sizeof(WALL);
		walls_loaded++;

		if (pWall->extra > 0)
		{
			uint16 nXWall = dbInsertXWall(i);
			XWALL *pXWall = &xwall[nXWall];
			Bmemcpy(pXWall, pMapData, sizeof(XWALL));
			pXWall->reference = i;
			pMapData += sizeof(XWALL);
			xwalls_loaded++;
		}
	}

	// Load sprites
	initspritelists();
	dassert(sizeof(SPRITE) == 44);
	dassert(sizeof(XSPRITE) == 56);
	for (int i = 0; i < numsprites; i++)
	{
		RemoveSpriteStat(i);

		// Copy sprite memory
		SPRITE *pSprite = &sprite[i];
		Bmemcpy(pSprite, pMapData, sizeof(SPRITE));
		pMapData += sizeof(SPRITE);

		InsertSpriteSect(i, sprite[i].sectnum);
		InsertSpriteStat(i, sprite[i].statnum);

		sprites_loaded++;

		if (pSprite->extra > 0)
		{
			uint16 nXSprite = dbInsertXSprite(i);
			XSPRITE *pXSprite = &xsprite[nXSprite];
			Bmemcpy(pXSprite, pMapData, sizeof(XSPRITE));
			pXSprite->reference = i;
			pMapData += sizeof(XSPRITE);
			xsprites_loaded++;
		}
	}

	// Sprite debugging
#ifdef _DEBUG
	FILE *spout = fopen("sprites.txt", "wt");
	if (spout)
	{
		SPRITE *pSprite = sprite;
		for (int i = 0; i < numsprites; i++)
		{
			if (pSprite->lotag >= kThingBase && pSprite->lotag < kThingMax)
			{
//				pSprite->xrepeat = 0;
//				pSprite->xrepeat = tilesizx[pSprite->picnum];
//				pSprite->yrepeat = tilesizy[pSprite->picnum];
				bool bDoIt = true;
				switch (pSprite->lotag)
				{
					case kDudeBase:
					case kDudeTommyCultist:
					case kDudeShotgunCultist:
					case kDudeAxeZombie:
					case kDudeEarthZombie:
					case kDudeBat:
					case kDudeRat:
						bDoIt = false;
						break;
				}
				if (bDoIt)
				{
					fprintf(spout,
						"Sprite %d\n"
						"    Position: %d, %d, %d\n"
						"    cstat:    %d\n"
						"    picnum:   %d\n"
						"    statnum:  %d\n"
						"    xrepeat:  %u\n"
						"    yrepeat:  %u\n"
						"    xoffset:  %d\n"
						"    yoffset:  %d\n"
						"    lotag:    %d\n"
						"    flags:    %d\n"
						"    extra:    %d\n"
						, i
						, pSprite->x, pSprite->y, pSprite->z
						, pSprite->cstat
						, pSprite->picnum
						, pSprite->statnum
						, pSprite->xrepeat
						, pSprite->yrepeat
						, pSprite->xoffset
						, pSprite->yoffset
						, pSprite->lotag
						, pSprite->hitag
						, pSprite->extra
					);
				}
			}
			pSprite++;
		}
		fflush(spout);
		fclose(spout);
	}
#endif

	for (int i = 0; i < numsectors; i++)
	{
		int curwall = sector[i].wallptr;
		for (int j = 0; j < sector[i].wallnum; j++)
		{
			int next = wall[curwall].point2;
			if (wall[curwall].x < xmin) xmin = wall[curwall].x;
			if (wall[curwall].x > xmax) xmax = wall[curwall].x;
			if (wall[curwall].y < ymin) ymin = wall[curwall].y;
			if (wall[curwall].y > ymax) ymax = wall[curwall].y;
			curwall = next;
		}
	}

	translate_x = -xmin;
	translate_y = -ymin;
	xmin += translate_x;
	xmax += translate_x;
	ymin += translate_y;
	ymax += translate_y;

	int32 biggest = xmax;
	if (ymax > biggest) biggest = ymax;

	// Scale it to something realistic
	int shift = 0;
	while (biggest > 4096)
	{
		biggest >>= 1;
		shift++;
	}

	int32 width  = xmax >> shift;
	int32 height = ymax >> shift;
	int32 size = width * height * 3;

	dprintf(
		"Map %s\n"
		"    Sectors:  %u\n"
		"    XSectors: %u\n"
		"    Walls:    %u\n"
		"    XWalls:   %u\n"
		"    Sprites:  %u\n"
		"    XSprites: %u\n"
		"    xmin:     %d\n"
		"    xmax:     %d\n"
		"    ymin:     %d\n"
		"    ymax:     %d\n"
		"    width:    %d\n"
		"    height:   %d\n"
		, map_name
		, sectors_loaded
		, xsectors_loaded
		, walls_loaded
		, xwalls_loaded
		, sprites_loaded
		, xsprites_loaded
		, xmin
		, xmax
		, ymin
		, ymax
		, width
		, height
	);

	uint8 *img = (uint8 *)malloc(size);
	memset(img, 0, size);
	for (int i = 0; i < numsectors; i++)
	{
		int curwall = sector[i].wallptr;
		for (int j = 0; j < sector[i].wallnum; j++)
		{
			int next = wall[curwall].point2;
			int x0 = (wall[curwall].x + translate_x) >> shift;
			int y0 = (wall[curwall].y + translate_y) >> shift;
			int x1 = (wall[next].x + translate_x) >> shift;
			int y1 = (wall[next].y + translate_y) >> shift;
			DrawLine(img, x0, y0, x1, y1, width, height);
			curwall = next;
		}
	}
	const SPRITE *pSprite = sprite;
	for (int i = 0; i < numsprites; i++)
	{
		int x = (pSprite->x + translate_x) >> shift;
		int y = (pSprite->y + translate_y) >> shift;
		if (x >= 0 && x < width && y >= 0 && y < height)
			img[(3*(x + y*width))+2] = 255;
		pSprite++;
	}
//	static void DrawLine(uint8 *base, int x0, int y0, int x1, int y1, int w, int h)
	FILE *fp = fopen("map.tga", "wb");
	if (fp)
	{
		const uint8 header[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		uint16 thew  = width;
		uint16 theh  = height;
		uint8  bits  = 24;
		uint8  flags = 32;
		fwrite(header, 12, 1, fp);
		fwrite(&thew, 2, 1, fp);
		fwrite(&theh, 2, 1, fp);
		fwrite(&bits, 1, 1, fp);
		fwrite(&flags, 1, 1, fp);
		fwrite(img, size, 1, fp);
		fclose(fp);
	}
	free(img);

	// Debugging
	// Create a map compatible with eduke32 (hopefully)
	char map_out[64];
	Bsnprintf(map_out, sizeof(map_out), "%s.map", map_name);
	fp = fopen(map_out, "wb");
	if (fp)
	{
		int32  out_version = 7;
		int32  out_x    = pHeader->posx;
		int32  out_y    = pHeader->posy;
		int32  out_z    = pHeader->posz;
		uint16 out_ang  = pHeader->ang;
		uint16 out_sect = pHeader->sector;
		uint16 out_numsect    = numsectors;
		uint16 out_numwall    = numwalls;
		uint16 out_numsprites = numsprites;

		fwrite(&out_version, sizeof(out_version), 1, fp);
		fwrite(&out_x, sizeof(out_x), 1, fp);
		fwrite(&out_y, sizeof(out_y), 1, fp);
		fwrite(&out_z, sizeof(out_z), 1, fp);
		fwrite(&out_ang, sizeof(out_ang), 1, fp);
		fwrite(&out_sect, sizeof(out_sect), 1, fp);
		fwrite(&out_numsect, sizeof(out_numsect), 1, fp);
		fwrite(sector, numsectors * sizeof(SECTOR), 1, fp);
		fwrite(&out_numwall, sizeof(out_numwall), 1, fp);
		fwrite(wall, numwalls * sizeof(WALL), 1, fp);
		fwrite(&out_numsprites, sizeof(out_numsprites), 1, fp);
		fwrite(sprite, numsprites * sizeof(SPRITE), 1, fp);
		fclose(fp);
	}

	dprintf("*** dbLoadMap() done\n");
}


void dbLoadExternalMap( const char *map, int *posx, int *posy, int *posz, short *ang, short *sectnum, unsigned int *seed )
{
	dprintf("dbLoadExternalMap(\"%s\")\n", map);
//	numsectors = 0;
//	numwalls = 0;
//	memset(sector, 0, sizeof(sector));
//	memset(wall, 0, sizeof(wall));
//	memset(sprite, 0, sizeof(sprite));

	FILE *fp = fopen(map, "rb");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		uint32 size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// We only load files 1MB or less
		if (size <= (1 * 1024 * 1024))
		{
			void *pBuffer = malloc(size);
			if (pBuffer == NULL)
				ThrowError("Failed allocating memory for map load");

			fread(pBuffer, size, 1, fp);
			dbLoadMemoryMap(pBuffer, posx, posy, posz, ang, sectnum, seed);
			free(pBuffer);
		}

		fclose(fp);
	}

#if 0
	FILE *fp = fopen(map, "rb");
	if (fp != NULL)
	{
		uint32 id;

		fread(&id, 4, 1, fp);
		fseek(fp, 0, SEEK_SET);
		if (id == 7)
		{
			dprintf("    Loading standard v7 BUILD map\n");

			// Normal version 7 map
			uint32 mapversion;
			int32  x, y, z;
			uint16 angle;
			uint16 sector;
			uint16 in_numsectors, in_numwalls;
			fread(&mapversion, sizeof(mapversion), 1, fp);
			fread(&x, sizeof(x), 1, fp);
			fread(&y, sizeof(y), 1, fp);
			fread(&z, sizeof(z), 1, fp);
			fread(&angle, sizeof(angle), 1, fp);
			fread(&sector, sizeof(sector), 1, fp);
			fread(&in_numsectors, sizeof(in_numsectors), 1, fp);
			numsectors = in_numsectors;
			fread(::sector, sizeof(SECTOR)*numsectors, 1, fp);
			fread(&in_numwalls, sizeof(in_numwalls), 1, fp);
			numwalls = in_numwalls;
			fread(wall, sizeof(WALL)*numwalls, 1, fp);
			initspritelists();

			*posx    = x;
			*posy    = y;
			*posz    = z;
			*ang     = angle;
			*sectnum = sector;
		}
		else if (id == BLM_SIG)
		{
			dprintf("    Loading Blood map\n");

			MAPHEADER header;
			fread(&header, sizeof(MAPHEADER), 1, fp);

			bool bEncrypted = false;
			switch (header.uVersion & 0xFF00)
			{
				case 1536: /* Beta version maps */ break;
				case 1792:
					// Retail version maps
					bEncrypted = true;	// Sigh
					break;

				default:
					// Unknown map
					// TODO: Throw errors etc.
					break;
			}

			// Note: I use the songid member of the structure
			// It appears this was used as a songid in the beta, but used
			// as a key for decryption now
			// Note also that a different decryption function is used here instead
			// of the same one used for the RFF stuff
			if (header.songid != 0 && header.songid != 0x7474614D && header.songid != 0x4D617474)
			{
				// Just decrypt the stuff from posx onwards
				AnotherCrypt(&header.posx, sizeof(MAPHEADER) - 6, 0x7474614D);
				// TODO: It sets some flag to true here
			}

			int numsprites = 0;

			*posx        = header.posx;
			*posy        = header.posy;
			*posz        = header.posz;
			*ang         = header.ang;
			*sectnum     = header.sector;
			pskybits     = header.skybits;
			visibility   = header.visibility;
			parallaxtype = header.parallaxtype;
			numsectors   = header.numsectors;
			numsprites   = header.numsprites;
			numwalls     = header.numwalls;

			// Clamp the number of sectors/walls/sprites
			if (numsectors > MAXSECTORS) numsectors = MAXSECTORS;
			if (numwalls   > MAXWALLS)   numwalls   = MAXWALLS;
			if (numsprites > MAXSPRITES) numsprites = MAXSPRITES;

			dbInit();

			dprintf(
				"    Sectors : %d (Limit %d)\n"
				"    Walls   : %d (Limit %d)\n"
				"    Sprites : %d (Limit %d)\n"
				"    pskybits: %d\n"
				"    sky size: %d\n"
				, numsectors, NUM_ELEMENTS(sector)
				, numwalls, NUM_ELEMENTS(wall)
				, numsprites, NUM_ELEMENTS(sprite)
				, pskybits
				, 2 * (1 << pskybits)
			);

			// If encrypted, we read another 128 bytes here and decrypt it
			uint8 unknown_bytes[128];
			if (bEncrypted)
			{
				fread(unknown_bytes, sizeof(unknown_bytes), 1, fp);
				// TODO: Add decryption
			}

			// We read the skybits here (I just skip it for now)
			int offs = ftell(fp);
			fseek(fp, offs + (2 * (1 << pskybits)), SEEK_SET);

			for (int i = 0; i < numsectors; i++)
			{
				SECTOR *pSector = &sector[i];
				fread(pSector, sizeof(SECTOR), 1, fp);
				if (bEncrypted)
					AnotherCrypt(pSector, sizeof(SECTOR), 40 * header.unknown1);

				if (pSector->extra > 0)
				{
					offs = ftell(fp);
					fseek(fp, offs + sizeof(XSECTOR), SEEK_SET);
				}
			}

			for (int i = 0; i < numwalls; i++)
			{
				WALL *pWall = &wall[i];
				fread(pWall, sizeof(WALL), 1, fp);
				if (bEncrypted)
					AnotherCrypt(pWall, sizeof(WALL), 40 * header.unknown1 | 0x7474614D);

				if (pWall->extra > 0)
				{
					offs = ftell(fp);
					fseek(fp, offs + sizeof(XWALL), SEEK_SET);
				}
			}

			initspritelists();
			for (int i = 0; i < numsprites; i++)
			{
				RemoveSpriteStat(i);

				SPRITE *pSprite = &sprite[i];
				fread(pSprite, sizeof(SPRITE), 1, fp);
				if (bEncrypted)
					AnotherCrypt(pSprite, sizeof(SPRITE), 44 * header.unknown1 | 0x7474614D);

				InsertSpriteSect(i, sprite[i].sectnum);
				InsertSpriteStat(i, sprite[i].statnum);

				if (pSprite->extra > 0)
				{
					offs = ftell(fp);
					fseek(fp, offs + sizeof(XSPRITE), SEEK_SET);
				}
			}
		}

		fclose(fp);
	}
#endif
}


extern "C" void dbInitSpriteLists()
{
	dprintf("*** dbInitSpriteLists()\n");
	for (int i = 0; i < (MAXSECTORS + 1); i++)
		headspritesect[i] = -1;
	for (int i = 0; i < (MAXSTATUS + 1); i++)
		headspritestat[i] = -1;
	for (int i = 0; i < MAXSPRITES; i++)
	{
		sprite[i].sectnum = -1;
		sprite[i].xvel    = -1;	// aka "index"
		InsertSpriteStat(i, 1024);
	}
}
