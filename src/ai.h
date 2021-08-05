#ifndef __AI_H__
#define __AI_H__

typedef void (/*__cdecl*/ *ai_func)( SPRITE *, XSPRITE * );

struct AISTATE
{
	int seqId;
	int seqCallback;
	int ticks;
	ai_func enter;
	ai_func move;
	ai_func think;
	AISTATE *ret;
};

void aiInit();
void aiProcessDudes();
void aiThinkTarget( SPRITE *pSprite, XSPRITE *pXSprite );
void aiMoveForward( SPRITE *pSprite, XSPRITE *pXSprite );
void aiMoveTurn( SPRITE *pSprite, XSPRITE *pXSprite );
void aiMoveDodge( SPRITE *pSprite, XSPRITE *pXSprite );
void aiNewState( SPRITE *pSprite, XSPRITE *pXSprite, AISTATE *pState );

#endif // __AI_H__
