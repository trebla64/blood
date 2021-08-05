#include "stdafx.h"
#include "game.h"
#include "ai.h"


/*
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
*/


static void thinkChase( SPRITE *pSprite, XSPRITE *pXSprite );
static void thinkGoto( SPRITE *pSprite, XSPRITE *pXSprite );


AISTATE cultistIdle        = { 0, NULL,   0, NULL, NULL         , aiThinkTarget, NULL          };
AISTATE cultistChase       = { 8, NULL,   0, NULL, aiMoveForward, thinkChase   , NULL          };
AISTATE cultistDodge       = { 8, NULL,  90, NULL, aiMoveDodge  , NULL         , &cultistChase };
AISTATE cultistGoto        = { 8, NULL, 600, NULL, aiMoveForward, thinkGoto    , &cultistIdle  };
AISTATE cultistProneChase  = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistProneDodge  = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistTThrow      = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSThrow      = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSearch      = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSFire       = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistTFire       = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSProneFire  = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistTProneFire  = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistRecoil      = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistProneRecoil = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSwimIdle    = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSwimChase   = { 0, NULL, 0, NULL, NULL, NULL, NULL };
AISTATE cultistSwimDodge   = { 0, NULL, 0, NULL, NULL, NULL, NULL };


static void thinkChase( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void thinkGoto( SPRITE *pSprite, XSPRITE *pXSprite )
{
}
