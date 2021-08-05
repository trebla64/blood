#include "stdafx.h"
#include "jfbuild/build.h"
#include "game.h"
#include "actor.h"
#include "view.h"
#include "debug.h"


void playerReset(PLAYER *pPlayer)
{
	dassert(pPlayer != NULL);
}


void playerInit(int nPlayer, uint32 bResetPlayer)
{
	dassert(nPlayer >= 0 && nPlayer < 8);
//	dassert(sizeof(PLAYER) == 787);
	PLAYER *pPlayer = &gPlayer[nPlayer];

	if (bResetPlayer)
		memset(pPlayer, 0, sizeof(PLAYER));

	// TODO: Some field-setting here

	if (bResetPlayer)
		playerReset(pPlayer);
}


void playerResetInertia(PLAYER *pPlayer)
{
	const POSTURE *pPosture = &gPosture[pPlayer->nLifeMode][pPlayer->nSprite];
	pPlayer->player_z = pPlayer->pSprite->z - pPosture->field_24;
	pPlayer->field_6B = pPlayer->pSprite->z - pPosture->field_28;
	viewBackupView(pPlayer->nView);
}


void playerStart(int nPlayer)
{
	dprintf("playerStart(%d)\n", nPlayer);
	dassert(nPlayer >= 0 && nPlayer < 8);
	PLAYER *pPlayer = &gPlayer[nPlayer];

	if (gGameOptions.nGameType == 1)
	{
	}

	ZONE *pZone = &gStartZone[nPlayer];
	SPRITE *pPlayerSprite = actSpawnSprite(pZone->sector, pZone->x, pZone->y, pZone->z, 6, 1);
	dprintf("    New sprite got sector %d\n", pPlayerSprite->sectnum);
	pPlayer->pSprite = pPlayerSprite;

	int32 z_adjust = 0;
	if ((pPlayerSprite->cstat & 0x30) != 32)
		z_adjust += (tilesizy[pPlayerSprite->picnum] - (((picanm[pPlayerSprite->picnum] >> 16) & 0xFF) +
			tilesizy[pPlayerSprite->picnum] / 2)) * 4 * pPlayerSprite->yrepeat;

	pPlayerSprite->z -= z_adjust;
	pPlayerSprite->ang      = pZone->ang;
	pPlayerSprite->type     = nPlayer + 231;
	pPlayerSprite->clipdist = 0;	// TODO: Set from dudeInfo
	pPlayerSprite->flags    = 15;

	pPlayer->nSprite = 0;
	playerResetInertia(pPlayer);

	// Clear sprite velocities
	int nIndex = pPlayerSprite->index;
	xvel[nIndex] = 0;
	yvel[nIndex] = 0;
	zvel[nIndex] = 0;

	if (pPlayer == gMe)
		viewInitializePrediction();
}
