#include "stdafx.h"
#include "game.h"
#include "qav.h"
#include "debug.h"


void QAV::Draw(int a, int b, int c)
{
	dassert(pData->ticksPerFrame > 0);
	int nFrame = a / pData->ticksPerFrame;
	dassert(nFrame >= 0 && nFrame < pData->nFrames);

	QAVFRAME *pFrame = &pData->aFrames[nFrame];
	for (int i = 0; i < 8; i++)
	{
		QAVINNERFRAME *pInnerFrame = &pFrame->aInnerFrames[i];
		// Don't draw if no picnum
		if (pInnerFrame->nPic <= 0)
			continue;

		int nStat  = pInnerFrame->field_10 | c;
		int nAngle = pInnerFrame->field_16;
		if (nStat & 0x100)
		{
			nAngle = (nAngle + 1024) & 2047;
			nStat ^= 4;
		}

		int nShade = b + pInnerFrame->field_14;
		if (nShade > 127)  nShade = 127;
		if (nShade < -128) nShade = -128;

		// Remove this
		continue;

		rotatesprite(
			(pInnerFrame->field_4 + pData->field_14) << 16,
			(pInnerFrame->field_8 + pData->field_18) << 16,
			pInnerFrame->field_C,
			nAngle,
			pInnerFrame->nPic,		// pic
			nShade,					// shade
			pInnerFrame->nPalette,	// pal
			nStat,					// stat
			windowx1, windowy1, windowx2, windowy2
		);
	}
}


void QAV::Play(int a, int b, int nCallback, void *pArg)
{
}


void QAV::Preload()
{
}
