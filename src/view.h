#ifndef __VIEW_H__
#define __VIEW_H__

enum ViewMode
{
	kView3D = 3
};

#pragma pack(push, 1)

struct CHARINFO
{
	uint32 offset;
	uint8  cols;
	uint8  rows;
	uint8  hspace;
	char   voffset;
};
CT_ASSERT(sizeof(CHARINFO) == 8);

struct QFONT
{
	uint8    filler1[15];
	uint8    baseline;
	uint8    foo;
	uint8    charSpace;
	uint8    sizeX;
	uint8    sizeY;
	uint8    filler2[12];
	CHARINFO info[256];
	uint8    data[1];
};
CT_ASSERT(sizeof(QFONT) == (17+1+14+(8*256)+1));

#pragma pack(pop)


extern int gViewSize;

struct NEW_INPUT;

void viewInit();
void viewDrawScreen();
void viewResizeView(int a);
void viewDrawText( int nFontId, const char *string, int x, int y, int shade, int nPLU, int nAlign );
void viewDrawChar( QFONT *pFont, uint8 c, int x, int y, uint8 *pPalookup );
void viewUpdatePrediction(NEW_INPUT *pInput);
void viewClearInterpolations();
void viewAddInterpolation(void *pBuffer, int nType);
void viewInitializePrediction();
void viewBackupView(int nView);
void viewCorrectPrediction();
void viewUpdatePredictionTest(NEW_INPUT *gInput, uint32 delta_ticks);
void viewProcessSprites( int nViewX, int nViewY, int nViewZ );


#endif // __VIEW_H__
