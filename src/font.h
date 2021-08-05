#ifndef __FONT_H__
#define __FONT_H__


class CFont
{
	FT_Library m_ftLibrary;
	FT_Face    m_ftFace;

public:
	CFont(const char *pzFontFile);

	void DrawChar(int x, int y, char c, int col, int *nCharWidth = NULL);
	void DrawString(int x, int y, const char *str, int col);
};


extern CFont gDbgFont;


#endif // __FONT_H__
