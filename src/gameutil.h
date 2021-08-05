#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

bool     FindSector( int x, int y, int z, int *nSector );
bool     FindSector( int x, int y, int *nSector );
unsigned ClipMove( int *px, int *py, int *pz, int *pnSector, int dx, int dy,
	int wallDist, int ceilDist, int floorDist, unsigned clipType );
void     RotateVector(int *xvec, int *yvec, int ang);
int      GetOctant(int xvec, int yvec);

#endif // __GAMEUTIL_H__
