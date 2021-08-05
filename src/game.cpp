#include "stdafx.h"
#include "game.h"
#include "db.h"
#include "actor.h"
#include "resource.h"
#include "gamemenu.h"
#include "messages.h"
#include "levels.h"
#include "screen.h"
#include "view.h"
#include "net.h"
#include "gameutil.h"
#include "player.h"
#include "warp.h"
#include "triggers.h"
#include "engine_access.h"
#include "sectorfx.h"
#include "mirrors.h"
#include "debug.h"

#include "jfbuild/build.h"

GAMEOPTIONS gGameOptions;
int         gFrameClock;
int         gFrame;
bool        gPaused;
bool        gStartNewGame;
bool        gGameStarted = false;
bool        gQuitGame;
bool        gQuitRequest;
int         gInputMode = 0;
NEW_INPUT   gInput;
int         gViewPos;
int         gViewMode = 3;
int         gGamma = 0;
int         gGammaLevels;
int         gSkill;
int         gNextLevel;
bool        gAutoRun = true;
int         gYLookup[MAX_YLOOKUPS];
bool        bAddUserMap = false;

// Trigger related stuff (TODO: Move to trigger module?)
POINT2D baseWall[MAXWALLS];
POINT3D baseSprite[MAXSPRITES];

int baseFloor[MAXSECTORS];
int baseCeil[MAXSECTORS];

// View related stuff (TODO: Move to view module)
int nInterpolations;

bool  gInterpolateSprite[512];	// TODO: Make uint8
bool  gInterpolateWall[1024];	// TODO: Make uint8
uint8 gInterpolateSector[128];

bool gViewInterpolate = true;
int gInterpolate;

INTERPOLATE gInterpolation[4096];

VIEW predict;
VIEW predictOld;

VIEW predictFifo[256];
int gPredictTail;
NEW_INPUT gFifoInput[2048];

int gNetFifoHead[8];

int16 gUpperLink[MAXSECTORS];
int16 gLowerLink[MAXSECTORS];

int32 xvel[MAXSPRITES];
int32 yvel[MAXSPRITES];
int32 zvel[MAXSPRITES];

uint8 *gCLU;

PLAYERTEMP  gPlayerTemp;

const POSTURE gPosture[2][3] =
{
	// Lifemode 0
	{
		{ 0x4000, 0x4000, 0x4000, 0x0E, 0x11, 0x18, 0x10, 0x20, 0x50, 5632, 4608 },	// Sprite 0
		{ 0x1200, 0x1200, 0x1200, 0x0E, 0x11, 0x18, 0x10, 0x20, 0x50, 5120, 4096 },	// Sprite 1
		{ 0x2000, 0x2000, 0x2000, 0x16, 0x1C, 0x18, 0x10, 0x10, 0x28, 2048, 1536 }	// Sprite 2
	},

	// Lifemode 1
	{
		{ 0x4000, 0x4000, 0x4000, 0x0E, 0x11, 0x18, 0x10, 0x20, 0x50, 5632, 2560 },	// Sprite 0
		{ 0x1200, 0x1200, 0x1200, 0x0E, 0x11, 0x18, 0x10, 0x20, 0x50, 5120, 3072 },	// Sprite 1
		{ 0x2000, 0x2000, 0x2000, 0x16, 0x1C, 0x18, 0x10, 0x10, 0x28, 2048, 1024 }	// Sprite 2
	}
};

PROFILE gProfile[8];
PLAYER  gPlayer[8];
PLAYER *gView;
PLAYER *gMe;
int     gViewIndex;
VIEW    gPrevView[8];

int gDetail;
int MusicVolume;
int FXVolume;
int gMouseSensitivity;
int gTurnSpeed = 92;
int gMessageCount;
int gMessageTime;
int gAimReticle;
int gShowWeapon;
int gSlopeTilting;
int gViewVBobbing;
int gViewHBobbing;

int numplayers = 1;

const GAMEOPTIONS gSingleGameOptions =
{
	GT_SINGLE,
	DIFF_UNKNOWN2,
	0, 0, "\0", "\0",
	2,	// nTrackNumber
	"\0", "\0", 0, 0, 0,
	1,	// nMonsterSettings
	0, 0, 0, 0, 0,
	2,
	1200, 1800, 1800, 3600
};

const GAMEOPTIONS gBloodBathGameOptions =
{
	GT_BLOODBATH,
	DIFF_UNKNOWN2,
	0, 0, "\0", "\0",
	2,	// nTrackNumber
	"\0", "\0", 0, 0, 0,
	0,	// nMonsterSettings
	0,
	1, 1, 1,
	0,
	2,
	1200, 1800, 1800, 3600
};

const GAMEOPTIONS gCoopGameOptions =
{
	GT_COOP,
	DIFF_UNKNOWN2,
	0, 0, "\0", "\0",
	2,	// nTrackNumber
	"\0", "\0", 0, 0, 0,
	1,	// nMonsterSettings
	0, 0,
	1,
   	0, 0, 0,
	1200, 1800, 1800, 3600
};

const GAMEOPTIONS gTeamGameOptions =
{
	GT_TEAMGAME,
	DIFF_UNKNOWN2,
	0, 0, "\0", "\0",
	2,	// nTrackNumber
	"\0", "\0", 0, 0, 0,
	0,	// nMonsterSettings
	0,
	1, 2, 1, 0,
	1,
	1200, 1800, 1800, 3600
};

EPISODEINFO gEpisodeInfo[5];
int gEpisodeCount;

int gSecretsFound[2];
int gSecrets;

XSECTOR xsector[MAX_XSECTORS];
XWALL   xwall[MAX_XWALLS];
XSPRITE xsprite[MAX_XSPRITES];

// Maybe engine globals
//int startposx;
//int startposy;
//int startposz;
//int startang;
//int startsectnum;
bool ready2send;
int gGameClock;
int gNetFifoClock;
ZONE gStartZone[8];

const VICTORY gVictory[26] =
{
	{ "%s boned %s like a fish"    , 0  },
	{ "%s castrated %s"            , 1  },
	{ "%s creamed %s"              , 2  },
	{ "%s destroyed %s"            , 3  },
	{ "%s diced %s"                , 4  },
	{ "%s disemboweled %s"         , 5  },
	{ "%s flattened %s"            , 6  },
	{ "%s gave %s Anal Justice"    , 7  },
	{ "%s gave AnAl MaDnEsS to %s" , 8  },
	{ "%s hurt %s real bad"        , 9  },
	{ "%s killed %s"               , 10 },
	{ "%s made mincemeat out of %s", 11 },
	{ "%s massacred %s"            , 12 },
	{ "%s mutilated %s"            , 13 },
	{ "%s reamed %s"               , 14 },
	{ "%s ripped %s a new orifice" , 15 },
	{ "%s slaughtered %s"          , 16 },
	{ "%s sliced %s"               , 17 },
	{ "%s smashed %s"              , 18 },
	{ "%s sodomized %s"            , 19 },
	{ "%s splattered %s"           , 20 },
	{ "%s squashed %s"             , 21 },
	{ "%s throttled %s"            , 22 },
	{ "%s wasted %s"               , 23 },
	{ "%s body bagged %s"          , 24 },
	{ "%s shat upon %s"            , 25 }
};

const SUICIDE gSuicide[6] =
{
	{ "%s is excrement"                      , 102 },
	{ "%s is hamburger"                      , 103 },
	{ "%s suffered scrotum separation"       , 104 },
	{ "%s is an imbecile"                    , 105 },
	{ "%s volunteered for population control", 106 },
	{ "%s has suicided"                      , 107 }
};


// Debugging variables
int nCurrentInterpolate = 0;
int gDebugInterpolates[NUM_DEBUG_INTERPOLATIONS];
int32 rposx, rposy, rposz, rang, rsector;	// Last render position/angle/sector


// Called by:
//     StartNetworkLevel
//     main
//     levelStartGame (level.cpp)
//     LevelWarp (level.cpp)
//     CDemo::Playback (demo.cpp)
void StartLevel( GAMEOPTIONS *pOptions )
{
	gStartNewGame = false;
	ready2send = false;
	gViewPos = 0;
	gGameMessageMgr.Clear();

	if (pOptions->uGameFlags & 1)
	{
	}
	memset(xsprite, 0, sizeof(xsprite));	// Should be 114688 bytes
	memset(sprite, 0, sizeof(sprite));		// Should be 180224 bytes
	dbLoadMap(pOptions->zLevelName, &startposx, &startposy, &startposz, &startang, &startsectnum, &pOptions->uMapCRC);
//	dbLoadMap("e1m3"/*pOptions->zLevelName*/, &startposx, &startposy, &startposz, &startang, &startsectnum, &pOptions->uMapCRC);
//	dbLoadExternalMap("C:\\SPILLHH.map", &startposx, &startposy, &startposz, &startang, &startsectnum, &pOptions->uMapCRC);
//	dbLoadExternalMap("C:\\Games\\Blood\\NEWBOARD.map", &startposx, &startposy, &startposz, &startang, &startsectnum, &pOptions->uMapCRC);
	srand(pOptions->uMapCRC);

	// TODO: Add call to levelClearSecrets()

	// Delete all sprites that we don't need for this difficulty/gametype
	for (int i = 0; i < MAXSPRITES; i++)
	{
		SPRITE *pSprite = &sprite[i];
		if (pSprite->statnum >= 1024)
			continue;

		if (pSprite->extra <= 0)
			continue;

		XSPRITE *pXSprite = &xsprite[pSprite->extra];

		bool bDelete = false;

//		dprintf("nDifficulty: %d\n", pOptions->nDifficulty);
		if (pOptions->nDifficulty == 0 && pXSprite->delete_diff1) bDelete = true;
		if (pOptions->nDifficulty == 1 && pXSprite->delete_diff2) bDelete = true;
		if (pOptions->nDifficulty == 2 && pXSprite->delete_diff3) bDelete = true;
		if (pOptions->nDifficulty == 3 && pXSprite->delete_diff4) bDelete = true;
		if (pOptions->nDifficulty == 4 && pXSprite->delete_diff5) bDelete = true;

		if (pOptions->nGameType == GT_SINGLE    && pXSprite->delete_single)    bDelete = true;
		if (pOptions->nGameType == GT_BLOODBATH && pXSprite->delete_bloodbath) bDelete = true;
		if (pOptions->nGameType == GT_COOP      && pXSprite->delete_coop)      bDelete = true;
		if (pOptions->nGameType == GT_TEAMGAME  && pXSprite->delete_team)      bDelete = true;

		if (bDelete)
			DeleteSprite(i);
	}

#if 0
	// Find the biggest sector and place us there, for debugging
	int32 best_area = 0;
	int32 best_x, best_y;
	int32 best_sect;
	for (int i = 0; i < numsectors; i++)
	{
		int curwall = sector[i].wallptr;
		int32 xmin = INT32_MAX;
		int32 xmax = INT32_MIN;
		int32 ymin = INT32_MAX;
		int32 ymax = INT32_MIN;
		for (int j = 0; j < sector[i].wallnum; j++)
		{
			if (wall[curwall].x < xmin) xmin = wall[curwall].x;
			if (wall[curwall].x > xmax) xmax = wall[curwall].x;
			if (wall[curwall].y < ymin) ymin = wall[curwall].y;
			if (wall[curwall].y > ymax) ymax = wall[curwall].y;
			curwall = wall[curwall].point2;
		}
		int32 area = (xmax - xmin) * (ymax - ymin);
		if (area > best_area)
		{
			int32 x = xmin + (xmax - xmin) / 2;
			int32 y = ymin + (ymax - ymin) / 2;
//			int32 sect = i;
			if (!inside(x, y, i))
				continue;
			best_area = area;
			best_x = x;
			best_y = y;
			best_sect = i;
		}
	}
	startposx = best_x;
	startposy = best_y;
	startsectnum = best_sect;
	int ceilz, floorz;
	getzsofslope(startsectnum, startposx, startposy, (long *)&ceilz, (long *)&floorz);
//	startposz = floorz + (ceilz - floorz) / 2;
	dprintf("ceilz = %d, floorz = %d\n", ceilz, floorz);
	startposz = floorz - (4096 << 1);

	gPlayerTemp.x = best_x;
	gPlayerTemp.y = best_y;
	gPlayerTemp.z = startposz;
	gPlayerTemp.ang = 0;
	gPlayerTemp.sector = best_sect;

//	startposx    = best_x;
//	startposy    = best_y;
//	startsectnum = best_sect;
#endif

	// Check our sector is correctly set
//	FindSector(startposx, startposy, startposz, &startsectnum);

	gPlayerTemp.x = startposx;
	gPlayerTemp.y = startposy;
	gPlayerTemp.z = sector[startsectnum].floorz;
	gPlayerTemp.ang = startang;
	gPlayerTemp.sector = startsectnum;

	scrLoadPLUs();
	startposz = sector[startsectnum].floorz;
	for (int i = 0; i < 8; i++)
	{
		ZONE *pZone = &gStartZone[i];
		pZone->x      = startposx;
		pZone->y      = startposy;
		pZone->z      = startposz;
		pZone->ang    = startang;
		pZone->sector = startsectnum;
	}

	InitSectorFX();
	warpInit();
	actInit();
	// TODO: For all players
	playerStart(0);

	// XXX: Remove this
	ZONE *pZone = &gStartZone[0];
	gPlayerTemp.x      = pZone->x;
	gPlayerTemp.y      = pZone->y;
	gPlayerTemp.z      = pZone->z;
	gPlayerTemp.ang    = pZone->ang;
	gPlayerTemp.sector = pZone->sector;

	InitMirrors();
	trInit();

	gGameClock    = 0;
	gFrameClock   = 0;
	gNetFifoClock = 0;
	gPredictTail  = 0;
	gFrame = 0;
	gPaused = false;
	gGameStarted = true;
	// TODO: Demo related menu disable
	gGameMenuMgr.Deactivate();
	ready2send = true;
}


void StartNetworkLevel()
{
}


void ProcessFrame()
{
//	dprintf( "ProcessFrame()\n" );
	if ( !gPaused && (!CGameMenuMgr::m_bActive || gGameOptions.nGameType) )
	{
		viewClearInterpolations();
		// TODO: Use connect list
//		for (int i = 0; i < 8; i++)
			viewBackupView(0);

		trProcessBusy();
		DoSectorPanning();
		actProcessSprites();
		actPostProcess();
		viewCorrectPrediction();
		gFrame++;
//		gFrameClock += 4;
		if ( (gGameOptions.uGameFlags & 1) && !gStartNewGame )
		{
			if (!(gGameOptions.uGameFlags & 2))
			{
				levelSetupOptions(gGameOptions.nEpisode, gNextLevel);
				viewResizeView(gViewSize);
			}
		}
	}
}


void SetGamma(CGameMenuItemSlider *pSlider)
{
	gGamma = pSlider->m_nValue;
	scrSetGamma(gGamma);
}


void SetMusicVol(CGameMenuItemSlider *pSlider)
{
}


void SetSoundVol(CGameMenuItemSlider *pSlider)
{
}


void SetCDVol(CGameMenuItemSlider *pSlider)
{
}


void SetMouseSensitivity(CGameMenuItemSlider *pSlider)
{
}


void SetCrosshair(CGameMenuItemZBool *pBool)
{
}


void SetShowWeapons(CGameMenuItemZBool *pBool)
{
}


void SetSlopeTilting(CGameMenuItemZBool *pBool)
{
}


void SetViewBobbing(CGameMenuItemZBool *pBool)
{
}


void SetViewSwaying(CGameMenuItemZBool *pBool)
{
}


void SetMessages(CGameMenuItemZBool *pBool)
{
}


void SetParentalLock(CGameMenuItemZBool *pBool)
{
}


void Quit(CGameMenuItemChain *pChain)
{
	if (gGameOptions.nGameType && numplayers != 1)
		gQuitRequest = true;
	else
		gQuitGame = true;
	gGameMenuMgr.Deactivate();
}


void SetDifficultyAndStart(CGameMenuItemChain *pChain)
{
	dprintf("Starting game with difficulty %d...\n", pChain->m_field_30);
	gGameOptions.nDifficulty = pChain->m_field_30;
	gSkill = gGameOptions.nDifficulty;
	gGameOptions.nLevel = 0;
	gStartNewGame = true;
	gGameMenuMgr.Deactivate();
	gGameMenuMgr.Push(&menuLoading, -1);
}


void StartNetGame(CGameMenuItemChain *pChain)
{
}


void SaveGame(CGameMenuItemZEdit *pEdit, CGameMenuEvent *pEvent)
{
}


void LoadGame(CGameMenuItemZEdit *pEdit, CGameMenuEvent *pEvent)
{
}


// This function isn't actually as fake as it's name suggests.  It does
// network and single-player input related stuff.
extern "C" void FakeTimerHandler()
{
	if (gGameClock >= gNetFifoClock && ready2send)
	{
		gNetFifoClock += 4;
		netGetInput();
	}
}
