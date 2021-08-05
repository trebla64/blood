#ifndef __GFX_H__
#define __GFX_H__

// Note, the right and bottom points are not included!!
struct Rect
{
	int x1;
	int y1;
	int x2;
	int y2;

	int Width() const 	{ return x2 - x1; }
	int Height() const	{ return y2 - y1; }
};

extern Rect clipRect;

void gfxSetClip(int a, int b, int c, int d);
void gfxSetClipToScreen();
bool ClipLine(int *x1, int *y1, int *x2, int *y2);
void gfxDrawPixel(int x, int y, int col);
void gfxDrawLine(int x0, int y0, int x1, int y1, int col);
void gfxDrawCircle(int cx, int cy, int radius, int col);

#endif // __GFX_H__
