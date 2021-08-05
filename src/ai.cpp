#include "stdafx.h"
#include "game.h"
#include "ai.h"
#include "db.h"
#include "trig.h"
#include "misc.h"
#include "actor.h"
#include "player.h"
#include "globals.h"
#include "gameutil.h"
#include "multi.h"
#include "dude.h"
#include "debug.h"
#include "eventq.h"

#include "ai_cultist.h"
#include "ai_gargoyle.h"
#include "ai_hand.h"
#include "ai_hound.h"
#include "ai_rat.h"
#include "ai_spider.h"
#include "ai_zombie_axe.h"
#include "ai_zombie_fat.h"

#if 0

/****************************************************************************
** LOCAL CONSTANTS
****************************************************************************/

#define kAIThinkRate		8	// how often high level AI is sampled (in frames)
#define kAIThinkMask		(kAIThinkRate - 1)
#define kAIThinkTime		(kAIThinkRate * kFrameTicks)

#define kSGargoyleMeleeDist		M2X(2.0)	//M2X(1.6)
#define kSGargoyleBlastDist1	M2X(20)	// used for paralyzing blast
#define kSGargoyleBlastDist2	M2X(14)

#define kTentacleActivateDist	M2X(5)	// activates and stays on until target reaches deactivate distance?
#define kTentacleDeactivateDist	M2X(9)
#define kTentacleMeleeDist		M2X(2)

#define kPodActivateDist		M2X(8)
#define kPodDeactivateDist		M2X(14)
#define kPodFireDist1			M2X(12)
#define kPodFireDist2			M2X(8)

#define kGillBeastMeleeDist		M2X(1.6)
#define kGillBeastSummonDist1	M2X(16)
#define kGillBeastSummonDist2	M2X(12)

#define kEelMeleeDist		M2X(1)
#define kRatMeleeDist		M2X(1)
#define kHandMeleeDist		M2X(1)

#define kCerberusMeleeDist	M2X(2)
#define kCerberusBlastDist1	M2X(14)	// used for fireball
#define kCerberusBlastDist2	M2X(10)

#define kPhantasmMeleeDist	M2X(1.6)
#define kPhantasmThrowDist1	M2X(16)
#define kPhantasmThrowDist2	M2X(12)


static int cumulDamage[kMaxXSprites];	// cumulative damage per frame
int gDudeSlope[kMaxXSprites];
#endif

#define kSeqDudeRecoil 4

// Function stubs
static void MoveForward( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void MoveSwoop( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void MoveFly( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void MoveToCeil( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void MoveDodgeUp( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void MoveDodgeDown( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void MoveAscend( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void MoveSlow( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void thinkTarget( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void thinkChase( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void thinkPonder( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void thinkGoto( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void thinkSearch( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown3( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown4( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown5( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown6( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown7( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown8( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void eelUnknown9( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void aiThinkTarget( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void aiMoveForward( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void cultistUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )	{}
static void cultistUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )	{}
static void cultistUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )	{}
static void cultistUnknown3( SPRITE *pSprite, XSPRITE *pXSprite )	{}
static void cerbUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void cerbUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void cerbUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown3( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown4( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown5( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown6( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown7( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown8( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gargUnknown9( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void entryFStatue( SPRITE *pSprite, XSPRITE *pXSprite )		{}	// Flesh statue entry
static void entrySStatue( SPRITE *pSprite, XSPRITE *pXSprite )		{}	// Stone statue entry
static void gillUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gillUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void gillUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown3( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown4( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown5( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown6( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown7( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown8( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown9( SPRITE *pSprite, XSPRITE *pXSprite )		{}
//static void ghostUnknown10( SPRITE *pSprite, XSPRITE *pXSprite )	{}
static void houndUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void houndUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void houndUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void spidUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void spidUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void spidUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown0( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown1( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown2( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown3( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown4( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown5( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void zombieUnknown6( SPRITE *pSprite, XSPRITE *pXSprite )		{}
static void entryAIdle( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void entryEZombie( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void entryEStand( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void myThinkTarget( SPRITE *pSprite, XSPRITE *pXSprite )			{}
static void myThinkSearch( SPRITE *pSprite, XSPRITE *pXSprite )			{}

// These are all the AI states in one place, and must be moved later
static AISTATE genIdle		= { 0,	NULL,	0,	NULL,	NULL,	NULL,	NULL };
static AISTATE genRecoil         = { 4, NULL,   20, NULL, NULL         , NULL           , &genIdle    };
// TODO: Move the following to the eel section
static AISTATE eelIdle           = { 0, NULL,    0, NULL, NULL         , eelUnknown0    , NULL        };
static AISTATE eelFlyIdle        = { 0, NULL,    0, NULL, NULL         , eelUnknown0    , NULL        };
static AISTATE eelChase          = { 0, NULL,    0, NULL, eelUnknown1  , eelUnknown2    , &eelIdle    };
static AISTATE eelPonder         = { 0, NULL,    0, NULL, NULL         , eelUnknown3    , NULL        };
static AISTATE eelGoto           = { 0, NULL,  600, NULL, NULL         , eelUnknown4    , &eelIdle    };
static AISTATE eelBite           = { 6, NULL,   60, NULL, NULL         , NULL           , &eelChase   };
static AISTATE eelRecoil         = { 4, NULL,    0, NULL, NULL         , NULL           , &eelChase   };
static AISTATE eelSearch         = { 0, NULL,  120, NULL, eelUnknown1  , eelUnknown5    , &eelIdle    };
static AISTATE eelSwoop          = { 0, NULL,   60, NULL, eelUnknown6  , eelUnknown2    , &eelChase   };
static AISTATE eelFly            = { 0, NULL,    0, NULL, MoveAscend   , eelUnknown2    , &eelChase   };
static AISTATE eelTurn           = { 0, NULL,   60, NULL, aiMoveTurn   , NULL           , &eelChase   };
static AISTATE eelHide           = { 0, NULL,    0, NULL, eelUnknown7  , eelUnknown1    , NULL        };
static AISTATE eelDodgeUp        = { 0, NULL,  120, NULL, eelUnknown8  , NULL           , &eelChase   };
static AISTATE eelDodgeUpRight   = { 0, NULL,   90, NULL, eelUnknown8  , NULL           , &eelChase   };
static AISTATE eelDodgeUpLeft    = { 0, NULL,   90, NULL, eelUnknown8  , NULL           , &eelChase   };
static AISTATE eelDodgeDown      = { 0, NULL,  120, NULL, eelUnknown9  , NULL           , &eelChase   };
static AISTATE eelDodgeDownRight = { 0, NULL,   90, NULL, eelUnknown9  , NULL           , &eelChase   };
static AISTATE eelDodgeDownLeft  = { 0, NULL,   90, NULL, eelUnknown9  , NULL           , &eelChase   };
// TODO: Move the following to the cultist section
static AISTATE cultistBurnIdle   = { 3, NULL,    0, NULL, NULL         , aiThinkTarget  , NULL        };
static AISTATE cultistBurnChase  = { 3, NULL,    0, NULL, aiMoveForward, cultistUnknown0, NULL        };
static AISTATE cultistBurnSearch = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown2, &cultistBurnSearch };
static AISTATE cultistBurnGoto   = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown1, &cultistBurnSearch };
static AISTATE cultistBurnAttack = { 3, NULL,  120, NULL, NULL         , NULL           , &cultistBurnChase  };
static AISTATE playerBurnChase   = { 3, NULL,    0, NULL, aiMoveForward, cultistUnknown0, NULL               };
static AISTATE playerBurnSearch  = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown2, &playerBurnSearch  };
static AISTATE playerBurnGoto    = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown1, &playerBurnSearch  };
static AISTATE playerBurnAttack  = { 3, NULL,  120, NULL, NULL         , NULL           , &playerBurnChase   };
static AISTATE zombieABurnChase  = { 3, NULL,    0, NULL, aiMoveForward, cultistUnknown0, NULL               };
static AISTATE zombieABurnSearch = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown2, NULL               };
static AISTATE zombieABurnGoto   = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown1, &zombieABurnSearch };
static AISTATE zombieABurnAttack = { 3, NULL,  120, NULL, NULL         , NULL           , &zombieABurnChase  };
static AISTATE zombieFBurnChase  = { 3, NULL,    0, NULL, aiMoveForward, cultistUnknown0, NULL               };
static AISTATE zombieFBurnSearch = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown2, NULL               };
static AISTATE zombieFBurnGoto   = { 3, NULL, 3600, NULL, aiMoveForward, cultistUnknown1, &zombieFBurnSearch };
static AISTATE zombieFBurnAttack = { 3, NULL,  120, NULL, NULL         , NULL           , &zombieFBurnChase  };
static AISTATE cerberusIdle      = { 0, NULL,    0, NULL, NULL         , aiThinkTarget  , NULL               };
static AISTATE cerberusSearch    = { 6, NULL, 1800, NULL, aiMoveForward, cerbUnknown0   , &cerberusIdle      };
static AISTATE cerberusChase     = { 6, NULL,    0, NULL, aiMoveForward, cerbUnknown1   , NULL               };
static AISTATE cerberusRecoil    = { 4, NULL,    0, NULL, NULL         , NULL           , &cerberusSearch    };
static AISTATE cerberusGoto      = { 6, NULL,  600, NULL, aiMoveForward, cerbUnknown2   , &cerberusIdle      };
static AISTATE cerberusBite      = { 5, NULL,   60, NULL, NULL         , NULL           , &cerberusChase     };
static AISTATE cerberusBurn      = { 5, NULL,   60, NULL, NULL         , NULL           , &cerberusChase     };
static AISTATE cerberus3Burn     = { 5, NULL,   60, NULL, NULL         , NULL           , &cerberusChase     };
static AISTATE cerberus2Idle     = { 0, NULL,    0, NULL, NULL         , aiThinkTarget  , NULL               };
static AISTATE cerberus2Search   = { 6, NULL, 1800, NULL, aiMoveForward, cerbUnknown0   , &cerberusIdle      };
static AISTATE cerberus2Chase    = { 6, NULL,    0, NULL, aiMoveForward, cerbUnknown1   , NULL               };
static AISTATE cerberus2Recoil   = { 4, NULL,    0, NULL, NULL         , NULL           , &cerberus2Search   };
static AISTATE cerberus2Goto     = { 6, NULL,  600, NULL, aiMoveForward, cerbUnknown2   , &cerberus2Idle     };
static AISTATE cerberus2Bite     = { 5, NULL,   60, NULL, NULL         , NULL           , &cerberus2Chase    };
static AISTATE unnamed1          = { 5, NULL,   60, NULL, NULL         , NULL           , &cerberus2Chase    };
static AISTATE cerberus4Burn     = { 5, NULL,   60, NULL, NULL         , NULL           , &cerberus2Chase    };
static AISTATE cultistTFire      = { 5, NULL,    0, NULL, aiMoveTurn   , cultistUnknown3, &cultistTFire      };
static AISTATE cultistProneChase = { 0, NULL,    0, NULL, NULL         , NULL           , NULL               };	// FIXME
static AISTATE cultistDodge      = { 0, NULL,    0, NULL, NULL         , NULL           , NULL               };	// FIXME
static AISTATE cultistProneDodge = { 0, NULL,    0, NULL, NULL         , NULL           , NULL               };	// FIXME
static AISTATE cultistSProneFire = { 7, NULL,   60, NULL, NULL         , NULL           , &cultistProneChase };
static AISTATE cultistTProneFire = { 7, NULL,    0, NULL, aiMoveTurn   , cultistUnknown3, &cultistTProneFire };
static AISTATE cultistRecoil     =  {  4, NULL,    0, NULL, NULL         , NULL           , &cultistDodge      };
static AISTATE cultistProneRecoil = {  4, NULL,    0, NULL, NULL         , NULL           , &cultistProneDodge };
static AISTATE cultistSwimIdle    = { 12, NULL,    0, NULL, NULL         , aiThinkTarget  , NULL               };
static AISTATE cultistSwimChase   = { 12, NULL,    0, NULL, aiMoveForward, cultistUnknown3, NULL               };
static AISTATE cultistSwimDodge   = { 12, NULL,   90, NULL, aiMoveForward, NULL           , &cultistSwimChase  };
static AISTATE cultistSwimGoto    = { 12, NULL,  600, NULL, aiMoveForward, thinkGoto      , &cultistSwimIdle   };
static AISTATE cultistSwimSearch  = { 12, NULL, 1800, NULL, aiMoveForward, thinkSearch    , &cultistSwimIdle   };
static AISTATE cultistSSwimFire   = {  7, NULL,   60, NULL, NULL         , NULL           , &cultistSwimChase  };
static AISTATE cultistTSwimFire   = {  7, NULL,    0, NULL, aiMoveTurn   , cultistUnknown3, &cultistTSwimFire  };
static AISTATE cultistSwimRecoil  = {  4, NULL,    0, NULL, NULL         , NULL           , &cultistSwimDodge  };
static AISTATE gargoyleFIdle      = {  0, NULL,    0, NULL, NULL         , gargUnknown0   , NULL               };
static AISTATE gargoyleFChase     = {  0, NULL,    0, NULL, gargUnknown1 , gargUnknown2   , &gargoyleFIdle     };
static AISTATE gargoyleFPonder    = {  0, NULL,    0, NULL, NULL         , gargUnknown3   , NULL               };
static AISTATE gargoyleFGoto      = {  0, NULL,  600, NULL, gargUnknown1 , gargUnknown4   , &gargoyleFIdle     };
static AISTATE gargoyleFSlash     = {  5, NULL,  120, NULL, NULL         , NULL           , &gargoyleFPonder   };
static AISTATE gargoyleFThrow     = {  5, NULL,  120, NULL, NULL         , NULL           , &gargoyleFChase    };
static AISTATE gargoyleSThrow     = {  5, NULL,  120, NULL, gargUnknown1 , NULL           , &gargoyleFPonder   };
static AISTATE gargoyleSBlast     = {  6, NULL,   60, NULL, MoveSlow     , NULL           , &gargoyleFPonder   };
static AISTATE gargoyleFRecoil    = {  4, NULL,    0, NULL, NULL         , NULL           , &gargoyleFPonder   };
static AISTATE gargoyleFSearch    = {  0, NULL,  120, NULL        , gargUnknown1 , gargUnknown5   , &gargoyleFIdle     };
static AISTATE gargoyleFMorph2    = { -1, NULL,    0, entryFStatue, NULL         , NULL           , &gargoyleFChase    };
static AISTATE gargoyleFMorph     = {  5, NULL,    0, NULL        , NULL         , NULL           , &gargoyleFMorph2   };
static AISTATE gargoyleSMorph2    = { -1, NULL,    0, entrySStatue, NULL         , NULL           , &gargoyleFChase    };
static AISTATE gargoyleSMorph     = {  5, NULL,    0, NULL        , NULL         , NULL           , &gargoyleSMorph2   };
static AISTATE gargoyleSwoop      = {  0, NULL,  120, NULL        , gargUnknown6 , gargUnknown2   , &gargoyleFChase    };
static AISTATE gargoyleFly        = {  0, NULL,    0, NULL        , gargUnknown7 , gargUnknown2   , &gargoyleFChase    };
static AISTATE gargoyleTurn       = {  0, NULL,  120, NULL        , aiMoveTurn   , NULL           , &gargoyleFChase    };
static AISTATE gargoyleDodgeUp         = {  0, NULL,   60, NULL        , gargUnknown8 , NULL           , &gargoyleFChase    };
static AISTATE gargoyleFDodgeUpRight   = {  0, NULL,   90, NULL        , gargUnknown8 , NULL           , &gargoyleFChase    };
static AISTATE gargoyleFDodgeUpLeft    = {  0, NULL,   90, NULL        , gargUnknown8 , NULL           , &gargoyleFChase    };
static AISTATE gargoyleDodgeDown       = {  0, NULL,  120, NULL        , gargUnknown9 , NULL           , &gargoyleFChase    };
static AISTATE gargoyleFDodgeDownRight = {  0, NULL,   90, NULL        , gargUnknown9 , NULL           , &gargoyleFChase    };
static AISTATE gargoyleFDodgeDownLeft  = {  0, NULL,   90, NULL        , gargUnknown9 , NULL           , &gargoyleFChase    };
static AISTATE gillBeastIdle           = {  0, NULL,    0, NULL        , NULL         , aiThinkTarget  , NULL               };
static AISTATE gillBeastChase          = {  8, NULL,    0, NULL        , aiMoveForward, gillUnknown0   , NULL               };
static AISTATE gillBeastDodge          = {  8, NULL,   90, NULL        , aiMoveDodge  , NULL           , &gillBeastChase    };
static AISTATE gillBeastGoto           = {  8, NULL,  600, NULL        , aiMoveForward, gillUnknown1   , &gillBeastIdle     };
static AISTATE gillBeastBite           = {  5, NULL,  120, NULL        , NULL         , NULL           , &gillBeastChase    };
static AISTATE gillBeastSearch         = {  8, NULL, 1800, NULL        , aiMoveForward, gillUnknown2   , &gillBeastIdle     };
static AISTATE gillBeastRecoil         = {  4, NULL,    0, NULL        , NULL         , NULL           , &gillBeastDodge    };
static AISTATE gillBeastSwimIdle       = {  9, NULL,    0, NULL        , NULL         , aiThinkTarget  , NULL               };
static AISTATE gillBeastSwimChase      = {  9, NULL,    0, NULL        , aiMoveForward, gillUnknown0   , NULL                };
static AISTATE gillBeastSwimDodge      = {  9, NULL,   90, NULL        , aiMoveDodge  , NULL           , &gillBeastSwimChase };
static AISTATE gillBeastSwimGoto       = {  9, NULL,  600, NULL        , aiMoveForward, gillUnknown1   , &gillBeastSwimIdle  };
static AISTATE gillBeastSwimSearch     = {  9, NULL, 1800, NULL        , aiMoveForward, gillUnknown2   , &gillBeastSwimIdle  };
static AISTATE gillBeastSwimBite       = {  6, NULL,    0, NULL        , aiMoveTurn   , gillUnknown0   , &gillBeastSwimChase };
static AISTATE gillBeastSwimRecoil     = {  4, NULL,    0, NULL        , NULL         , NULL           , &gillBeastSwimDodge };
static AISTATE houndIdle               = {  0, NULL,    0, NULL         , NULL          , aiThinkTarget  , NULL                };
static AISTATE houndSearch             = {  7, NULL, 1800, NULL         , aiMoveForward , houndUnknown0  , &houndIdle          };
static AISTATE houndChase              = {  7, NULL,    0, NULL         , aiMoveForward , houndUnknown1  , NULL                };
static AISTATE houndRecoil             = {  4, NULL,    0, NULL         , NULL          , NULL           , &houndSearch        };
static AISTATE houndGoto               = {  7, NULL,  600, NULL         , aiMoveForward , houndUnknown2  , &houndIdle          };
static AISTATE houndBite               = {  5, NULL,   60, NULL         , NULL          , NULL           , &houndChase         };
static AISTATE houndBurn               = {  6, NULL,   60, NULL         , NULL          , NULL           , &houndChase         };
static AISTATE spidIdle                = {  0, NULL,    0, NULL         , NULL          , aiThinkTarget  , NULL                };
static AISTATE spidChase               = {  6, NULL,    0, NULL         , aiMoveForward , spidUnknown0   , NULL                };
static AISTATE spidDodge               = {  6, NULL,   90, NULL         , aiMoveDodge   , NULL           , &spidChase          };
static AISTATE spidGoto                = {  6, NULL,  600, NULL         , aiMoveForward , spidUnknown1   , &spidIdle           };
static AISTATE spidSearch              = {  6, NULL, 1800, NULL         , aiMoveForward , spidUnknown2   , &spidIdle           };
static AISTATE spidBite                = {  5, NULL,   60, NULL         , NULL          , NULL           , &spidChase          };
static AISTATE spidJump                = {  7, NULL,   60, NULL         , aiMoveForward , NULL           , &spidChase          };
static AISTATE zombieAIdle             = {  0, NULL,    0, entryAIdle   , NULL          , aiThinkTarget  , NULL                };
static AISTATE zombieAChase            = {  7, NULL,    0, NULL         , aiMoveForward , zombieUnknown0 , NULL                };
static AISTATE zombieAPonder           = {  0, NULL,    0, NULL         , aiMoveTurn    , zombieUnknown1 , NULL                };
static AISTATE zombieAGoto             = {  7, NULL, 1800, NULL         , aiMoveForward , zombieUnknown2 , &zombieAIdle        };
static AISTATE zombieAHack             = {  5, NULL,   80, NULL         , NULL          , NULL           , &zombieAPonder      };
static AISTATE zombieASearch           = {  7, NULL, 1800, NULL         , aiMoveForward , zombieUnknown3 , &zombieAIdle        };
static AISTATE zombieARecoil           = {  4, NULL,    0, NULL         , NULL          , NULL           , &zombieAPonder      };
static AISTATE zombieAStand            = {  9, NULL,    0, NULL         , NULL          , NULL           , &zombieAPonder      };
static AISTATE zombieARecoil2          = {  1, NULL,  360, NULL         , NULL          , NULL           , &zombieAStand       };
static AISTATE zombieEIdle             = { 10, NULL,    0, NULL         , NULL          , aiThinkTarget  , NULL                };
static AISTATE zombieEUp2              = {  0, NULL,    0, entryEZombie , NULL          , NULL           , &zombieASearch      };
static AISTATE zombieEUp               = {  8, NULL,  180, entryEStand  , NULL          , NULL           , &zombieEUp2         };
static AISTATE zombie2Idle             = {  0, NULL,    0, entryAIdle   , NULL          , myThinkTarget  , NULL                };
static AISTATE zombie2Search           = {  7, NULL, 1800, NULL         , aiMoveForward , myThinkSearch  , &zombie2Idle        };
static AISTATE zombieFIdle             = {  0, NULL,    0, NULL         , NULL          , aiThinkTarget  , NULL                };
static AISTATE zombieFChase            = {  7, NULL,    0, NULL         , aiMoveForward , zombieUnknown4 , NULL                };
static AISTATE zombieFGoto             = {  7, NULL,  600, NULL         , aiMoveForward , zombieUnknown5 , &zombieFIdle        };
static AISTATE zombieFDodge            = {  7, NULL,    0, NULL         , aiMoveDodge   , zombieUnknown4 , &zombieFChase       };
static AISTATE zombieFHack             = {  5, NULL,  120, NULL         , NULL          , NULL           , &zombieFChase       };
static AISTATE zombieFPuke             = {  8, NULL,  120, NULL         , NULL          , NULL           , &zombieFChase       };
static AISTATE zombieFThrow            = {  5, NULL,  120, NULL         , NULL          , NULL           , &zombieFChase       };
static AISTATE zombieFSearch           = {  7, NULL, 1800, NULL         , aiMoveForward , zombieUnknown6 , &zombieFIdle        };
static AISTATE zombieFRecoil           = {  4, NULL,    0, NULL         , NULL          , NULL           , &zombieFChase       };


void aiInit()
{
	for (int i = headspritestat[6]; i >= 0; i = nextspritestat[i])
	{
		SPRITE *pDude = &sprite[i];
		XSPRITE *pXDude = &xsprite[pDude->extra];

		switch (pDude->type)
		{
			case kDudeTommyCultist:
			case kDudeShotgunCultist:
				aiNewState(pDude, pXDude, &cultistIdle);
				break;
		}
	}
}


void aiProcessDudes()
{
	for (int i = headspritestat[6]; i >= 0; i = nextspritestat[i])	// TODO: 6 = kStatDude
	{
		SPRITE *pSprite = &sprite[i];
		if (!(pSprite->flags & 0x20))
		{
			int nXSprite = pSprite->extra;
			int nType = pSprite->type;
			XSPRITE *pXSprite = &xsprite[nXSprite];
//			DUDEINFO *pDudeInfo = &dudeInfo[nType - kDudeBase];
			// Only do sprites that are not players
			if (nType < 231 || nType > 238)	// TODO: Fix magic
			{
				AISTATE *pState = (AISTATE *)pXSprite->aiState;
				if (pState)
				{
					// Call move function
					if (pState->move)
						pState->move(pSprite, pXSprite);

					// Call think function
					if (pState->think)
						pState->think(pSprite, pXSprite);
				}
			}
		}
	}
}


void aiMoveForward( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


void aiThinkTarget( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


void aiMoveTurn( SPRITE *pSprite, XSPRITE *pXSprite )
{
}


void aiNewState( SPRITE *pSprite, XSPRITE *pXSprite, AISTATE *pState )
{
}


void aiMoveDodge( SPRITE *pSprite, XSPRITE *pXSprite )
{
}
