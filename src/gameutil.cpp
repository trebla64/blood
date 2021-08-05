#include "stdafx.h"
#include "typedefs.h"
//#include "engine.h"
#include "gameutil.h"
#include "globals.h"
#include "debug.h"
#include "palette.h"
#include "misc.h"
#include "tile.h"
#include "trig.h"

#include "jfbuild/build.h"


bool FindSector( int x, int y, int z, int *nSector )
{
	int ceilz, florz;

//	dprintf("nSector = %p, *nSector = %d\n", nSector, *nSector);

	dassert(*nSector >= 0 && *nSector < MAXSECTORS);
	if (inside(x, y, *nSector))
	{
		getzsofslope(*nSector, x, y, &ceilz, &florz);
		if (z >= ceilz && z <= florz)
			return true;
	}

	// Check the adjoining sectors
	WALL *pWall = &wall[sector[*nSector].wallptr];
	for (int i = 0; i < sector[*nSector].wallnum; i++)
	{
		int nextsector = pWall->nextsector;
		if (nextsector >= 0 && inside(x, y, nextsector))
		{
			getzsofslope(nextsector, x, y, &ceilz, &florz);
			if (z >= ceilz && z <= florz)
			{
				*nSector = nextsector;
				return true;
			}
		}
		pWall++;
	}

	// Check all sectors
	for (int i = 0; i < numsectors; i++)
		if (inside(x, y, i))
		{
			getzsofslope(i, x, y, &ceilz, &florz);
			if (z >= ceilz && z <= florz)
			{
				*nSector = i;
				return true;
			}
		}

	return false;
}


unsigned ClipMove( int *px, int *py, int *pz, int *pnSector, int dx, int dy,
	int wallDist, int ceilDist, int floorDist, unsigned clipType )
{
//	return clipmove(px, py, pz, pnSector, dx << 14, dy << 14,
//		wallDist, ceilDist, floorDist, clipType);
	int x = *px;
	int y = *py;
	int z = *pz;
	int nSector = *pnSector;

	unsigned ccode = clipmove((int32_t *)px, (int32_t *)py, (int32_t *)pz, (short *)pnSector, dx << 14, dy << 14,
		wallDist, ceilDist, floorDist, clipType);

	// force temporary fix to ken's inside() bug
	if ( *pnSector == -1)
	{
		// return to last known good location
		*px = x;
		*py = y;
		*pz = z;
		*pnSector = nSector;
	}
	return ccode;
}


#ifdef _MSC_VER

/**
 *
 */
__declspec(naked) void __cdecl RotateVector(int *pdx, int *pdy, int ang)
{
	__asm {
					// ret|pdx|pdy|ang
		push eax	// eax|ret|pdx|pdy|ang
		push edx	// edx|eax|ret|pdx|pdy|ang
		push ebx	// ebx|edx|eax|ret|pdx|pdy|ang
		push ecx	// ecx|ebx|edx|eax|ret|pdx|pdy|ang
		push ebp	// ebp|ecx|ebx|edx|eax|ret|pdx|pdy|ang
		push esi	// esi|ebp|ecx|ebx|edx|eax|ret|pdx|pdy|ang
		push edi	// edi|esi|ebp|ecx|ebx|edx|eax|ret|pdx|pdy|ang

		// Normalize the arguments first for __fastcall
		//    eax = pdx
		//    edx = pdy
		//    ebx = ang
		mov eax, [esp + 32]
		mov edx, [esp + 36]
		mov ebx, [esp + 40]

		// We have 4 variables on the local stack
		// -16 = dy
		// -12 = sinval
		// -8  = pdy
		// -4  = pdx
		mov [esp - 4], eax
		mov [esp - 8], edx
		mov ebp, [eax]	// ebp = dx for remainder

		mov eax, [edx]
		mov [esp - 16], eax	// dy on local stack
		lea eax, [ebx - 512]
		mov esi, [esp - 16]
		and eax, 2047
		and ebx, 2047
		neg esi
		mov eax, costable[eax*4]
		mov ecx, costable[ebx*4]
		mov [esp - 12], eax

		mov edi, eax
		mov ebx, ecx
		mov eax, ebp
		imul ebx
		xchg eax, esi
		mov ebx, edx
		imul edi
		add eax, esi
		adc edx, ebx
		add eax, 0x20000000
		adc edx, 0
		shrd eax, edx, 30

		mov ebx, [esp - 4]
		mov esi, [esp - 16]
		mov edi, ecx
		mov [ebx], eax

		mov ebx, [esp - 12]
		mov eax, ebp
		imul ebx
		xchg eax, esi
		mov ebx, edx
		imul edi
		add eax, esi
		adc edx, ebx
		add eax, 0x20000000
		adc edx, 0
		shrd eax, edx, 30
		mov ebx, [esp - 8]
		mov [ebx], eax

		pop edi
		pop esi
		pop ebp
		pop ecx
		pop ebx
		pop edx
		pop eax

		ret
	}
}


/*

    \   7 | 0  /
      \   |  /
      6 \ |/  1
    ------+------
      5  /|\  2
       /  |  \
     /  4 | 3  \

*/
static const int OctantTable[8] = { 5, 6, 2, 1, 4, 7, 3, 0 };

__declspec(naked) int __cdecl GetOctant(int dx, int dy)
{
	__asm {
		push edx	// edx|ret|dx|dy
		push ebx	// ebx|edx|ret|dx|dy
		push ecx	// ecx|ebx|edx|ret|dx|dy

		mov eax, [esp + 16]
		mov edx, [esp + 20]

		mov ebx, eax
		test eax, eax
		jns label0
		neg eax

label0:
		mov ecx, eax
		mov eax, edx
		test eax, eax
		jns label1
		neg eax

label1:
		sub ecx, eax
		mov eax, ebx
		add eax, eax
		sbb eax, eax
		mov ebx, eax
		mov eax, edx
		add eax, eax
		sbb eax, eax
		add ebx, 7
		add eax, eax
		add ebx, eax
		mov eax, ecx
		add eax, eax
		sbb eax, eax
		lea eax, [eax*4]
		add eax, ebx
		mov eax, OctantTable[eax*4]

		pop ecx
		pop ebx
		pop edx

		ret
	}
}

#else

void RotateVector(int *pdx, int *pdy, int ang)	{}
int  GetOctant(int dx, int dy)			{ return 0; }

#endif
