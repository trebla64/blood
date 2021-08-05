#include "stdafx.h"
#include "screen.h"
#include "error.h"
#include "debug.h"

#include "jfbuild/build.h"
#include "jfbuild/compat.h"
#include "jfbuild/baselayer.h"


#define kMaxTileFiles 256	//1024 / 4
#define kMaxTiles 512
//#define DUMP_TILES

struct CACHENODE
{
	int field_0;
	int field_4;
	int field_8;
	int lockCount;
};

static bool artLoaded = false;
static int nTileFiles;
static int nTotalTiles;
static FILE *hTileFile[kMaxTileFiles];
static int artversion;
static uint32 tileStart[8192];
static uint32 tileEnd[8192];
static short voxelIndex[MAXTILES];
static CACHENODE tileNode[MAXTILES];
//static int nLastTile;


static void CalcPicsiz(int nTile, int w, int h)
{
	int size = 0;
	for (int i = 2; i <= w; i += i) size++;
	for (int i = 2; i <= h; i += i) size += 16;
//	if (w >= 2)
//	{
//		for (int i = 2; i <= w; i *= 2)
//			size++;
//		if (h >= 2)
//			size += 16;
//	}
//	else
//	{
//		for (int i = 2; i <= h; i *= 2)
//			size += 16;
//	}
	picsiz[nTile] = size;
}


int tileInit( unsigned char do_version_check )
{
	dprintf("tileInit()\n");
	if (artLoaded)
		return nTileFiles;

	// Some checks for overflows
	dassert(sizeof(tilesizx) >= 8192);
	dassert(sizeof(tilesizy) >= 8192);
	dassert(sizeof(picanm) >= 16384);
	dassert(sizeof(gotpic) >= 512);
	dassert(sizeof(hTileFile) >= 1024);
	dassert(sizeof(voxoff) >= 10240);

	memset(tilesizx, 0, sizeof(tilesizx));		// 8192 bytes
	memset(tilesizy, 0, sizeof(tilesizy));		// 8192
	memset(picanm, 0, sizeof(picanm));			// 16384
	memset(gotpic, 0, sizeof(gotpic));			// 512
	memset(hTileFile, 0, sizeof(hTileFile));	// 1024
	memset(voxoff, 0, sizeof(voxoff));			// 10240
	memset(tilefilenum, 0, sizeof(tilefilenum));
	memset(tilefileoffs, 0, sizeof(tilefileoffs));

	nTileFiles  = 0;
	nTotalTiles = 0;

	int i = 0;
	for (;; i++)
	{
		char filename[128];
		Bsnprintf(filename, sizeof(filename), "TILES%03i.ART", i);
		dprintf("    Attepting to open \"%s\"\n", filename);
		FILE *fp = fopen(filename, "rb");
		if (fp == NULL)
			break;
		hTileFile[i] = fp;
		fread(&artversion, 4, 1, fp);
		if (do_version_check)
		{
			if (artversion != 69)
				ThrowError("Invalid .ART files");
			dassert(artversion == 69);
		}
		fread(&numtiles, 4, 1, fp);
		fread(&tileStart[i], 4, 1, fp);
		fread(&tileEnd[i], 4, 1, fp);
		int the_num = tileEnd[i] - tileStart[i] + 1;
		dprintf("    %d: start = %u, end = %u, num = %u\n", i, tileStart[i], tileEnd[i], the_num);
		fread(&tilesizx[tileStart[i]], 2 * the_num, 1, fp);
		fread(&tilesizy[tileStart[i]], 2 * the_num, 1, fp);
		fread(&picanm[tileStart[i]], 4 * the_num, 1, fp);
		nTotalTiles += the_num;

		uint32 offs = ftell(fp);
//		dprintf("Looping from %u to %u\n", tileStart[i], tileEnd[i]);
		for (uint32 j = tileStart[i]; j <= tileEnd[i]; j++)
		{
			tilefilenum[j] = i;
			tilefileoffs[j] = offs;
			uint32 size = tilesizx[j] * tilesizy[j];
			offs += size;
		}
	}
	nTileFiles = i;
	dprintf("    %d tile files opened, %d tiles loaded\n", nTileFiles, nTotalTiles);

	// We need to do MAXTILES here, because Blood has less space for tiles than JFBuild
	// Blood only has space for 4096 tiles
	for (int i = 0; i < MAXTILES; i++)	// XXX: Hardcode
	{
		CalcPicsiz(i, tilesizx[i], tilesizy[i]);
		waloff[i] = 0;
		voxelIndex[i] = -1;
	}
//	dprintf("Tile 0 has picsiz of %d\n", picsiz[0]);

	artLoaded = true;

	return nTileFiles;
}


void tileTerm()
{
//	dprintf("tileTerm()\n");
	for (int i = 0; i < kMaxTileFiles; i++)
		if (hTileFile[i] != NULL)
			fclose(hTileFile[i]);

	// Free all tiles that we loaded
	for (int i = 0; i < MAXTILES; i++)
		if (waloff[i] != 0)
			free((void *)waloff[i]);
}


extern "C" unsigned char *tileLoadTile(int nTile)
{
//	dprintf("Loading tile %d\n", nTile);

	dassert(artLoaded == true);
	dassert(nTile >= 0 && nTile < nTotalTiles);
	if (waloff[nTile] != 0)
		return (unsigned char *)waloff[nTile];

	unsigned size = tilesizx[nTile] * tilesizy[nTile];
	uint8 *tile_mem = (uint8 *)malloc(size);
	FILE *fp = hTileFile[tilefilenum[nTile]];
	fseek(fp, tilefileoffs[nTile], SEEK_SET);
	fread(tile_mem, size, 1, fp);
	waloff[nTile] = FP_OFF(tile_mem);
	return tile_mem;

#if 0
	if (!tileNode[nLastTile].lockCount)
		waloff[nLastTile] = 0;
	nLastTile = nTile;

	CACHENODE *pNode = &tileNode[nTile];
	if (pNode->field_0)
	{
		waloff[nTile] = pNode->field_0;
		if (pNode->lockCount)
			dprintf("loadtile() called for locked tile %d\n", nTile);
		else
		{
			// List linking :(
		}
		return (unsigned char *)waloff[nTile];
	}
	else
	{
		int size = tilesizx[nTile] * tilesizy[nTile];
		if (size > 0)
		{
			dassert(pNode->lockCount == 0);
			unsigned char *tile_mem = (unsigned char *)malloc(size);
			waloff[nTile] = FP_OFF(tile_mem);	// TODO: Use Resource::Alloc
			FILE *fp = hTileFile[tilefilenum[nTile]];
			fseek(fp, tilefileoffs[nTile], SEEK_SET);
			fread(tile_mem, size, 1, fp);
			return (unsigned char *)waloff[nTile];
		}
	}

	return NULL;
#endif
}


void tileBlit(int nTile, int x, int y, int nShade)
{
//	uint8 *org = (uint8 *)frameplace + ylookup[y] + x;
	uint8 *pPalookup = (uint8 *)((getpalookup(0, nShade) << 8) + palookup[0]);
	uint8 *src = (uint8 *)waloff[nTile];
	if (src == NULL)
		src = tileLoadTile(nTile);
	begindrawing();	//{{{
	for (int i = 0; i < tilesizx[nTile]; i++)
		for (int j = 0; j < tilesizy[nTile]; j++)
			*((uint8 *)frameplace + ylookup[j+y] + (i+x)) = pPalookup[*src++];
	enddrawing();	//}}}
}


extern "C" unsigned char *tileAllocTile(int nTile, int xsiz, int ysiz)
{
//	dprintf("*** tileAllocTile(%d, %d, %d)\n", nTile, xsiz, ysiz);
	return NULL;
}
