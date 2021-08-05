#include "stdafx.h"
#include "game.h"
#include "ai.h"

static void ratUnknown0( SPRITE *, XSPRITE * );
static void ratUnknown1( SPRITE *, XSPRITE * );
static void ratUnknown2( SPRITE *, XSPRITE * );

static AISTATE ratIdle                 = {  0, NULL,    0, NULL         , NULL          , aiThinkTarget  , NULL                };
static AISTATE ratSearch               = {  6, NULL, 1800, NULL         , aiMoveForward , ratUnknown0    , &ratIdle            };
static AISTATE ratChase                = {  6, NULL,    0, NULL         , aiMoveForward , ratUnknown1    , NULL                };
static AISTATE ratDodge                = {  6, NULL,    0, NULL         , NULL          , NULL           , &ratChase           };
static AISTATE ratRecoil               = {  6, NULL,    0, NULL         , NULL          , NULL           , &ratDodge           };
static AISTATE ratGoto                 = {  6, NULL,  600, NULL         , aiMoveForward , ratUnknown2    , &ratIdle            };
static AISTATE ratBite                 = {  5, NULL,  120, NULL         , NULL          , NULL           , &ratChase           };


static void ratUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ratUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ratUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )
{
}
