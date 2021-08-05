#include "stdafx.h"
#include "debug.h"
#include "gfx.h"

#include "jfbuild/build.h"
#include "jfbuild/pragmas.h"
#include "jfbuild/baselayer.h"


Rect clipRect;

// These are unused now
//static int clipX0;
//static int clipY0;
//static int clipX1;
//static int clipY1;


void gfxSetClip(int x1, int y1, int x2, int y2)
{
	clipRect.x1 = x1;
	clipRect.y1 = y1;
	clipRect.x2 = x2;
	clipRect.y2 = y2;
//	clipX0 = a << 8;
//	clipY0 = b << 8;
//	clipX1 = (c << 8) - 1;
//	clipY1 = (d << 8) - 1;
}


void gfxSetClipToScreen()
{
	gfxSetClip(0, 0, xdim, ydim);
}


#define CLIP_INSIDE 0
#define CLIP_LEFT   1
#define CLIP_RIGHT  2
#define CLIP_BOTTOM 4
#define CLIP_TOP    8


// Returns true if the line crosses the screen, false otherwise
bool ClipLine(int *x1, int *y1, int *x2, int *y2)
{
	int ccode1 = CLIP_INSIDE, ccode2 = CLIP_INSIDE;

	// Compute code for first point
	if (*x1 <  clipRect.x1) ccode1 |= CLIP_LEFT;
	if (*x1 >= clipRect.x2) ccode1 |= CLIP_RIGHT;
	if (*y1 <  clipRect.y1) ccode1 |= CLIP_TOP;
	if (*y1 >= clipRect.y2) ccode1 |= CLIP_BOTTOM;

	// Compute code for second point
	if (*x2 <  clipRect.x1) ccode2 |= CLIP_LEFT;
	if (*x2 >= clipRect.x2) ccode2 |= CLIP_RIGHT;
	if (*y2 <  clipRect.y1) ccode2 |= CLIP_TOP;
	if (*y2 >= clipRect.y2) ccode2 |= CLIP_BOTTOM;

	// Accept it if both points are on screen
	if (!(ccode1 | ccode2)) return true;

	// Reject if both points are on the same outer sector
	if ((ccode1 & ccode2) != 0) return false;

	// Time for Liang-Barsky
	int t0 = 0, t1 = 65536, r;	// 16.16
	int xdelta = *x2 - *x1;
	int ydelta = *y2 - *y1;
	int p, q;

//	__asm int 3
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
			case 0: p = -xdelta; q = -(clipRect.x1 - *x1); break;
			case 1: p =  xdelta; q =  (clipRect.x2 - *x1 - 1); break;
			case 2: p = -ydelta; q = -(clipRect.y1 - *y1); break;
			case 3: p =  ydelta; q =  (clipRect.y2 - *y1 - 1); break;
		}

//		dprintf("edge %d, p = %d, q = %d\n", i, p, q);
		if (p == 0 && q < 0) return false;

		// To avoid division by zero
		if (p == 0)
			r = q << 16;
		else
			r = divscale16(q << 16, p << 16);
//		dprintf("r = %f\n", float(r) / 65536.0f);
		if (p < 0)
		{
			if (r > t1) return false;
			else if (r > t0) t0 = r;
		}
		else if (p > 0)
		{
			if (r < t0) return false;
			else if (r < t1) t1 = r;
		}
	}

	int bx1 = *x1;
	int by1 = *y1;
	*x1 = bx1 + (mulscale16(t0, xdelta << 16) >> 16);
	*y1 = by1 + (mulscale16(t0, ydelta << 16) >> 16);
	*x2 = bx1 + (mulscale16(t1, xdelta << 16) >> 16);
	*y2 = by1 + (mulscale16(t1, ydelta << 16) >> 16);

	return true;
}

#if 0
// Liang-Barsky function by Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
// This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to say whether the clipped line is drawn at all).
//
bool LiangBarsky (double edgeLeft, double edgeRight, double edgeBottom, double edgeTop,   // Define the x/y clipping values for the border.
                  double x0src, double y0src, double x1src, double y1src,                 // Define the start and end points of the line.
                  double &x0clip, double &y0clip, double &x1clip, double &y1clip)         // The output values, so declare these outside.
{

    double t0 = 0.0;    double t1 = 1.0;
    double xdelta = x1src-x0src;
    double ydelta = y1src-y0src;
    double p,q,r;

    for(int edge=0; edge<4; edge++) {   // Traverse through left, right, bottom, top edges.
        if (edge==0) {  p = -xdelta;    q = -(edgeLeft-x0src);  }
        if (edge==1) {  p = xdelta;     q =  (edgeRight-x0src); }
        if (edge==2) {  p = -ydelta;    q = -(edgeBottom-y0src);}
        if (edge==3) {  p = ydelta;     q =  (edgeTop-y0src);   }
        r = q/p;
        if(p==0 && q<0) return false;   // Don't draw line at all. (parallel line outside)

        if(p<0) {
            if(r>t1) return false;         // Don't draw line at all.
            else if(r>t0) t0=r;            // Line is clipped!
        } else if(p>0) {
            if(r<t0) return false;      // Don't draw line at all.
            else if(r<t1) t1=r;         // Line is clipped!
        }
    }

    x0clip = x0src + t0*xdelta;
    y0clip = y0src + t0*ydelta;
    x1clip = x0src + t1*xdelta;
    y1clip = y0src + t1*ydelta;

    return true;        // (clipped) line is drawn
}
#endif


void gfxDrawPixel(int x, int y, int col)
{
	if (x < 0 || x >= xdim || y < 0 || y >= ydim)
		return;

	begindrawing();	//{{{
	*((uint8 *)frameplace + ylookup[y] + x) = col;
	enddrawing();	//}}}
}


#define ABS(x)	(((x) < 0) ? -(x) : (x))
void gfxDrawLine(int x0, int y0, int x1, int y1, int col)
{
	int dx =  ABS(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -ABS(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	begindrawing();	//{{{
	for (;;) {
		// I hate this clipping check, but it still has a crash after
		// ClipLine() output
		if (x0 >= clipRect.x1 && x0 < clipRect.x2 && y0 >= clipRect.y1 && y0 < clipRect.y2)
			*((uint8 *)frameplace + ylookup[y0] + x0) = col;
		if (x0 == x1 && y0 == y1) break;
		int e2 = err << 1;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
	enddrawing();	//}}}
}


static void plot4(int cx, int cy, int x, int y, int col)
{
	*((uint8 *)frameplace + ylookup[cy + y] + (cx + x)) = col;
	if (x != 0) *((uint8 *)frameplace + ylookup[cy + y] + (cx - x)) = col;
	if (y != 0) *((uint8 *)frameplace + ylookup[cy - y] + (cx + x)) = col;
	if (x != 0 && y != 0) *((uint8 *)frameplace + ylookup[cy - y] + (cx - x)) = col;
}


static void plot8(int cx, int cy, int x, int y, int col)
{
	plot4(cx, cy, x, y, col);
	if (x != y)
		plot4(cx, cy, y, x, col);
}


void gfxDrawCircle(int cx, int cy, int radius, int col)
{
	int error = -radius;
	int x = radius;
	int y = 0;

	begindrawing();	//{{{
	while (x >= y)
	{
		plot8(cx, cy, x, y, col);

		error += y;
		y++;
		error += y;

		if (error >= 0)
		{
			error -= x;
			x--;
			error -= x;
		}
	}
	enddrawing();	//}}}
}
