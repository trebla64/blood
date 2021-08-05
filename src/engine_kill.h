#ifndef __ENGINE_H__
#define __ENGINE_H__

// Constants
#define kMaxSectors 1024
#define kMaxWalls   8192
#define kMaxSprites 4096
#define kMaxStatus  1024

// Types
struct SPRITE
{
	int x, y, z;
	ushort cstat;
	sshort picnum;
	schar shade;
	uchar pal, clipdist;
	uchar filler;
	uchar xrepeat, yrepeat;
	schar xoffset, yoffset;
	short sectnum, statnum;
	short ang;
	short owner;
	short xvel, yvel, zvel;
	short type;
	ushort flags;
	short extra;
};

struct SECTOR
{
	short wallptr;
	short wallnum;
	long ceilingz;
	long floorz;
	ushort ceilingstat;
	ushort floorstat;
	sshort ceilingpicnum;
	sshort ceilingslope;
	schar ceilingshade;
	uchar ceilingpal;
	uchar ceilingxpanning, ceilingypanning;
	sshort floorpicnum;
	sshort floorslope;
	schar floorshade;
	uchar floorpal;
	uchar floorxpanning, floorypanning;
	uchar visibility;
	uchar filler;
	ushort type;
	short hitag;
	short extra;
};

struct WALL
{
	long x, y;
	short point2;			// right vertex of wall
	short nextwall;			// wall on other side (-1 if single sided)
	short nextsector;      // sector on other side (-1 if single sided)
	ushort cstat;
	short picnum, overpicnum;
	schar shade;
	uchar pal, xrepeat, yrepeat, xpanning, ypanning;
	ushort type;
	short hitag, extra;
};

#ifdef __cplusplus
extern "C" {
#endif

// Engine globals
extern int   vidoption;
extern byte *frameplace;

extern SPRITE sprite[kMaxSprites];
extern SECTOR sector[kMaxSectors];
extern WALL   wall[kMaxWalls];

// Functions
void initengine();
void uninitengine();
void nextpage();
void clearview(int col);
void initspritelists();

#ifdef __cplusplus
}
#endif

#endif // __ENGINE_H__
