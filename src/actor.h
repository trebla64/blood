#ifndef __ACTOR_H__
#define __ACTOR_H__


void    actInit();
void    actProcessSprites();
void    actPostProcess();
SPRITE *actSpawnSprite(int nSector, int x, int y, int z, int nStatus, bool bAddXSprite);
void    actPostSprite(int nSprite, int nStatus);


#endif // __ACTOR_H__
