#include "stdafx.h"
#include "game.h"

#include "multi.h"

#include "jfbuild/build.h"
#include "jfbuild/a.h"
#include "jfbuild/baselayer.h"
#include "jfbuild/pragmas.h"
#include "jfbuild/compat.h"

#include "typedefs.h"
#include "misc.h"
#include "gameutil.h"
#include "trig.h"
#include "view.h"
#include "globals.h"
#include "textio.h"
#include "names.h"
#include "db.h"
#include "debug.h"
#include "resource.h"
#include "error.h"
#include "player.h"
#include "screen.h"
#include "options.h"
#include "tile.h"
#include "levels.h"
#include "actor.h"
#include "sectorfx.h"
#include "mirrors.h"
#include "fire.h"
#include "weapon.h"
#include "map2d.h"
#include "gfx.h"
#include "gamemenu.h"

static bool gShowFrameRate = true;

static const int gPackIcons[9] =
{
	2569, 2564, 2565, 2566, 2568, 2561, 2560, 2562, 2563
};

//#define kStatBarHeight	25
//
///***********************************************************************
// * Variables
// **********************************************************************/
//int gViewIndex = 0;
//int gViewSize = 1;
//int gViewMode = kView3D;
//VIEWPOS gViewPos = kViewPosCenter;
//int gZoom = 1024;
int gViewSize = 1;
bool gOverlayMap;
static int gViewX0;
static int gViewY0;
static int gViewX1;
static int gViewY1;
static int gViewXCenter;
static int gViewYCenter;
static int xscale;
static int yscale;
static int xstep;
static int ystep;

static int deliriumTilt  = 0;
static int deliriumTurn  = 0;
static int deliriumPitch = 0;


//int gViewXCenter, gViewYCenter;
//static long messageTime = 0;
//static char message[256];
//
//static int pcBackground = 0;
//
//int gShowFrameRate = FALSE;
//int gShowFrags = FALSE;
//
//long gScreenTilt = 0;
//
//int deliriumTilt = 0;
//int deliriumTurn = 0;
//int deliriumPitch = 0;
//
//struct PLOCATION
//{
//	int x, y, z;
//	int bobHeight, bobWidth;
//	int swayHeight, swayWidth;
//	int horiz;
//	short ang;
//} gPrevPlayerLoc[kMaxPlayers];
//
//struct LOCATION
//{
//	int x, y, z;
//	short ang;
//} gPrevSpriteLoc[kMaxSprites];
//
//int gInterpolate;
//
//int gViewPosAngle[] =
//{
//	0,						// kViewPosCenter
//	kAngle180,				// kViewPosBack
//	kAngle180 - kAngle45,	// kViewPosLeftBack
//	kAngle90,				// kViewPosLeft
//	kAngle45,				// kViewPosLeftFront
//	0,						// kViewPosFront
//	-kAngle45,				// kViewPosRightFront
//	-kAngle90,				// kViewPosRight
//	-kAngle90 - kAngle45,	// kViewPosRightBack
//};
//
//
void viewDrawChar( QFONT *pFont, uint8 c, int nX, int nY, uint8 *pPalookup )
{
#if 0
	//******** Alpha code
	dassert(pFont != NULL);
	y += pFont->baseline;

	CHARINFO *pInfo = &pFont->info[c];
	int sizeX = pInfo->cols;
	int sizeY = pInfo->rows;
	int nSize = sizeX * sizeY;
	if ( nSize == 0 )
		return;

	Rect dest(x, y + pInfo->voffset, x + sizeX, y + sizeY + pInfo->voffset);
	Rect screen(0, 0, xdim, ydim);
	dest &= screen;

	if ( !dest )
		return;

	BYTE *pSource = &pFont->data[pInfo->offset];

	for (int i = 0; i < 4; i++)
	{
		palookupoffse[i] = pPalookup;
		vince[i] = 0x00010000;
	}

 	BYTE *bufplc = pSource;
	BYTE *p = frameplace + ylookup[dest.y0] + dest.x0;

	x = dest.x0;

	// dword align to video memory
	while (x < dest.x1 && (x & 3) )
	{
		mvlineasm1(0x00010000, pPalookup, sizeY - 1, 0, bufplc, p);
		bufplc += sizeY;
		p++;
		x++;
	}

	while ( x + 3 < dest.x1 )
	{
		for (i = 0; i < 4; i++)
		{
			bufplce[i] 	= bufplc;
			bufplc += sizeY;
			vplce[i] = 0;
		}
		mvlineasm4(sizeY, p);
		p += 4;
		x += 4;
	}

	while ( x < dest.x1 )
	{
		mvlineasm1(0x00010000, pPalookup, sizeY - 1, 0, bufplc, p);
		bufplc += sizeY;
		p++;
		x++;
	}
#endif
	CHARINFO *pInfo = &pFont->info[c];

	// Degenerate?
	if (pInfo->cols == 0 || pInfo->rows == 0)
		return;

	int sizeY = mulscale16(yscale, pInfo->rows);

	Rect dest;
	dest.x1 = mulscale16(xscale, nX);
	dest.x2 = dest.x1 + mulscale16(xscale, pInfo->cols);
	dest.y1 = mulscale16(yscale, nY + pFont->baseline + pInfo->voffset);
	dest.y2 = dest.y1 + sizeY;

//	int ta = mulscale16(x, xscale);
//	int tb = mulscale16(y, yscale);
//	int tc = mulscale16(pInfo->cols, xscale);
//	int td = mulscale16(pInfo->rows, yscale);

	const uint8 *pSource = (const uint8 *)&pFont->data[pInfo->offset];

//#if 0
	// This code doesn't work yet
//	for (int i = 0; i < 4; i++)
//	{
//		vince[i]         = ystep;
//		palookupoffse[i] = FP_OFF(pPalookup);
//	}

	uint8 *p = ylookup[dest.y1] + (uint8 *)frameplace + dest.x1;
	uint32 bufplc = 0;		// Type?

//	int x = dest.x0;
//	for (; x < dest.x1 && (x & 3); x++)
//	{
//		mvlineasm1(ystep, FP_OFF(pPalookup), sizeY - 1, 0, FP_OFF(&pSource[(bufplc >> 16) * pInfo->rows]), FP_OFF(p));
//		bufplc += xstep;
//		p++;
//	}
//	while ((x + 3) < dest.x1)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			vplce[i]   = 0;
//			bufplce[i] = FP_OFF(&pSource[(bufplc >> 16) * pInfo->cols]);
//			bufplc += xstep;
//		}
//		mvlineasm4(sizeY, FP_OFF(p));
//		p += 4;
//		x += 4;
//	}
	for (int x = dest.x1; x < dest.x2; x++)
	{
		mvlineasm1(ystep, FP_OFF(pPalookup), sizeY - 1, 0, FP_OFF(&pSource[(bufplc >> 16) * pInfo->rows]), FP_OFF(p));
		bufplc += xstep;
		p++;
	}
//#endif

#if 0
	for (int i = 0; i < pInfo->cols; i++)
	{
		uint8 *p = (uint8 *)frameplace + ylookup[pFont->baseline + nY + pInfo->voffset] + nX + i;
		for (int j = 0; j < pInfo->rows; j++)
		{
			if (*pSource != 255)
				*p = pPalookup[*pSource];
			pSource++;
			p += xdim;
		}
	}
#endif
}


//#define TA_LEFT   0
//#define TA_CENTER 1

void viewDrawText( int nFontId, const char *string, int nX, int nY, int shade, int nPLU, int nAlign )
{
#if 0
	//******** Alpha code
	char *s;
	dassert(string != NULL);

	RESHANDLE hFont = gSysRes.Lookup(nFontId, ".QFN");
	dassert(hFont != NULL);
	QFONT *pFont = (QFONT *)gSysRes.Lock(hFont);

	BYTE *pPalookup = palookup[nPLU] + (getpalookup(0, shade) << 8);
	setupmvlineasm(16);

	if ( nAlign != TA_LEFT )
	{
		int nWidth = -pFont->charSpace;
		for ( s = string; *s; s++ )
			nWidth += pFont->info[*s].hspace + pFont->charSpace;

		if ( nAlign == TA_CENTER )
			nWidth >>= 1;

		x -= nWidth;
	}

	for ( s = string; *s; s++ )
	{
		viewDrawChar(pFont, *s, x, y, pPalookup);
		x += pFont->info[*s].hspace + pFont->charSpace;
	}

	gSysRes.Unlock(hFont);
#endif
	const char *s;
	int x = nX;

	dassert(string != NULL);
	ResHandle hFont = gSysRes.Lookup(nFontId, "QFN");
	dassert(!hFont.isNull());
	QFONT *pFont = (QFONT *)gSysRes.Lock(hFont);

	uint8 *pPalookup = (uint8 *)((getpalookup(0, shade) << 8) + palookup[nPLU]);
	setupmvlineasm(16);

	if (nAlign != 0)	// TA_LEFT = 0, TA_CENTER = 1
	{
		int nWidth = -pFont->charSpace;
		for (s = string; *s; s++)
			nWidth += pFont->info[*s].hspace + pFont->charSpace;

		if (nAlign == 1)
			nWidth >>= 1;

		x -= nWidth;
	}

	begindrawing();	//{{{
	for (s = string; *s; s++)
	{
		viewDrawChar(pFont, *s, x, nY, pPalookup);
		x += pFont->info[*s].hspace + pFont->charSpace;
	}
	enddrawing();	//}}}

	gSysRes.Unlock(hFont);
}
//
//
//
//
//static void TileSprite( int nTile, int shade, int nPLU, int x0, int y0, int x1, int y1)
//{
//	long x, y, i;
//
//	Rect clip(x0, y0, x1, y1);
//	Rect screen(0, 0, xdim, ydim);
//	clip &= screen;
//
//	if ( !clip)
//		return;
//
//	dassert(nTile >= 0 && nTile < kMaxTiles);
//	int sizeX = tilesizx[nTile];
//	int sizeY = tilesizy[nTile];
//	int nSize = sizeX * sizeY;
//	if ( nSize == 0 )
//		return;
//
//	BYTE *palookupoffs = palookup[nPLU] + (getpalookup(0, shade) << 8);
//	BYTE *pTile = tileLoadTile(nTile);
//	BYTE *wrap = pTile + nSize;
//
//	setupvlineasm(16);
//
//	for (i = 0; i < 4; i++)
//	{
//		palookupoffse[i] = palookupoffs;
//		vince[i] = 0x00010000;
//	}
//
//	int yNext;
//	for (y = clip.y0; y < clip.y1; y = yNext)
//	{
//		yNext = ClipHigh(IncBy(y, sizeY), clip.y1);
//		x = clip.x0;
//
//		BYTE *bufplc = pTile + (x % sizeX) * sizeY + (y % sizeY);
//		BYTE *p = frameplace + ylookup[y] + x;
//
//		// dword align to video memory
//		while (x < clip.x1 && (x & 3) )
//		{
//			vlineasm1(0x00010000, palookupoffs, yNext - y - 1, 0, bufplc, p);
//			bufplc += sizeY;
//			if (bufplc >= wrap) bufplc -= nSize;
//			p++;
//			x++;
//		}
//
//		while ( x + 3 < clip.x1 )
//		{
//			for (i = 0; i < 4; i++)
//			{
//				bufplce[i] 	= bufplc;
//				bufplc += sizeY;
//				if (bufplc >= wrap) bufplc -= nSize;
//				vplce[i] = 0;
//			}
//			vlineasm4(yNext - y, p);
//			p += 4;
//			x += 4;
//		}
//
//		while ( x < clip.x1 )
//		{
//			vlineasm1(0x00010000, palookupoffs, yNext - y - 1, 0, bufplc, p);
//			bufplc += sizeY;
//			if (bufplc >= wrap) bufplc -= nSize;
//			p++;
//			x++;
//		}
//	}
//}
//
//
//static void ClipSprite( int x, int y, int nTile, int shade, int nPLU, int x0, int y0, int x1, int y1 )
//{
//	Rect clip(x0, y0, x1, y1);
//	Rect screen(0, 0, xdim, ydim);
//	clip &= screen;
//
//	if ( !clip)
//		return;
//
//	dassert(nTile >= 0 && nTile < kMaxTiles);
//	int sizeX = tilesizx[nTile];
//	int sizeY = tilesizy[nTile];
//	int nSize = sizeX * sizeY;
//	if ( nSize == 0 )
//		return;
//
//	Rect dest(x, y, x + sizeX, y + sizeY);
//	dest &= clip;
//
//	if ( !dest )
//		return;
//
//	Rect source(dest);
//	source.offset(-x, -y);
//
//	BYTE *palookupoffs = palookup[nPLU] + (getpalookup(0, shade) << 8);
//	BYTE *pTile = tileLoadTile(nTile);
//
//	setupvlineasm(16);
//
//	for (int i = 0; i < 4; i++)
//	{
//		palookupoffse[i] = palookupoffs;
//		vince[i] = 0x00010000;
//	}
//
//	BYTE *bufplc = pTile + (source.x0 % sizeX) * sizeY + (source.y0 % sizeY);
//	BYTE *p = frameplace + ylookup[dest.y0] + dest.x0;
//
//	x = dest.x0;
//
//	// dword align to video memory
//	while (x < dest.x1 && (x & 3) )
//	{
//		vlineasm1(0x00010000, palookupoffs, source.rows() - 1, 0, bufplc, p);
//		bufplc += sizeY;
//		p++;
//		x++;
//	}
//
//	while ( x + 3 < dest.x1 )
//	{
//		for (i = 0; i < 4; i++)
//		{
//			bufplce[i] 	= bufplc;
//			bufplc += sizeY;
//			vplce[i] = 0;
//		}
//		vlineasm4(source.rows(), p);
//		p += 4;
//		x += 4;
//	}
//
//	while ( x < dest.x1 )
//	{
//		vlineasm1(0x00010000, palookupoffs, source.rows() - 1, 0, bufplc, p);
//		bufplc += sizeY;
//		p++;
//		x++;
//	}
//}
//
//
//void InitStatusBar( void )
//{
//	tileLoadTile(kPicStatBar);
//}
//
//
//static void DrawStatSprite( int nTile, int x, int y, int nShade = 0, int nPLU = kPLUNormal )
//{
//	rotatesprite(x << 16, y << 16, 0x10000, 0, (short)nTile, (schar)nShade, (char)nPLU,
//		kRotateStatus | kRotateNoMask, 0, 0, xdim-1, ydim-1);
//}
//
//
//static void DrawStatMaskedSprite( int nTile, int x, int y, int nShade = 0, int nPLU = kPLUNormal )
//{
//	rotatesprite(x << 16, y << 16, 0x10000, 0, (short)nTile, (schar)nShade, (char)nPLU,
//		kRotateStatus, 0, 0, xdim-1, ydim-1);
//}
//
//
//static void DrawStatNumber( char *sFormat, int n, int nTile, int x, int y, int xSpace )
//{
//	char buffer[80];
//	sprintf(buffer, sFormat, n);
//	for (int i = 0; i < strlen(buffer); i++)
//	{
//		if ( buffer[i] != ' ' )
//			DrawStatSprite(nTile + buffer[i] - '0', x + i * xSpace, y);
//	}
//}
//
//
//static void TileHGauge( int nTile, int x, int y, int n, int total )
//{
//	int nGauge = n * tilesizx[nTile] / total;
//
//	ClipSprite(x, y, nTile, 0, 0, 0, 0, x + nGauge, ydim);
//}
//
//
//static void UpdateStatusBar(void)
//{
//	SPRITE *pSprite = gView->sprite;
//	XSPRITE *pXSprite = gView->xsprite;
//	int i;
//
//	if (gViewMode == kView2D || gViewSize > 0)		// status bar present?
//	{
//		DrawStatMaskedSprite(2200, 160, 178);	// status bar
//		DrawStatMaskedSprite(2234, 296, 179);	// heart
//
//		int nHealthTile = 2231;	// green
//		if ( pXSprite->health < (80 << 4) )
//			nHealthTile = 2232;	// yellow
//		if ( pXSprite->health < (40 << 4) )
//			nHealthTile = 2227;	// red
//
//		// show health amount
//		DrawStatNumber("%3d", pXSprite->health >> 4, 2250, 157, 182, 5);
//
//		TileHGauge(nHealthTile, 174, 180, pXSprite->health, 100 << 4);	// health meter
//		TileHGauge(2226, 221, 192, gView->bloodlust, 100 << 4);	// lust
//
//		// draw keys
//		for (i = 0; i < 6; i++)
//		{
//			if ( gView->hasKey[i+1] )
//				DrawStatSprite(2220 + i, 156 + i * 10, 193);
//			else
//				DrawStatSprite(2220 + i, 156 + i * 10, 193, 40, kPLUGrayish);
//		}
//
//		if ( gView->weapon != 0 )
//		{
//			DrawStatMaskedSprite(2229, 38 + gView->weapon * 13, 185);	// highlight
//
//			// show ammo count
//			if ( gView->weaponAmmo != kAmmoNone )
//				DrawStatNumber("%3d", gView->ammoCount[gView->weaponAmmo], 2190, 13, 180, 9);
//		}
//
//		// display level name
//		if ( gViewMode == kView2D )
//			gfxDrawText(0, 160, gStdColor[kColorWhite], gLevelDescription);
//	}
//
//	if (gCacheMiss)
//	{
//		DrawStatSprite(kPicDisk, xdim - 15, ydim - 15);
//		gCacheMiss = ClipLow(gCacheMiss - kFrameTicks, 0);
//	}
//}
//
//
//void viewInit(void)
//{
//	gViewXCenter = xdim / 2;
//	gViewYCenter = ydim / 2;
//
//	gViewSize 		= BloodINI.GetKeyInt("View", "Size", 1);
//
//	tioPrint("Initializing status bar");
//	InitStatusBar();
//}
//
//


static int ClipRange(int a, int lo, int hi)
{
	int res = a;
	if (res < lo) res = lo;
	if (res > hi) res = hi;
	return res;
}


void viewResizeView(int size)
{
	uint64 tmp;

	gViewXCenter = xdim / 2;
	gViewYCenter = ydim / 2;
	LODWORD(tmp) = xdim << 16;
	HIDWORD(tmp) = xdim >> 16;
	xscale = tmp / 320;
	LODWORD(tmp) = ydim << 16;
	HIDWORD(tmp) = ydim >> 16;
	yscale = tmp / 200;
	xstep = (320 << 16) / xdim;
	ystep = (200 << 16) / ydim;

	gViewSize = ClipRange(size, 0, 6);
	dprintf("viewResizeView: old = %d, new = %d\n", size, gViewSize);
	if (gViewSize > 1)
	{
		gViewX0 = 0;
		gViewY0 = 0;
		gViewX1 = xdim - 1;
		gViewY1 = ydim - 1 - 25 * ydim / 200;
	}
	else
	{
		// gViewSize <= 1
		gViewX0 = 0;
		gViewY0 = 0;
		gViewX1 = xdim - 1;
		gViewY1 = ydim - 1;
	}
	setview(gViewX0, gViewY0, gViewX1, gViewY1);
}
//
//
//#define kBackTile 230
//void viewDrawInterface( void )
//{
//	int x0, y0, x1, y1;
//
//	if (gViewMode == kView3D && gViewSize > 1)
//	{
//		if ( pcBackground )
//		{
//			x0 = gViewX0;
//			y0 = gViewY0;
//			x1 = gViewX1;
//			y1 = gViewY1;
//
//			TileSprite(kBackTile, 20, kPLUNormal, 0, 0, xdim, y0-3); 		// top
//			TileSprite(kBackTile, 20, kPLUNormal, 0, y1+4, xdim, ydim);	// bottom
//			TileSprite(kBackTile, 20, kPLUNormal, 0, y0-3, x0-3, y1+4);		// left
//			TileSprite(kBackTile, 20, kPLUNormal, x1+4, y0-3, xdim, y1+4);	// right
//
//			TileSprite(kBackTile, 40, kPLUNormal, x0-3, y0-3, x0, y1+1);		// left
//			TileSprite(kBackTile, 40, kPLUNormal, x0, y0-3, x1+4, y0); 		// top
//			TileSprite(kBackTile, 0, kPLUNormal, x1+1, y0, x1+4, y1+4);		// right
//			TileSprite(kBackTile, 0, kPLUNormal, x0-3, y1+1, x1+1, y1+4);		// bottom
//
//			pcBackground--;
//		}
//	}
//
//	UpdateStatusBar();
//
//	powerupDraw( gView );
//}
//
//
//#if 0
//short viewInsertTSprite( short nSector, short nStatus )
//{
//	short nTSprite = -1;
//
//	nTSprite = (short)spritesortcnt;
//	SPRITE *pTSprite = &tsprite[nTSprite];
//	memset(pTSprite, 0, sizeof(SPRITE));
//	pTSprite->type = kNothing;
//	pTSprite->sectnum = nSector;
//	pTSprite->statnum = nStatus;
//	pTSprite->cstat = kSpriteOriginAlign;
//	pTSprite->xrepeat = 64;
//	pTSprite->yrepeat = 64;
//	pTSprite->owner = -1;
//	pTSprite->extra = -1;
//	spritesortcnt++;
//
//	return nTSprite;
//}
//#endif
//
//static SPRITE *viewInsertTSprite( short nSector, short nStatus, SPRITE *pSource = NULL );
//
//static SPRITE *viewInsertTSprite( short nSector, short nStatus, SPRITE *pSource )
//{
//	short nTSprite = -1;
//
//	nTSprite = (short)spritesortcnt;
//	SPRITE *pTSprite = &tsprite[nTSprite];
//	memset(pTSprite, 0, sizeof(SPRITE));
//	pTSprite->type = (short)-spritesortcnt;			// negated temporary tsprite index - cleared after display
//	pTSprite->sectnum = nSector;
//	pTSprite->statnum = nStatus;
//	pTSprite->cstat = kSpriteOriginAlign;
//	pTSprite->xrepeat = 64;
//	pTSprite->yrepeat = 64;
//	pTSprite->owner = -1;
//	pTSprite->extra = -1;
//	spritesortcnt++;
//
//	if ( pSource != NULL )
//	{
//		pTSprite->x = pSource->x;
//		pTSprite->y = pSource->y;
//		pTSprite->z = pSource->z;
//		pTSprite->owner = pSource->owner;
//		pTSprite->ang = pSource->ang;
//	}
//
//	return &tsprite[nTSprite];
//}
//
//
//enum VIEW_EFFECT {
//	kViewEffectShadow = 0,
//	kViewEffectFlareHalo,
//	kViewEffectCeilGlow,
//	kViewEffectFloorGlow,
//	kViewEffectTorchHigh,
//	kViewEffectTorchLow,
//	kViewEffectSmokeHigh,
//	kViewEffectSmokeLow,
//	kViewEffectFlame,
//	kViewEffectSpear,
//	kViewEffectBloodSpray,
//	kViewEffectPhase,
//
//	kViewEffectMax,
//};
//
//int effectDetail[ kViewEffectMax ] =
//{
//	kDetailLevelMax,	// kViewEffectShadow
//	kDetailLevelMax,	// kViewEffectFlareHalo
//	kDetailLevelMax,	// kViewEffectCeilGlow,
//	kDetailLevelMax,	// kViewEffectFloorGlow,
//	kDetailLevelMin,	// kViewEffectTorchHigh
//	kDetailLevelMin,	// kViewEffectTorchLow
//	kDetailLevelMin,	// kViewEffectSmokeHigh
//	kDetailLevelMin,	// kViewEffectSmokeLow
//	kDetailLevelMin,	// kViewEffectFlame
//	kDetailLevelMin,	// kViewEffectSpear
//	kDetailLevel2,		// kViewEffectBloodSpray
//};
//
//
//static void viewAddEffect( int nTSprite, VIEW_EFFECT nViewEffect )
//{
//	dassert( nViewEffect >= 0 && nViewEffect < kViewEffectMax );
//	SPRITE *pTSprite = &tsprite[nTSprite];
//
//	if ( gDetail < effectDetail[nViewEffect] )	// skip effects at higher detail levels
//		return;
//
//	// can more tsprites be inserted?
//	if ( spritesortcnt < kMaxViewSprites )
//	{
//		switch (nViewEffect)
//		{
//			case kViewEffectPhase:
//			{
//				int nAngle = pTSprite->ang;
//
//				// remember: things like butcher knive sprites are already +kAngle90 because they are wall sprites
//				if (pTSprite->cstat & kSpriteWall)
//					 nAngle = (nAngle + kAngle90) & kAngleMask;
//				else
//					 nAngle = (nAngle + kAngle180) & kAngleMask;
//
//				for (int i=0; i<5 && spritesortcnt < kMaxViewSprites; i++)
//				{
//					short nSector;
//					SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF );
//					pTEffect->ang = pTSprite->ang;
//					pTEffect->x = pTSprite->x + mulscale30( (i+1) << 7, Cos(nAngle));
//					pTEffect->y = pTSprite->y + mulscale30( (i+1) << 7, Sin(nAngle));
//					pTEffect->z = pTSprite->z;
//			        FindSector(pTEffect->x, pTEffect->y, pTEffect->z, &nSector);
//					pTEffect->sectnum = nSector;
//					pTEffect->owner = pTSprite->owner;
//					pTEffect->picnum = pTSprite->picnum;
//					pTEffect->cstat = (uchar)(pTSprite->cstat & ~kSpriteBlocking);
//					pTEffect->cstat |= kSpriteTranslucent;
//					if (i < 2)
//						pTEffect->cstat |= kSpriteTranslucentR;
//					pTEffect->shade = (schar)ClipLow(pTSprite->shade - 16, -128);
//					pTEffect->xrepeat = pTSprite->xrepeat;
//					pTEffect->yrepeat = pTSprite->yrepeat;
//					pTEffect->picnum = pTSprite->picnum;
//				}
//				break;
//			}
//			case kViewEffectFlame:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				pTEffect->z = pTSprite->z;
//				pTEffect->cstat = (uchar)(pTSprite->cstat & ~kSpriteBlocking);
//				pTEffect->shade = kMaxShade;
//				pTEffect->xrepeat = pTEffect->yrepeat =
//					(uchar)(tilesizx[pTSprite->picnum] * pTSprite->xrepeat / 64);
//				pTEffect->picnum = kAnmFlame3;
//				pTEffect->statnum = kStatDefault; // show up in front of burning objects
//				break;
//			}
//
//			case kViewEffectSmokeHigh:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				int zTop, zBot;
//				GetSpriteExtents(pTSprite, &zTop, &zBot);
//				pTEffect->z = zTop;
//				if ( IsDudeSprite(pTSprite->owner)  )
//					pTEffect->picnum = kAnmSmoke1;
//				else
//					pTEffect->picnum = kAnmSmoke2;
//				pTEffect->cstat = (short)(pTSprite->cstat | kSpriteTranslucent & ~kSpriteBlocking);
//				pTEffect->shade = 8;
//				pTEffect->xrepeat = pTSprite->xrepeat;
//				pTEffect->yrepeat = pTSprite->yrepeat;
//				break;
//			}
//
//			case kViewEffectSmokeLow:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				int zTop, zBot;
//				GetSpriteExtents(pTSprite, &zTop, &zBot);
//				pTEffect->z = zBot;
//				if ( IsDudeSprite(pTSprite->owner)  )
//					pTEffect->picnum = kAnmSmoke1;
//				else
//					pTEffect->picnum = kAnmSmoke2;
//				pTEffect->cstat = (short)(pTSprite->cstat | kSpriteTranslucent & ~kSpriteBlocking);
//				pTEffect->shade = 8;
//				pTEffect->xrepeat = pTSprite->xrepeat;
//				pTEffect->yrepeat = pTSprite->yrepeat;
//				break;
//			}
//
//			case kViewEffectTorchHigh:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				int zTop, zBot;
//				GetSpriteExtents(pTSprite, &zTop, &zBot);
//				pTEffect->z = zTop;
//				pTEffect->picnum = kAnmFlame2;
//				pTEffect->cstat = (short)(pTSprite->cstat & ~kSpriteBlocking);
//				pTEffect->shade = kMaxShade;
//				pTEffect->xrepeat = pTEffect->yrepeat =
//					(uchar)(tilesizx[pTSprite->picnum] * pTSprite->xrepeat / 32);
//				break;
//			}
//
//			case kViewEffectTorchLow:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				int zTop, zBot;
//				GetSpriteExtents(pTSprite, &zTop, &zBot);
//				pTEffect->z = zBot;
//				pTEffect->picnum = kAnmFlame2;
//				pTEffect->cstat = (short)(pTSprite->cstat & ~kSpriteBlocking);
//				pTEffect->shade = kMaxShade;
//				pTEffect->xrepeat = pTEffect->yrepeat =
//					(uchar)(tilesizx[pTSprite->picnum] * pTSprite->xrepeat / 32);
//				break;
//			}
//
//			case kViewEffectShadow:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				// insert a shadow
//				pTEffect->z = getflorzofslope(pTSprite->sectnum, pTEffect->x, pTEffect->y);
//
//				pTEffect->cstat = (uchar)(pTSprite->cstat | kSpriteTranslucent);
//				pTEffect->shade = kMinShade;
//				pTEffect->xrepeat = pTSprite->xrepeat;
//				pTEffect->yrepeat = (uchar)(pTSprite->yrepeat >> 2);
//				pTEffect->picnum = pTSprite->picnum;
//				int nTile = pTEffect->picnum;
//
//				// position it so it's based on the floor
//				pTEffect->z -= (tilesizy[nTile] - (tilesizy[nTile] / 2 + picanm[nTile].ycenter)) * (pTEffect->yrepeat << 2);
//				break;
//			}
//
//			case kViewEffectFlareHalo:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				// insert a lens flare halo effect around flare missiles
//				pTEffect->z = pTSprite->z;
//				pTEffect->cstat = (uchar)(pTSprite->cstat | kSpriteTranslucent);
//				pTEffect->shade = kMaxShade;
//				pTEffect->pal = kPLURed;
//				pTEffect->xrepeat = pTSprite->xrepeat;
//				pTEffect->yrepeat = pTSprite->yrepeat;
//				pTEffect->picnum = kAnmFlareHalo;
//				break;
//			}
//
//			case kViewEffectCeilGlow:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				SECTOR *pSector = &sector[pTSprite->sectnum];
//				int zDist = (pTSprite->z - pSector->ceilingz) >> 8;
//				pTEffect->x = pTSprite->x;
//				pTEffect->y = pTSprite->y;
//				pTEffect->z = pSector->ceilingz;
//				pTEffect->cstat = (uchar)(pTSprite->cstat | kSpriteTranslucent | kSpriteFloor | kSpriteOneSided | kSpriteFlipY);
//				pTEffect->shade = (schar)(-64 + zDist);
//				pTEffect->pal = kPLURed;
//				pTEffect->xrepeat = 64;
//				pTEffect->yrepeat = 64;
//				pTEffect->picnum = kAnmGlowSpot1;
//				pTEffect->ang = pTSprite->ang;
//				pTEffect->owner = pTSprite->owner;	//(short)nTSprite;
//				break;
//			}
//
//			case kViewEffectFloorGlow:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				SECTOR *pSector = &sector[pTSprite->sectnum];
//				int zDist = (pSector->floorz - pTSprite->z) >> 8;
//				pTEffect->x = pTSprite->x;
//				pTEffect->y = pTSprite->y;
//				pTEffect->z = pSector->floorz;
//				pTEffect->cstat = (uchar)(pTSprite->cstat | kSpriteTranslucent | kSpriteFloor | kSpriteOneSided);
//				pTEffect->shade = (schar)(-32 + zDist);
//				pTEffect->pal = kPLURed;
//				pTEffect->xrepeat = (uchar)zDist;
//				pTEffect->yrepeat = (uchar)zDist;
//				pTEffect->picnum = kAnmGlowSpot1;
//				pTEffect->ang = pTSprite->ang;
//				pTEffect->owner = pTSprite->owner;	//(short)nTSprite;
//				break;
//			}
//
//			case kViewEffectSpear:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				pTEffect->z = pTSprite->z;
//				pTEffect->cstat = (uchar)((pTSprite->cstat & ~kSpriteRMask) | kSpriteFloor);
//				pTEffect->shade = pTSprite->shade;
//				pTEffect->xrepeat = pTSprite->xrepeat;
//				pTEffect->yrepeat = pTSprite->yrepeat;
//				pTEffect->picnum = pTSprite->picnum;
//				pTSprite->ang += kAngle180;
//				pTSprite->ang &= kAngleMask;
//				break;
//			}
//
//			case kViewEffectBloodSpray:
//			{
//				SPRITE *pTEffect = viewInsertTSprite( pTSprite->sectnum, 0x7FFF, pTSprite);
//				pTEffect->z = pTSprite->z;
//				pTEffect->cstat = (uchar)(pTSprite->cstat & ~kSpriteBlocking);
//				if (gDetail > kDetailLevel2)
//					pTEffect->cstat |= kSpriteTranslucent | kSpriteTranslucentR;
//				pTEffect->shade = (schar)ClipLow((int)(pTSprite->shade - 32), -128);
//				pTEffect->xrepeat = pTSprite->xrepeat;
//				pTEffect->yrepeat = 64;
//				pTEffect->picnum = kAnmBloodSpray;
//				break;
//			}
//		}
//	}
//}


void viewProcessSprites( int nViewX, int nViewY, int nViewZ )
{
	dassert(spritesortcnt < MAXSPRITESONSCREEN);
	for (int nTSprite = spritesortcnt; nTSprite >= 0; nTSprite--)
	{
		SPRITE *pTSprite = &tsprite[nTSprite];
		int nXSprite = pTSprite->extra;
		XSPRITE *pTXSprite = NULL;
		if (sprite[pTSprite->owner].filler <= gDetail)
		{
			if (nXSprite > 0)
				pTXSprite = &xsprite[nXSprite];

			int nTile = pTSprite->picnum;
			if (nTile < 0 || nTile >= MAXTILES)
			{
				// This was in beta for debugging, probably not required, but meh
				dprintf(
					"tsprite[].cstat   = %d\n"
					"tsprite[].shade   = %d\n"
					"tsprite[].pal     = %d\n"
					"tsprite[].picnum  = %d\n"
					"tsprite[].ang     = %d\n"
					"tsprite[].owner   = %d\n"
					"tsprite[].sectnum = %d\n"
					"tsprite[].statnum = %d\n"
					"tsprite[].type    = %d\n"
					"tsprite[].flags   = %d\n"
					"tsprite[].extra   = %d\n"
					, pTSprite->cstat
					, pTSprite->shade
					, pTSprite->pal
					, pTSprite->picnum
					, pTSprite->ang
					, pTSprite->owner
					, pTSprite->sectnum
					, pTSprite->statnum
					, pTSprite->type
					, pTSprite->flags
					, pTSprite->extra
				);
				dassert(nTile >= 0 && nTile < MAXTILES);
			}

//			if (gViewInterpolate)
			{
				// TODO: Add interpolation stuff here
			}

			int nTileAdd = 0;

//			uint32 nAnim = ((uint32)picanm[nTile] >> 28) & 7;	// Correct?
			switch ((uint32(picanm[nTile])>>28)&7)
			{
				case 1:	// TODO: kSpriteView5Full
				{
					int dx = nViewX - pTSprite->x;
					int dy = nViewY - pTSprite->y;
					RotateVector(&dx, &dy, -pTSprite->ang + 128);
					int nOctant = GetOctant(dx, dy);
					if (nOctant > 4)
					{
						nOctant = 8 - nOctant;
						pTSprite->cstat |= 4;
					}
					else
					{
						pTSprite->cstat &= ~4;
					}
					pTSprite->picnum += nOctant;

					break;
				}

				case 2:	// TODO: kSpriteView8Full
				{
					int dx = nViewX - pTSprite->x;
					int dy = nViewY - pTSprite->y;
					RotateVector(&dx, &dy, -pTSprite->ang + 128);
					pTSprite->picnum += GetOctant(dx, dy);
					break;
				}

				case 6:
				case 7:
					if (gDetail >= 4)
					{
						if (!(pTSprite->flags & 0x10))
						{
							pTSprite->cstat |= 0x30;
//							pTSprite->picnum = voxelIndex[pTSprite->picnum];
							if ((uint32(picanm[nTile]) & 0x70000000) == 1879048192)
								pTSprite->ang = (8 * gGameClock) & 2047;
						}
					}
					break;
			}

//			while (nTileAdd > 0)
//			{
//				nTileAdd--;
//				pTSprite->picnum += (picanm[nTile] & 0x1F) + 1;
//			}
		}

		// Check for stacked sectors (hack)
		switch (pTSprite->type)
		{
			case 11:
			{
				int nFloorPic = sector[pTSprite->sectnum].floorpicnum;
				gotpic[nFloorPic>>3] |= 1 << (nFloorPic & 7);
//				uint8 *showpic = (uint8 *)&gotpic[sector[pTSprite->sectnum].floorpicnum >> 3];
//				*showpic |= 1 << (sector[pTSprite->sectnum].floorpicnum & 7);
				break;
			}

			case 12:
			{
				int nCeilPic = sector[pTSprite->sectnum].ceilingpicnum;
				gotpic[nCeilPic>>3] |= 1 << (nCeilPic & 7);
//				uint8 *showpic = (uint8 *)&gotpic[sector[pTSprite->sectnum].ceilingpicnum >> 3];
//				*showpic |= 1 << (sector[pTSprite->sectnum].ceilingpicnum & 7);
				break;
			}
		}
	}
}


//#define kViewDistance	(80 << 4)
//extern void viewCalcPosition( SPRITE *pSprite, long *px, long *py, long *pz, short *pAngle, short *pSector )
//{
//	int nAngle = (*pAngle + gViewPosAngle[gViewPos]) & kAngleMask;
//	int dx = mulscale30(kViewDistance, Cos(nAngle));
//	int dy = mulscale30(kViewDistance, Sin(nAngle));
//
//	*pAngle = (short)((nAngle + kAngle180) & kAngleMask);
//
//	ushort cstat = pSprite->cstat;
//	pSprite->cstat &= ~kSpriteBlocking;
//	ClipMove(px, py, pz, pSector, dx, dy, 4 << 4, 1, 1, 0);
//	pSprite->cstat = cstat;
//}
//
//
//void viewBackupPlayerLoc( int nPlayer )
//{
//	SPRITE *pSprite = gPlayer[nPlayer].sprite;
//	PLOCATION *pPLocation = &gPrevPlayerLoc[nPlayer];
//	pPLocation->x = pSprite->x;
//	pPLocation->y = pSprite->y;
//	pPLocation->z = pSprite->z;
//	pPLocation->ang = pSprite->ang;
//	pPLocation->horiz = gPlayer[nPlayer].horiz;
//	pPLocation->bobHeight = gPlayer[nPlayer].bobHeight;
//	pPLocation->bobWidth = gPlayer[nPlayer].bobWidth;
//	pPLocation->swayHeight = gPlayer[nPlayer].swayHeight;
//	pPLocation->swayWidth = gPlayer[nPlayer].swayWidth;
//}
//
//
//void viewBackupSpriteLoc( int nSprite, SPRITE *pSprite )
//{
//	LOCATION *pLocation = &gPrevSpriteLoc[nSprite];
//	pLocation->x = pSprite->x;
//	pLocation->y = pSprite->y;
//	pLocation->z = pSprite->z;
//	pLocation->ang = pSprite->ang;
//}
//
//
//void viewBackupAllSpriteLoc( void )
//{
//	SPRITE *pSprite = &sprite[0];
//	LOCATION *pLocation = &gPrevSpriteLoc[0];
//	for (int i = 0; i < kMaxSprites; i++, pSprite++, pLocation++)
//	{
//		if ( pSprite->flags & kAttrMove )
//		{
//			pLocation->x = pSprite->x;
//			pLocation->y = pSprite->y;
//			pLocation->z = pSprite->z;
//			pLocation->ang = pSprite->ang;
//		}
//	}
//}
//
//
///***********************************************************************
// * viewDrawSprite()
// *
// * Draws tiles to the screen using a unified set of flags
// * similar to those in overwritesprite/rotatesprite
// **********************************************************************/
//void viewDrawSprite( long sx, long sy,
//	long nZoom, int nAngle, short nTile, schar nShade, char nPLU, ushort nFlags,
//	long wx1, long wy1, long wx2, long wy2 )
//{
//	// convert x-flipping
//	if ( nFlags & kDrawXFlip )
//	{
//		nAngle = (nAngle + kAngle180) & kAngleMask;
//		nFlags ^= kDrawYFlip;
//	}
//
//	// call rotatesprite passing only compatible bits in nFlags
//	rotatesprite( sx, sy, nZoom, (short)nAngle, nTile, nShade, nPLU,
//		(char)nFlags, wx1, wy1, wx2, wy2 );
//}
//
//#define kMaxViewFlames	9
//
//static struct {
//	short	tile;
//	uchar	flags;
//	char	pal;
//	long	zoom;
//	short	x;
//	short	y;
//} burnTable[ kMaxViewFlames ] = {
//	{	kAnmFlame2,	(uchar)(kDrawScale),			0,	0x22000,	10,		200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale|kDrawXFlip),	0,	0x20000,	40,		200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale),			0,	0x19000,	85,		200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale),			0,	0x16000,	120,	200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale|kDrawXFlip),	0,	0x14000,	160,	200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale|kDrawXFlip),	0,	0x17000,	200,	200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale),			0,	0x18000,	235,	200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale),			0,	0x20000,	275,	200 },
//	{	kAnmFlame2,	(uchar)(kDrawScale|kDrawXFlip),	0,	0x23000,	310,	200 },
//};
//
//
///***********************************************************************
// * viewBurnTime()
// **********************************************************************/
//void viewBurnTime( int nTicks )
//{
//	if ( nTicks == 0 )
//		return;
//
//	for ( int i = 0; i < kMaxViewFlames; i++ )
//	{
//		// fake a sprite type for animateoffs to derive an index into an animation sequence
//		short nTile = burnTable[i].tile;
//		nTile += animateoffs( nTile, (ushort)(0x8000 + i));
//		long nZoom = burnTable[i].zoom;
//
//		// size diminishes during last 5 seconds
//		if ( nTicks < 5 * kTimerRate )
//			nZoom = muldiv(nZoom, nTicks, 5 * kTimerRate);
//
//		viewDrawSprite( burnTable[i].x << 16, burnTable[i].y << 16, nZoom, 0, nTile, 0,
//			burnTable[i].pal, burnTable[i].flags, windowx1, windowy1, windowx2, windowy2);
//	}
//}
//
//
//void viewSetMessage( char *s )
//{
//	messageTime = gFrameClock;
//	strcpy(message, s);
//}
//
//
//void viewDisplayMessage( void )
//{
//
//	if (gFrameClock < messageTime + kTimerRate * 2)
//		viewDrawText(kFontMessage, message, windowx1 + 1, windowy1 + 1, 0, 0);
//}


#if 0
static int index = 0;
static int ticks[64];

void CalcFrameRate()
{
	if ( gFrameClock != ticks[index] )
	{
		int div = gFrameClock - ticks[index];
		if ( div )
			(bleh) = 7680 / div;
		ticks[index] = gFrameClock;
	}
	index = (index + 1) & 0x3F;
}
#endif


static void CalcInterpolations()
{
}


static void UpdateFrame()
{
//	viewTileSprite(230, 0, 0, 0, 0, xdim, gViewY0 - 3);
}


static void DrawStatMaskedSprite(int nTile, int x, int y, int nShade, int nPLU)
{
	rotatesprite(x << 16, y << 16, 65536, 0, nTile, nShade, nPLU, 0xA, 0, 0, xdim - 1, ydim - 1);
}


static void DrawStatNumber(const char *sFormat, int n, int nTile, int x, int y, int xSpace)
{
	char buffer[64];
	Bsnprintf(buffer, sizeof(buffer), sFormat, n);
	int i = 0;
	int cur_x = x;
	int v11 = tilesizx[nTile] + 1;
	while (1)
	{
		const char *v8 = buffer;
		int v7 = -1;
		bool v9;
		do
		{
			if (!v7) break;
			v9 = *v8++ == 0;
			v7--;
		}
		while (!v9);
		if (i >= ~v7 - 1)
			break;
		if (buffer[i] != 32)
			DrawStatMaskedSprite(nTile + (uint8)buffer[i] - 48, cur_x, y, xSpace, 0);
		i++;
		cur_x += v11;
	}
}


static void DrawStatSprite(int nTile, int x, int y, int nShade, int nPLU)
{
	rotatesprite(x << 16, y << 16, 65536, 0, nTile, nShade, nPLU, 0x4A, 0, 0, xdim - 1, ydim - 1);
}


static void viewDrawInterface()
{
	if (gViewMode == 3 && gViewSize > 2/* && pcBackground*/)
	{
		UpdateFrame();
//		pcBackground--;
	}

	if (gViewMode == 2 || gViewSize >= 0)
	{
		if (gViewSize == 1)
		{
			DrawStatSprite(2201, 34, 187, 16, 0);	// Left side background
			DrawStatNumber("%3d", 999, 2190, 8, 183, 0);	// Player health
			DrawStatNumber("%3d", 999, 2240, 42, 183, 0);	// Player ammo

			DrawStatSprite(2173, 284, 187, 16, 0);	// Right side background
			DrawStatNumber("%3d", 999, 2230, 255, 178, 0);	// What is this?
			DrawStatNumber("%3d", 999, 2230, 255, 186, 0);	// What is this?
			DrawStatNumber("%3d", 999, 2230, 255, 194, 0);	// What is this?

			DrawStatSprite(gPackIcons[0], 286, 186, 0, 0);	// Currently active item
			DrawStatNumber("%3d", 999, 2250, 302, 183, 4);	// Amount left on active item
		}
		else
		{
			if (gViewSize > 1)
			{
				DrawStatMaskedSprite(2200, 160, 172, 0, 0);	// TODO: Fixed 2nd last arg?
			}
		}
	}
}


void viewDrawScreen( void )
{
	static int lastUpdate = 0;

	int nUpdateDelta = gGameClock - lastUpdate;
	if (nUpdateDelta < 0)
		nUpdateDelta = 0;
	lastUpdate = gGameClock;

	if ( !gPaused && (!CGameMenuMgr::m_bActive || gGameOptions.nGameType) )
	{
		// TODO: Clean this up
		int64 big;
		int nDelta = gGameClock - gNetFifoClock + 4;
		*(uint32 *)&big     = nDelta << 16;
		*((uint32 *)&big+1) = nDelta >> 16;
		gInterpolate = big >> 2;
	}

	if (gInterpolate < 0)     gInterpolate = 0;
	if (gInterpolate > 65536) gInterpolate = 65536;

	if (gViewInterpolate)
		CalcInterpolations();

	if (gViewMode == 3 || gViewMode == 4 || gOverlayMap)
		DoSectorLighting();

	if (gViewMode == 3 || gOverlayMap)
	{
		int posx   = gPlayerTemp.x/*gView->pSprite->x*/;
		int posy   = gPlayerTemp.y/*gView->pSprite->y*/;
		int posz   = gPlayerTemp.z/*gView->player_z*/;
		int sector = gPlayerTemp.sector/*gView->pSprite->sectnum*/;
		int ang    = gPlayerTemp.ang/*gView->pSprite->ang*/;

		if (gViewInterpolate)
		{
			// TODO: Add multiplayer interpolation code here
			if (0)
			{
				VIEW *pView = &gPrevView[gViewIndex];

				posx = pView->x + mulscale16(posx - pView->x, gInterpolate);
				posy = pView->y + mulscale16(posy - pView->y, gInterpolate);
				posz = pView->another_z + mulscale16(posz - pView->another_z, gInterpolate);
				ang  = pView->ang + mulscale16(((ang - pView->ang + 1024) & 2047) - 1024, gInterpolate);
			}
		}

		if (gViewPos)
		{
			FindSector(posx, posy, posz, &sector);
		}

		if (true)
		{
			deliriumTilt  = 0;
			deliriumTurn  = 0;
			deliriumPitch = 0;
		}

		DrawMirrors(posx, posy, posz, ang, deliriumPitch + 0 + 90);

		if (gViewPos)
			gView->pSprite->cstat |= 0x0202;
		else
			gView->pSprite->cstat |= 0x8000;

		rposx  = posx;
		rposy  = posy;
		rposz  = posz;
		rang   = ang;
		rsector = sector;
		drawrooms(posx, posy, posz, ang, deliriumPitch + 0 + 90, sector);
//		drawrooms(posx, posy, posz, ang, 90, sector);
		viewProcessSprites(posx, posy, posz);
//		viewProcessSprites(posx, posy, posz);
		drawmasks();

		WeaponDraw(gView, ::sector[gPlayerTemp.sector].floorshade, 160, 210);
	}
	else
	{
		clearview(0);
	}

	viewDrawInterface();
}
//
//
//void Detail( int nType, BOOL bDetail )
//{
//	switch ( nType )
//	{
//		case kDetailSprites:
//		{
//			int nSprite, nNext;
//			for (nSprite = headspritestat[kStatProximity]; nSprite >= 0; nSprite = nNext)
//			{
//				nNext = nextspritestat[nSprite];
//
//				if (bDetail)
//					sprite[nSprite].cstat &= ~kSpriteInvisible;
//				else
//					sprite[nSprite].cstat |= kSpriteInvisible;
//			}
//			break;
//		}
//
//		default:
//			break;
//	}
//}
//
//
//void viewSetDetail( int nDetail )
//{
//	dassert( nDetail >= kDetailLevelMin && nDetail <= kDetailLevelMax );
//
//	switch ( nDetail )
//	{
//		// no kStatDetail sprites, no fake gibs, no shadows, no smoke trail effects, no glow or lens flares
//		case kDetailLevelMin:
//			Detail( kDetailSprites, FALSE );
//			break;
//
//		case kDetailLevel2:
//			Detail( kDetailSprites, FALSE );
//			break;
//
//		case kDetailLevel3:
//			Detail( kDetailSprites, FALSE );
//			break;
//
//		case kDetailLevel4:
//			Detail( kDetailSprites, FALSE );
//			break;
//
//		case kDetailLevelMax:
//			Detail( kDetailSprites, TRUE );
//			break;
//	}
//	gDetail = nDetail;
//}


void viewToggle(int nViewMode)
{
	switch (nViewMode)
	{
		case 2: gViewMode = 4; break;
		case 3: gViewMode = 2; break;
		default:
			gViewMode = 3;
			viewResizeView(gViewSize);
			break;
	}
}


void viewUpdatePrediction(NEW_INPUT *pInput)
{
	memcpy(&predictOld, &predict, sizeof(VIEW));

//	if (pInput->forward) dprintf("forward: %d\n", pInput->forward);
//	if (pInput->strafe)  dprintf("strafe:  %d\n", pInput->strafe);
//	if (pInput->turn)    dprintf("turn:    %d\n", pInput->turn);

	int32 sinval = costable[(gPlayerTemp.ang - 512) & 2047];
	int32 cosval = costable[gPlayerTemp.ang & 2047];

	int dx = mulscale30(pInput->forward, cosval);
	int dy = mulscale30(pInput->forward, sinval);

	if (pInput->turn != 0)
		predict.ang = (predict.ang + (kFrameTicks * pInput->turn >> 4)) & 2047;

	unsigned ccode = ClipMove(&gPlayerTemp.x, &gPlayerTemp.y, &gPlayerTemp.z, &gPlayerTemp.sector, dx, dy,
		128, 0, 0, 0x10001);

	// Check our sector
	FindSector(gPlayerTemp.x, gPlayerTemp.y, gPlayerTemp.z, &gPlayerTemp.sector);
	gPlayerTemp.z = getflorzofslope(gPlayerTemp.sector, gPlayerTemp.x, gPlayerTemp.y) - (5632+4608)/*(16 << 8)*/;
//	updatesectorz(gPlayerTemp.x, gPlayerTemp.y, gPlayerTemp.z, (short *)&gPlayerTemp.sector);

//	int z = getflorzofslope(gPlayerTemp.sector, gPlayerTemp.x, gPlayerTemp.y);
//	gPlayerTemp.z = z - (512 << 4);

	memcpy(&predictFifo[gPredictTail & 0xFF], &predict, sizeof(VIEW));
	gPredictTail++;
}


void viewClearInterpolations()
{
	nInterpolations = 0;
	memset(gInterpolateSprite, 0, sizeof(gInterpolateSprite));	// 512 bytes
	memset(gInterpolateWall, 0, sizeof(gInterpolateWall));		// 1024 bytes
	memset(gInterpolateSector, 0, sizeof(gInterpolateSector));	// 128 bytes
}


void viewAddInterpolation(void *pBuffer, int nType)
{
	if (nInterpolations == NUM_ELEMENTS(gInterpolation))
		ThrowError("Too many interpolations");

	INTERPOLATE *pInterpolation = &gInterpolation[nInterpolations];
	nInterpolations++;

	pInterpolation->ptr  = pBuffer;
	pInterpolation->type = nType;

	switch (nType)
	{
		case 0: pInterpolation->start_value = *(uint16 *)pBuffer; break;
		case 1: pInterpolation->start_value = *(uint32 *)pBuffer; break;
	}
}


void viewInitializePrediction()
{
	predict.ang      = gMe->pSprite->ang;
	predict.field_20 = gMe->field_73;
	predict.field_24 = gMe->field_77;
	predict.field_28 = gMe->field_7B;
	predict.field_67 = gMe->field_2B5;
	predict.field_68 = gMe->field_2A;
	predict.x        = gMe->pSprite->x;
	predict.y        = gMe->pSprite->y;
	predict.z        = gMe->pSprite->z;
	predict.sectnum  = gMe->pSprite->sectnum;
	predict.flags    = gMe->pSprite->hitag;	// TODO: Replace with flags

	int index = gMe->pSprite->xvel;	// TODO: Replace with index
	predict.xvel     = xvel[index];
	predict.yvel     = yvel[index];
	predict.zvel     = zvel[index];

//	predict.field_62 = *(_DWORD *)&gMe->pXSprite->unknown0 & 0xFFFF;
	predict.nSprite  = gMe->nSprite;
//	predict.field_66 = *(_DWORD *)&gMe->field_8[8] << 23 >> 31;
//	v1 = &gSpriteHit[3 * gMe->pSprite->extra];
//	predict.ccode = *v1;
//	++v1;
//	predict.field_6F = *v1;
//	predict.field_73 = v1[1];
	predict.field_0   = gMe->field_33;
	predict.field_4   = gMe->field_37;
	predict.field_8   = gMe->field_3B;
	predict.field_C   = gMe->field_3F;
	predict.field_10  = gMe->field_43;
	predict.field_14  = gMe->field_47;
	predict.field_18  = gMe->field_4B;
	predict.field_1C  = gMe->field_4F;
	predict.field_30  = gMe->field_6B - gMe->player_z - 3072;
	predict.another_z = gMe->player_z;
	predict.field_38  = gMe->field_67;
	predict.field_3C  = gMe->field_6B;
	predict.field_40  = gMe->field_6F;
	memcpy(&predictOld, &predict, sizeof(VIEW));
}


void viewBackupView(int nView)
{
//	dprintf("viewBackupView(%d)\n", nView);
	PLAYER *pPlayer = &gPlayer[nView];
	VIEW *pView = &gPrevView[nView];

	pView->ang       = pPlayer->pSprite->ang;
	pView->x         = pPlayer->pSprite->x;
	pView->y         = pPlayer->pSprite->y;
	pView->another_z = pPlayer->player_z;
	pView->field_30  = pPlayer->field_6B - pPlayer->player_z - 3072;
	pView->field_24  = pPlayer->field_77;
	pView->field_28  = pPlayer->field_7B;
	pView->field_8   = pPlayer->field_3B;
	pView->field_C   = pPlayer->field_3F;
	pView->field_18  = pPlayer->field_4B;
	pView->field_1C  = pPlayer->field_4F;
}


void viewCorrectPrediction()
{
}


void viewUpdatePredictionTest(NEW_INPUT *pInput, uint32 delta_ticks)
{
//	dprintf("fDelta = %f\n", fDelta);
	int32 dx = 0, dy = 0;
	int32 sinval, cosval;

	if (pInput->forward != 0)
	{
		sinval = costable[(gPlayerTemp.ang - 512) & 2047];
		cosval = costable[gPlayerTemp.ang & 2047];

//		dx = int(32.0f * fDelta * float(pInput->forward) * cosval);
		dx = mulscale30(4 * delta_ticks * pInput->forward, cosval);
//		dy = int(32.0f * fDelta * float(pInput->forward) * sinval);
		dy = mulscale30(4 * delta_ticks * pInput->forward, sinval);
	}

	if (pInput->strafe != 0)
	{
		sinval = costable[(gPlayerTemp.ang - 1024) & 2047];
		cosval = costable[(gPlayerTemp.ang - 512) & 2047];

//		dx += int(32.0f * fDelta * float(pInput->strafe) * cosval);
		dx += mulscale30(4 * delta_ticks * pInput->strafe, cosval);
//		dy += int(32.0f * fDelta * float(pInput->strafe) * sinval);
		dy += mulscale30(4 * delta_ticks * pInput->strafe, sinval);
	}

	unsigned ccode = 0;
	if (dx != 0 || dy != 0)
	{
		ccode = ClipMove(&gPlayerTemp.x, &gPlayerTemp.y, &gPlayerTemp.z, &gPlayerTemp.sector, dx, dy, 128, 0, 0, 0x10001);
	}

	if (pInput->turn)
	{
		gPlayerTemp.ang = (gPlayerTemp.ang + (delta_ticks * pInput->turn)) & 2047;
	}

//	FindSector(gPlayerTemp.x, gPlayerTemp.y, gPlayerTemp.z, &gPlayerTemp.sector);
	gPlayerTemp.z = getflorzofslope(gPlayerTemp.sector, gPlayerTemp.x, gPlayerTemp.y) - (5632+4608)/*(16 << 8)*/;
}
