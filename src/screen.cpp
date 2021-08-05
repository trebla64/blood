#include "stdafx.h"

#include "jfbuild/build.h"
#include "jfbuild/baselayer.h"
#include "jfbuild/a.h"
#include "jfbuild/compat.h"

#include "resource.h"
#include "game.h"
#include "screen.h"
#include "textio.h"
#include "gfx.h"
#include "error.h"
#include "debug.h"
#include "pal.h"


struct RGB
{
	uint8 r;
	uint8 g;
	uint8 b;
};
CT_ASSERT(sizeof(RGB) == 3);

//static byte *last_frameplace;
static int curPalette;
static int curGamma;
static bool DacInvalid = true;
static RGB baseDAC[256];
static bool gFogMode = false;
static uint8 *gammaTable = NULL;	//256*16 bytes

#define NUM_PALETTES 5

struct LOADITEM
{
	int index;
	const char *name;
};

static const RGB StdPal[NUM_STD_COLORS] =
{
	{   0,   0,   0 },	// 0
	{   0,   0, 170 },	// 1
	{   0, 170,   0 },	// 2
	{   0, 170, 170 },	// 3
	{ 170,   0,   0 },	// 4
	{ 170,   0, 170 },	// 5
	{ 170,  85,   0 },	// 6
	{ 170, 170, 170 },	// 7
	{  85,  85,  85 },	// 8
	{  85,  85, 255 },	// 9
	{  85, 255,  85 },	// 10
	{  85, 255, 255 },	// 11
	{ 255,  85,  85 },	// 12
	{ 255,  85, 255 },	// 13
	{ 255, 255,  85 },	// 14
	{ 255, 255, 255 },	// 15	// White
	{   0,   0,   0 },	// 16
	{   0,   0,   0 },	// 17
	{   0,   0,   0 },	// 18
	{   0,   0,   0 },	// 19
	{   0,   0,   0 },	// 20
	{   0,   0,   0 },	// 21
	{   0,   0,   0 },	// 22
	{   0,   0,   0 },	// 23
	{   0,   0,   0 },	// 24
	{   0,   0,   0 },	// 25
	{   0,   0,   0 },	// 26
	{   0,   0,   0 },	// 27
	{   0,   0,   0 },	// 28
	{   0,   0,   0 },	// 29
	{   0,   0,   0 },	// 30
	{   0,   0,   0 },	// 31
	{ 255,   0,   0 },	// 32	// Red
	{   0, 255,   0 },	// 33	// Green
	{   0,   0, 255 },	// 34	// Blue
};

uint8 gStdColor[NUM_STD_COLORS];

static const LOADITEM PLU[15] =
{
	{  0, "NORMAL"   },
	{  1, "SATURATE" },
	{  2, "BEAST"    },
	{  3, "TOMMY"    },
	{  4, "SPIDER3"  },
	{  5, "GRAY"     },
	{  6, "GRAYISH"  },
	{  7, "SPIDER1"  },
	{  8, "SPIDER2"  },
	{  9, "FLAME"    },
	{ 10, "COLD"     },
	{ 11, "P1"       },
	{ 12, "P2"       },
	{ 13, "P3"       },
	{ 14, "P4"       }
};

static const LOADITEM PAL[NUM_PALETTES] =
{
	{ 0, "BLOOD"   },
	{ 1, "WATER"   },
	{ 2, "BEAST"   },
	{ 3, "SEWER"   },
	{ 4, "INVULN1" }
};

static RGB *palTable[NUM_PALETTES];

uint8 scrFindClosestColor(uint8 r, uint8 g, uint8 b)
{
	unsigned dr, dg, db;	// Delta
	unsigned r2, g2, b2;	// Squared

	uint8 the_best = 0;
	unsigned best_dist = 2147483647;
	for (int i = 0; i < 256; i++)
	{
		int idx = 3 * i;
		// Check green
		uint8 pg = palette[idx + 1];
		if (pg >= g)
			dg = pg - g;
		else
			dg = g - pg;
		g2 = dg * dg;
		if (g2 >= best_dist)
			continue;
		// Check red
		uint8 pr = palette[idx];
		if (pr >= r)
			dr = pr - r;
		else
			dr = r - pr;
		r2 = dr * dr + g2;
		if (r2 >= best_dist)
			continue;
		// Check blue
		uint8 pb = palette[idx + 2];
		if (pb >= b)
			db = pb - b;
		else
			db = b - pb;
		b2 = db * db + r2;
		if (b2 >= best_dist)
			continue;
		best_dist = b2;
		the_best = i;
		if (b2)
			continue;
		break;
	}
	return the_best;
}


void scrCreateStdColors()
{
	dprintf("scrCreateStdColors()\n");
	for (int i = 0; i < NUM_STD_COLORS; i++)
	{
		gStdColor[i] = scrFindClosestColor(StdPal[i].r, StdPal[i].g, StdPal[i].b);
		int idx = gStdColor[i]*3;
		uint8 tor = palette[idx];
		uint8 tog = palette[idx+1];
		uint8 tob = palette[idx+2];
		dprintf("    Mapped (%u, %u, %u) -> (%u, %u, %u)\n", StdPal[i].r, StdPal[i].g, StdPal[i].b,
			tor, tog, tob);
	}
}


void scrLoadPLUs()
{
	dprintf("*** scrLoadPLUs()\n");
	if (gFogMode)
	{
		assert(0);
	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			ResHandle h = gSysRes.Lookup(PLU[i].name, "PLU");
			if (h.isNull())
				ThrowError("%s.PLU not found", PLU[i].name);
			dprintf("PLU %d, name %s got size %u\n", PLU[i].index, PLU[i].name, h.size());
			if (i == 0)
			{
				uint8 *p = (uint8 *)gSysRes.Lock(h);
				FILE *fp = fopen("NORMAL.PLU", "wb");
				if (fp)
				{
					fwrite(p, h.size(), 1, fp);
					fclose(fp);
				}
			}
			// Ugly stuff
#if 0
			int _size = h.size();
			int _out = 0;
			__asm {
				push eax
				push edx
				mov edx, _size
				mov eax, edx
				sar edx, 31
				shl edx, 8
				sbb eax, edx
				sar eax, 8
				mov _out, eax
				pop edx
				pop eax
			}
			if (_out != 64)
				ThrowError("Incorrect PLU size");
#endif
			palookup[PLU[i].index] = (uint8_t *)gSysRes.Lock(h);
		}
	}
}


// Need C linkage on this one because the engine calls it
extern "C" void scrLoadPalette()
{
	dprintf("*** Loading palettes\n");
	for (int i = 0; i < NUM_PALETTES; i++)
	{
		ResHandle h = gSysRes.Lookup(PAL[i].name, "PAL");
		if (h.isNull())
			ThrowError("%s.PAL not found", PAL[i].name);
		dprintf("    %d: %s.PAL got size %d\n", i, PAL[i].name, h.size());
		palTable[PAL[i].index] = (RGB *)gSysRes.Lock(h);
	}
	memcpy(palette, palTable[0], sizeof(palette));
	numpalookups = 64;
	paletteloaded = 1;
	scrLoadPLUs();
	dprintf("Loading translucency table\n");
	ResHandle h = gSysRes.Lookup("TRANS", "TLU");
	if (h.isNull())
		ThrowError("TRANS.TLU not found");
	transluc = (char *)gSysRes.Lock(h);
	fixtransluscence(FP_OFF(transluc));
}


void scrSetMessage(char *)
{
}


void scrDisplayMessage(int)
{
}


void scrSetGamma(int nGamma)
{
	dassert(nGamma < gGammaLevels);
	curGamma = nGamma;
	for (int i = 0; i < 256; i++)
	{
		const RGB *pPal = palTable[curPalette];
		const uint8 *gammatab = &gammaTable[256 * nGamma];
		baseDAC[i].r = gammatab[pPal[i].r];
		baseDAC[i].g = gammatab[pPal[i].g];
		baseDAC[i].b = gammatab[pPal[i].b];
	}
	DacInvalid = true;
}


void scrSetPalette(int nPalette)
{
	curPalette = nPalette;
	scrSetGamma(curGamma);
}


void scrSetupFade(unsigned char, unsigned char, unsigned char)
{
}


void scrSetupUnfade()
{
}


void scrFadeAmount(int)
{
}


// Set palette NOW!  And you better do it, otherwise I'm going to
// debug you!
void scrSetPaletteNOW(int nStart, int nCount, const uint8 *pPal)
{
	for (int i = nStart; i < (nStart + nCount); i++)
	{
		// We just screw with the engine palette directly
		curpalettefaded[i].r = curpalette[i].r = pPal[i*3+0];
		curpalettefaded[i].g = curpalette[i].g = pPal[i*3+1];
		curpalettefaded[i].b = curpalette[i].b = pPal[i*3+1];
		curpalettefaded[i].f = curpalette[i].f = 0;
	}
	setpalette(nStart, nCount, 0);	// It doesn't even care about last argument
}


static void PrepAndSendDAC()
{
	uint8 new_dac[768];

	const RGB *dac = baseDAC;
	for (int i = 0; i < 256; i++)
	{
		new_dac[i*3+0] = dac->r >> 2;
		new_dac[i*3+1] = dac->g >> 2;
		new_dac[i*3+2] = dac->b >> 2;
		dac++;
	}
	setbrightness(0, new_dac, 0);
}


// Done
void scrSetDac()
{
//	if (vidoption != 6)
//	{
		if (DacInvalid)
			PrepAndSendDAC();
//			scrSetPaletteNOW(0, 256, (const uint8 *)baseDAC);
//			setbrightness(0, (uint8 *)baseDAC, 0);
//			setbrightness(0, (char *)baseDAC, 4|2);	// Use this one for LSD mode lol
//			setbrightness(0, (char *)baseDAC, 0);	// Use this one for LSD mode lol
		DacInvalid = false;
//	}
}


void scrInit()
{
	dprintf( "Initializing engine\n" );
	tioPrint( "Initializing engine" );
	if (initengine())
		ThrowError("Failed initializing build engine: %s", engineerrstr);
	curPalette = 0;
	curGamma = 0;
	dprintf( "Loading gamma correction table\n" );
	tioPrint( "Loading gamma correction table" );
	ResHandle h = gSysRes.Lookup( "gamma", "DAT" );
	if ( h.isNull() )
		ThrowError( "Gamma table not found" );
	dprintf("*** Gamma table has size %d\n", h.size());

	// Ugly stuff
#if 0
	int _size = h.size();
	__asm {
		push eax
		push edx
		mov edx, _size
		mov eax, edx
		sar edx, 31
		shl edx, 8
		sbb eax, edx
		sar eax, 8
		mov gGammaLevels, eax
		pop edx
		pop eax
	}
#endif
	gGammaLevels = 16;
	dprintf("    %d gamma levels\n", gGammaLevels);
	gammaTable = (uint8 *)gSysRes.Lock(h);
}


void scrNextPage()
{
	nextpage();
//	if ( vidoption == 1 )
//		last_frameplace = frameplace;
}


void scrSetGameMode(int nMode, int nWidth, int nHeight)
{
	dprintf("*** setgamemode called\n");
	if (setgamemode(0, nWidth, nHeight, 8) < 0)
	{
		wm_msgbox("Error", "Failed setting video mode (%dx%d)", nWidth, nHeight);
	}
	gfxSetClip(0, 0, nWidth, nHeight);
	clearview(0);
	scrNextPage();
	scrSetPalette(curPalette);
}


void scrForcePalette()
{
	for (int i = 0; i < 256; i++)
	{
		curpalettefaded[i].r = curpalette[i].r = defaultPal[i*3+0];
		curpalettefaded[i].g = curpalette[i].g = defaultPal[i*3+1];
		curpalettefaded[i].b = curpalette[i].b = defaultPal[i*3+1];
		curpalettefaded[i].f = curpalette[i].f = 0;
	}
	setpalette(0, 256, 0);
}


long scrGetPalookup(int nVis, int nShade)
{
	long res = (nVis >> 8) + nShade;
	if (res < 0)  res = 0;
	if (res > 63) res = 63;
	return res;
}
