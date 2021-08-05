#include "stdafx.h"
#include "game.h"
#include "ai.h"

static void ghostUnknown0( SPRITE *, XSPRITE * );
static void ghostUnknown1( SPRITE *, XSPRITE * );
static void ghostUnknown2( SPRITE *, XSPRITE * );
static void ghostUnknown3( SPRITE *, XSPRITE * );
static void ghostUnknown4( SPRITE *, XSPRITE * );
static void ghostUnknown5( SPRITE *, XSPRITE * );
static void ghostUnknown6( SPRITE *, XSPRITE * );
static void ghostUnknown7( SPRITE *, XSPRITE * );
static void ghostUnknown8( SPRITE *, XSPRITE * );
static void ghostUnknown9( SPRITE *, XSPRITE * );
static void ghostUnknown10( SPRITE *, XSPRITE * );

static AISTATE ghostIdle               = {  0, NULL,   60, NULL        , NULL         , ghostUnknown0  , NULL                };
static AISTATE ghostChase              = {  0, NULL,    0, NULL        , ghostUnknown1, ghostUnknown2  , &ghostIdle          };
static AISTATE ghostPonder             = {  0, NULL,    0, NULL        , NULL         , ghostUnknown3  , NULL                };
static AISTATE ghostGoto               = {  0, NULL,  600, NULL        , ghostUnknown1, ghostUnknown4  , &ghostIdle          };
static AISTATE ghostSlash              = {  5, NULL,    0, NULL        , NULL         , NULL           , &ghostPonder        };
static AISTATE ghostThrow              = {  5, NULL,    0, NULL        , NULL         , NULL           , &ghostPonder        };
static AISTATE ghostBlast              = {  5, NULL,    0, NULL        , ghostUnknown5, NULL           , &ghostChase         };
static AISTATE ghostRecoil             = {  4, NULL,    0, NULL        , NULL         , NULL           , &ghostPonder        };
static AISTATE ghostSearch             = {  0, NULL,  120, NULL        , ghostUnknown1, ghostUnknown6  , &ghostIdle          };
static AISTATE ghostSwoop              = {  0, NULL,   60, NULL        , ghostUnknown7, ghostUnknown2  , &ghostChase         };
static AISTATE ghostFly                = {  0, NULL,    0, NULL        , ghostUnknown8, ghostUnknown2  , &ghostChase         };
static AISTATE ghostTurn               = {  0, NULL,   60, NULL        , aiMoveTurn   , NULL           , &ghostChase         };
static AISTATE ghostDodgeUp            = {  0, NULL,  120, NULL        , ghostUnknown9, NULL           , &ghostChase         };
static AISTATE ghostDodgeUpRight       = {  0, NULL,   90, NULL        , ghostUnknown9, NULL           , &ghostChase         };
static AISTATE ghostDodgeUpLeft        = {  0, NULL,   90, NULL        , ghostUnknown9 , NULL           , &ghostChase         };
static AISTATE ghostDodgeDown          = {  0, NULL,  120, NULL        , ghostUnknown10, NULL           , &ghostChase         };
static AISTATE ghostDodgeDownRight     = {  0, NULL,   90, NULL        , ghostUnknown10, NULL           , &ghostChase         };
static AISTATE ghostDodgeDownLeft      = {  0, NULL,   90, NULL        , ghostUnknown10, NULL           , &ghostChase         };


static void ghostUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown3( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown4( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown5( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown6( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown7( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown8( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown9( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void ghostUnknown10( SPRITE *pSprite, XSPRITE *pXSprite )
{
}
