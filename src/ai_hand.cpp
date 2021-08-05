#include "stdafx.h"
#include "game.h"
#include "ai.h"

static void handUnknown0( SPRITE *, XSPRITE * );
static void handUnknown1( SPRITE *, XSPRITE * );
static void handUnknown2( SPRITE *, XSPRITE * );

static AISTATE handIdle                = {  0, NULL,    0, NULL        , NULL          , aiThinkTarget  , NULL                };
static AISTATE handSearch              = {  5, NULL,  600, NULL        , aiMoveForward , handUnknown0   , &handIdle           };
static AISTATE handChase               = {  5, NULL,    0, NULL        , aiMoveForward , handUnknown1   , NULL                };
static AISTATE handRecoil              = {  4, NULL,    0, NULL        , NULL          , NULL           , &handSearch         };
static AISTATE handGoto                = {  5, NULL, 1800, NULL         , aiMoveForward , handUnknown2   , &handIdle           };
static AISTATE handJump                = {  6, NULL,  120, aiMoveForward, NULL          , NULL           , &handChase          };
static AISTATE handChoke               = {  0, NULL,  120, NULL         , NULL          , NULL           , &handChase          };


static void handUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void handUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void handUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )
{
}
