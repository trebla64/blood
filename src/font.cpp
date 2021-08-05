#include "stdafx.h"
#include "font.h"
#include "gfx.h"

#include "jfbuild/build.h"
#include "jfbuild/baselayer.h"


CFont gDbgFont("PROFONT.FON");


CFont::CFont(const char *pzFontFile)
{
	FT_Init_FreeType(&m_ftLibrary);
	FT_New_Face(m_ftLibrary, pzFontFile, 0, &m_ftFace);
	FT_Set_Char_Size(m_ftFace, 0, 10*64, 0, 0);
}


void CFont::DrawChar(int x, int y, char c, int col, int *nCharWidth/* = NULL*/)
{
//	return;

	FT_Load_Glyph(m_ftFace, FT_Get_Char_Index(m_ftFace, c), 0);
	FT_Render_Glyph(m_ftFace->glyph, FT_RENDER_MODE_MONO);
	FT_Bitmap *bmp = &m_ftFace->glyph->bitmap;

	int w = bmp->width;
	int h = bmp->rows;

	// Is it fully outside
	if ((x + bmp->width) < clipRect.x1) return;
	if (x >= clipRect.x2) return;
	if ((y + bmp->rows) < clipRect.y1) return;
	if (y >= clipRect.y2) return;

	int xdist = clipRect.x2 - x;
	if (xdist < w) w = xdist;
	int ydist = clipRect.y2 - y;
	if (ydist < h) h = ydist;

	if (nCharWidth) *nCharWidth = w;

	uint8 *src = bmp->buffer;
	begindrawing();	//{{{
	uint8 *p = (uint8 *)frameplace + ylookup[y] + x;
	for (int j = 0; j < h; j++)
	{
		uint8 fetch = *src;
		for (int i = 0; i < w; i++)
			if (fetch & (1 << (7 - i)))
				p[i] = col;
		p += bytesperline;
		src += bmp->pitch;
	}
	enddrawing();	//}}}
}


void CFont::DrawString(int x, int y, const char *str, int col)
{
	int curx = x;
	for (const char *s = str; *s; s++)
	{
		int w;
		DrawChar(curx, y, *s, col, &w);
		curx += w;
	}
}
