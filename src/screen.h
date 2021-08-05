#ifndef __SCREEN_H__
#define __SCREEN_H__

#define NUM_STD_COLORS 35

extern uint8 gStdColor[NUM_STD_COLORS];

// Some named constants for commonly used standard colors
enum StandardColors
{
	kColWhite = 15,
	kColRed   = 32,
	kColGreen = 33,
	kColBlue  = 34
};

uint8 scrFindClosestColor(uint8, uint8, uint8);
void  scrCreateStdColors();
void  scrLoadPLUs();
void  scrSetMessage(char *);
void  scrDisplayMessage(int);
void  scrSetPalette(int);
void  scrSetGamma(int nGamma);
void  scrSetupFade(unsigned char, unsigned char, unsigned char);
void  scrSetupUnfade();
void  scrFadeAmount(int);
void  scrSetDac();
void  scrInit();
void  scrSetGameMode(int, int, int);
void  scrNextPage();
void  scrForcePalette();
void  scrSetPaletteNOW(int nStart, int nCount, const uint8 *pPal);

// Functions that the engine needs access to
#ifdef __cplusplus
extern "C" {
#endif

void scrLoadPalette();
long scrGetPalookup(int nVis, int nShade);

#ifdef __cplusplus
}
#endif

#endif // __SCREEN_H__
