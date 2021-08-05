#ifndef __PLAYER_H__
#define __PLAYER_H__

struct PLAYER;

void playerInit(int nPlayer, uint32 bResetPlayer);
void playerReset(PLAYER *pPlayer);
void playerStart(int nPlayer);

#endif // __PLAYER_H__
