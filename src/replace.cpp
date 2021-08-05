#include "stdafx.h"
#include "game.h"
#include "debug.h"


extern uint32 CRC( uint32 count, const uint8 *buffer );


extern "C" int animateoffs(short nTile, uint16 nInfo)
{
	int index = 0, frames, clock;

//	__asm int 3

	dassert(nTile >= 0 && nTile < MAXTILES);

	int speed = uint32(picanm[nTile] << 4) >> 28;
	if ((nInfo & 0xC000) == 0x8000)	// Sprite
	{
		// Hash sprite frame using info variable
		clock = (gFrameClock + CRC(2, (uint8 *)&nInfo)) >> speed;
//		clock = gFrameClock >> speed;
	}
	else
	{
		clock = gFrameClock >> speed;
	}

	frames = picanm[nTile] & 0x1F;
	if (frames > 0)
	{
		int type = uint32(picanm[nTile] << 24) >> 30;
		switch (type)
		{
			case 1:	// Oscil
				index = clock % (frames * 2);
				if (index >= frames)
					index = frames * 2 - index;
				break;

			case 2:	// Forward
				index = clock % (frames + 1);
				break;

			case 3:	// Backward
				index = -(clock % (frames + 1));
				break;
		}
	}

	return index;
}
