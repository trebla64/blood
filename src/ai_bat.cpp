#include "stdafx.h"
#include "game.h"
#include "ai.h"

/****************************************************************************
** LOCAL callback prototypes
****************************************************************************/


/****************************************************************************
** LOCAL think/move function prototypes
****************************************************************************/

static void thinkTarget( SPRITE *pSprite, XSPRITE *pXSprite );
static void thinkChase( SPRITE *pSprite, XSPRITE *pXSprite );
static void thinkPonder( SPRITE *pSprite, XSPRITE *pXSprite );
static void thinkGoto( SPRITE *pSprite, XSPRITE *pXSprite );
static void thinkSearch( SPRITE *pSprite, XSPRITE *pXSprite );
static void MoveForward( SPRITE *pSprite, XSPRITE *pXSprite );
static void MoveSwoop( SPRITE *pSprite, XSPRITE *pXSprite );
static void MoveFly( SPRITE *pSprite, XSPRITE *pXSprite );
static void MoveToCeil( SPRITE *pSprite, XSPRITE *pXSprite );
static void MoveDodgeUp( SPRITE *pSprite, XSPRITE *pXSprite );
static void MoveDodgeDown( SPRITE *pSprite, XSPRITE *pXSprite );

/****************************************************************************
** GLOBAL CONSTANTS
****************************************************************************/

AISTATE batIdle           = { 0, NULL,    0, NULL, NULL         , thinkTarget    , NULL        };
AISTATE batFlyIdle        = { 5, NULL,    0, NULL, NULL         , thinkTarget    , NULL        };
AISTATE batChase          = { 5, NULL,    0, NULL, MoveForward  , thinkChase     , &batFlyIdle };
AISTATE batPonder         = { 5, NULL,    0, NULL, NULL         , thinkPonder    , NULL        };
AISTATE batGoto           = { 5, NULL,  600, NULL, MoveForward  , thinkGoto      , &batFlyIdle };
AISTATE batBite           = { 6, NULL,   60, NULL, NULL         , NULL           , &batPonder  };
AISTATE batRecoil         = { 4, NULL,    0, NULL, NULL         , NULL           , &batChase   };
AISTATE batSearch         = { 5, NULL,  120, NULL, MoveForward  , thinkSearch    , &batFlyIdle };
AISTATE batSwoop          = { 5, NULL,   60, NULL, MoveSwoop    , thinkChase     , &batChase   };
AISTATE batFly            = { 5, NULL,    0, NULL, MoveFly      , thinkChase     , &batChase   };
AISTATE batTurn           = { 5, NULL,   60, NULL, aiMoveTurn   , NULL           , &batChase   };
AISTATE batHide           = { 5, NULL,    0, NULL, MoveToCeil   , MoveForward    , NULL        };
AISTATE batDodgeUp        = { 5, NULL,  120, NULL, MoveDodgeUp  , NULL           , &batChase   };
AISTATE batDodgeUpRight   = { 5, NULL,   90, NULL, MoveDodgeUp  , NULL           , &batChase   };
AISTATE batDodgeUpLeft    = { 5, NULL,   90, NULL, MoveDodgeUp  , NULL           , &batChase   };
AISTATE batDodgeDown      = { 5, NULL,  120, NULL, MoveDodgeDown, NULL           , &batChase   };
AISTATE batDodgeDownRight = { 5, NULL,   90, NULL, MoveDodgeDown, NULL           , &batChase   };
AISTATE batDodgeDownLeft  = { 5, NULL,   90, NULL, MoveDodgeDown, NULL           , &batChase   };


static void thinkPonder( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void thinkTarget( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void thinkChase( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void thinkSearch( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void thinkGoto( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void MoveForward( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void MoveToCeil( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void MoveFly( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void MoveDodgeUp( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void MoveDodgeDown( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


static void MoveSwoop( SPRITE *pSprite, XSPRITE *pXSprite )
{
}
