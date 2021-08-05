#ifndef __GAME_H__
#define __GAME_H__

// I just include this here, because of the SPRITE issues
#include "jfbuild/build.h"

#define MAX_YLOOKUPS 1200	// Game has 1200 limit

struct AISTATE;

// These should all be bytes
enum GAMETYPE
{
	GT_SINGLE    = 0,
	GT_BLOODBATH = 1,
	GT_COOP      = 2,
	GT_TEAMGAME  = 3
};
//CT_ASSERT(sizeof(GAMETYPE) == 1);	// This is 4 on MSCV -.-

enum DIFFICULTY
{
	DIFF_UNKNOWN0 = 0,
	DIFF_UNKNOWN1 = 1,
	DIFF_UNKNOWN2 = 2
};

enum MONSTERSETTINGS
{
	BAR = 0
};

enum WEAPONSETTINGS
{
	WS_UNKNOWN = 0
};

enum ITEMSETTINGS
{
	IS_UNKNOWN = 0
};

enum RESPAWNSETTINGS
{
	RS_UNKNOWN = 0
};

enum TEAMSETTINGS
{
	TS_UNKNOWN = 0
};


// Dude types
enum DudeTypes
{
	kDudeBase = 200,

	kDudeTommyCultist   = 201,
	kDudeShotgunCultist = 202,
	kDudeAxeZombie      = 203,
	kDudeEarthZombie    = 205,	// Earth or Axe, not sure yet
	kDudeBat            = 219,	// Has hanging from ceiling sprite
	kDudeRat            = 220,

	kDudeMax = 247
};


// Thing types
enum ThingTypes
{
	kThingBase = 400,

	kThingMax = 436
};


struct THINGINFO
{
	int32  field_0;
	int8   clipdist;
	uint16 flags;
	int32  field_7;
	int32  field_B;
	uint16 cstat;
	int16  picnum;
	int8   shade;
	int8   pal;
	uint8  xrepeat;
	uint8  yrepeat;
	int32  unknown[6];
};
const THINGINFO thingInfo[] =
{
//    field_0
//    |         clipdist
//    |         |   flags
//    |         |   |    field_7
//    |         |   |    |       field_B
//    |         |   |    |       |     cstat
//    |         |   |    |       |     |      picnum
	{ 0xFA0019, 32, 0xB, 0x1000, 0x50, 0x180, 907, 0, 0, 0, 0, { 0x100, 0x100, 0x80, 0x40, 0, 0 } },	// 400
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 401
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 402
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 403
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 404
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 405
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 406
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 407
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 408
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 409
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 410
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 411
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 412
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 413
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 414
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 415
	{        0,  0,   0,      0,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 416
	{  0x20014,  0,   0, 0x8000,    0,     0,   0, 0, 0, 0, 0, {     0,     0,    0,    0, 0, 0 } },	// 417
};


#pragma pack(push, 1)
#if 0
struct GAMEOPTIONS
{
	uint8 gamemode;
	bool  blah;
	int   episode;
	int   level;
	char  mapName[32];	// Size still unknown
	int   seed;
	uint8 flags;
	uint8 filler[0x189 - 6 - 32 - 9];
};
#endif

#define kMaxFileKeyLen 26	// Calculated, dunno if correct

struct GAMEOPTIONS
{
	uint8			nGameType;		// GAMETYPE
	uint8			nDifficulty;	// DIFFICULTY
	int             nEpisode;

	int             nLevel;
	char            zLevelName[ 144 ];	// _MAX_PATH = 144
	char            zLevelSong[ 144 ];	// _MAX_PATH = 144
	int             nTrackNumber;
	char            szSaveGameName[kMaxFileKeyLen];
	char            szUserGameName[kMaxFileKeyLen];

	short           nSaveGameSlot;
	int             picEntry;
	uint32           uMapCRC;
	uint8			nMonsterSettings;	// MONSTERSETTINGS

	uint32           uGameFlags;

	// net game options/data only
	uint32           uNetGameFlags;

	uint8			nWeaponSettings;	// WEAPONSETTINGS
	uint8			nItemSettings;		// ITEMSETTINGS
	uint8			nRespawnSettings;	// RESPAWNSETTINGS
	uint8			nTeamSettings;		// TEAMSETTINGS [team and cooperative]

	int             nMonsterRespawnTime;
	int             nWeaponRespawnTime;
	int             nItemRespawnTime;
	int             nSpecialRespawnTime;
};
CT_ASSERT( sizeof(GAMEOPTIONS) == 0x189 );

struct DEMOHEADER
{
	uint32      signature;
	uint16      nVersion;	// Assuming 2 bytes (check)
	int         nBuild;
	int         nInputCount;
	int         nNetPlayers;
	short       nMyConnectIndex;
	short       nConnectHead;
	short       connectPoints[8];	// Blood says 8, engine says 16?
	GAMEOPTIONS gameOptions;
};
CT_ASSERT(sizeof(DEMOHEADER) == 431);

struct NEW_INPUT
{
	uint8  syncFlags;	// 1
	int8   forward;		// 2
	int16  turn;		// 4
	int8   strafe;		// 5
	uint8  buttonFlags;	// 6
	uint16 keyFlags;	// 8
	uint8  useFlags;	// 9
	uint8  newWeapon;	// 10
	int8   mlook;		// 11
};
CT_ASSERT(sizeof(NEW_INPUT) == 11);	// This actually has a size of 18 in beta?!

// Sector flags (Relates to XSECTOR.flags)
#define SFL_UNKNOWN0  0x01	// 0
#define SFL_UNKNOWN1  0x02	// 1
#define SFL_UNKNOWN2  0x04	// 2
#define SFL_UNKNOWN3  0x08	// 3
#define SFL_UNKNOWN4  0x10	// 4
#define SFL_UNKNOWN5  0x20	// 5
#define SFL_PAN_FLOOR 0x40	// 6
#define SFL_UNKNOWN7  0x80	// 7

// Part of field_4 is txID
// field_8 = rxID & command
//
// Decoupled = bit 11 of field_14
struct XSECTOR
{
	// field_0
	int32  reference     : 14;
	uint32 state         : 1;
	uint32 unknown_bits0 : 17;	// Bit 14 = state

	// field_4;
	uint32 data          : 16;
	uint32 txID          : 10;
	uint32 waveType      : 2;
	uint32 unknown_bits1 : 1;
	uint32 waveType2     : 2;
	uint32 unknown_bits2 : 1;

	// field_8
	uint32 rxID          : 10;
	uint32 command       : 8;
	uint32 sendAtOn      : 1;
	uint32 sendAtOff     : 1;
	uint32 busyTime      : 8;
	uint32 unknown_bits5 : 2;

	// field_C;
	uint32 waitTime          : 8;
	uint32 unknown_bits6     : 6;
	int32  lightingAmplitude : 8;
	uint32 lightingFrequency : 8;
	uint32 haveWaitTime      : 1;
	uint32 haveWaitTime2     : 1;

	// field_10;
	uint32 lightingPhase  : 8;
	uint32 waveLighting   : 4;
	uint32 shade_always   : 1;
	uint32 shade_floor    : 1;
	uint32 shade_ceiling  : 1;
	uint32 shade_walls    : 1;
	int32  shade_value    : 8;
	uint32 pan_always     : 1;
	uint32 pan_floor      : 1;
	uint32 pan_ceiling    : 1;
	uint32 drag           : 1;
	uint32 unknown_bits22 : 1;
	uint32 depth          : 3;

	// field_14;
	uint32 motionfx_speed : 8;
	uint32 motionfx_angle : 11;
	uint32 unknown_bits13 : 4;
	uint32 key            : 3;
	uint32 unknown_bits19 : 6;

	// field_18;
	uint32 colorLights    : 1;
	uint32 unknown_bits8  : 1;
	uint32 busyTime2      : 8;
	uint32 unknown_bits9  : 4;
	uint32 waitTime2      : 8;
	uint32 unknown_bits10 : 6;
	uint32 ceil_pal2      : 4;

	int   field_1C;
	int   field_20;
	int   field_24;
	int   field_28;
	int   field_2C;

	// field_30;
	uint32 unknown_bits11 : 25;
	uint32 damageType     : 3;
	uint32 floor_pal2     : 4;

	// field_34;
	uint32 unknown_bits14 : 8;
	uint32 unknown_bits20 : 1;
	uint32 wind_velocity  : 10;
	uint32 wind_angle     : 11;
	uint32 wind_always    : 1;		// Flag
	uint32 unknown_bits15 : 1;

	// field_38;
	uint32 z_theta         : 11;	// For z-motion (this is an angle)
	uint32 z_range         : 5;		// For z-motion
	int32  z_speed         : 12;
	uint32 z_motion_always : 1;
	uint32 bob_floor       : 1;
	uint32 bob_ceiling     : 1;
	uint32 rotate          : 1;
};
CT_ASSERT(sizeof(XSECTOR) == 60);

struct XWALL
{
	uint32 busy      : 17;
	uint32 state     : 1;
	uint32 reference : 14;	// FIXME: Number of bits
	uint8 filler[20];
};
CT_ASSERT(sizeof(XWALL) == 24);

struct XSPRITE
{
//	int field_0;	// 4
	int32  reference     : 14;
	uint32 state         : 1;
	uint32 unknown_bits2 : 17;

//	int field_4;	// 8
	uint32 txID          : 10;
	uint32 rxID          : 10;
	uint32 command       : 8;
	uint32 triggerOn     : 1;
	uint32 triggerOff    : 1;
	uint32 unknown_bits1 : 2;

//	int field_8;	// 12
	uint32 busyTime      : 12;
	uint32 waitTime      : 12;
	uint32 restState     : 1;
	uint32 unknown_bits3 : 6;
	uint32 delete_team   : 1;

//	int field_C;	// 16
	uint32 unknown_bits4    : 23;
	uint32 delete_diff1     : 1;
	uint32 delete_diff2     : 1;
	uint32 delete_diff3     : 1;
	uint32 delete_diff4     : 1;
	uint32 delete_diff5     : 1;
	uint32 delete_single    : 1;
	uint32 delete_bloodbath : 1;
	uint32 delete_coop      : 1;
	uint32 unknown_bits5    : 1;

//	int field_10;	// 20
	uint32 data1         : 16;
	uint32 data2         : 16;

//	int field_14;	// 24
	uint32 data3         : 16;
	uint32 unknown_bits6 : 16;

//	int field_18;	// 28
	uint32 unknown_bits7 : 2;
	uint32 data4         : 16;
	uint32 unknown_bits8 : 14;

	int field_1C;	// 32
	int field_20;	// 36
	int field_24;	// 40
	int field_28;	// 44
	int field_2C;	// 48
	int field_30;	// 52

//	AISTATE *aiState;	// 56	// The correct field for this?
	uint32 aiState;	// TODO: Should be AISTATE *

	// More members with known names:
	//     respawnPending
	//     targetX
	//     targetY
	//     targetZ
};
CT_ASSERT(sizeof(XSPRITE) == 56);

#if 0
struct XSPRITE
{
//	int reference;	// Reference to corresponding sprite index
	uint32 waitTime  : 17;
	uint32 restState : 1;
	uint32 state     : 1;
	uint32 reference : 13;

	uint32 unknown_bits0 : 4;
	uint32 command       : 4;
	uint32 unknown_bits1 : 10;
	uint32 triggerOff    : 1;
	uint32 triggerOn     : 1;
	uint32 unknown_bits2 : 2;
	uint32 txID          : 10;

	int respawnPending;

	uint32 unknown_bits3 : 4;
	uint32 difficulty    : 5;
	uint32 unknown_bits4 : 23;

	uint32 data2 : 16;	// Was "unknown_bits5"
	uint32 data1 : 16;	// Was "player_index"

	int goalAng;
	byte fill2[4];
	byte target1;
	byte field_1D;
	uint16 target;
	int targetX;
	int targetY;
	int targetZ;
	byte fill3[4];
	uint16 unknown0;
	uint16 stateTimer;
	AISTATE *aiState;
};
CT_ASSERT( sizeof(XSPRITE) == 56 );
#endif

//struct SPRITE
//{
//	int32 x, y, z;
//	int16 cstat, picnum;
//	int8  shade;
//	int8  pal, clipdist, filler;
//	uint8 xrepeat, yrepeat;
//	int8  xoffset, yoffset;
//	int16 sectnum, statnum;
//	int16 ang, owner, xvel, yvel, zvel;
//	int16 lotag, hitag, extra;
//};
//CT_ASSERT(sizeof(SPRITE) == 44);

struct PLAYER
{
	SPRITE  *pSprite;
	XSPRITE *pXSprite;

	int     nQAVIndex;	// 0x22
	uint32  field_73;	// 1
	uint32  field_77;	// 2
	uint32	field_7B;	// 3
	uint32  field_2B5;	// 4
	uint32  field_2A;	// 5
	int     nSprite;	// 6
	uint32  field_33;	// 7
	uint32  field_37;	// 8
	uint32  field_3B;	// 9
	uint32  field_3F;	// 10
	uint32  field_43;	// 11
	uint32  field_47;	// 12
	uint32  field_4B;	// 13
	uint32  field_4F;	// 14
	uint32  field_6B;	// 15
	int     player_z;	// 16	// Offs = 0x63
	uint32  field_67;	// 17
	uint32  field_6F;	// 18
	int     nLifeMode;	// 19
	int     nView;		// 20
	int8    weapon;		// 0xA4	// Current weapon
	int     weaponTimer;	// 21	// 0xA6
	int     weaponCallback;	// 22	// 0x26
	bool    fLoopQAV;	// 0x186
	uint8 filler[787-8-(23*4)-2];
};
//CT_ASSERT(sizeof(PLAYER) == 787);
#pragma pack(pop)

typedef char message_t[64];

struct LEVELINFO
{
	char zLevelName[144];
	char title[32];
	char author[32];
	char zSong[16];
	int  track;
	int  ending_a;
	int  ending_b;
	message_t messages[32];
	char fog;
	char weather;
};

struct EPISODEINFO
{
	char title[32];
	int  nLevels;
	LEVELINFO levels[16];
};

struct POINT2D
{
	int32 x, y;
};

struct POINT3D
{
	int32 x, y, z;
};

struct PROFILE
{
	int   nSkill;
	uint8 filler[12];
};	// 16 bytes

#define kFrameTicks 4

// Global game variables
extern GAMEOPTIONS gGameOptions;	// Current game options

// These 4 should not be global (?)
extern const GAMEOPTIONS gSingleGameOptions;
extern const GAMEOPTIONS gBloodBathGameOptions;
extern const GAMEOPTIONS gCoopGameOptions;
extern const GAMEOPTIONS gTeamGameOptions;

extern bool    gQuitGame;
extern bool    gQuitRequest;
extern bool    gGameStarted;
extern bool    gStartNewGame;
extern bool    gPaused;
extern int     gFrame;
extern int     gFrameClock;
extern int     gInputMode;
extern int     gViewPos;
extern int     gViewMode;
extern int     gGamma;
extern int     gGammaLevels;
extern int     gSkill;
extern int     gNextLevel;
extern int     gGameClock;
extern int     gNetFifoClock;
extern NEW_INPUT gInput;
extern bool    gAutoRun;
extern int     gYLookup[MAX_YLOOKUPS];
extern bool    bAddUserMap;

extern int16 gUpperLink[MAXSECTORS];
extern int16 gLowerLink[MAXSECTORS];

// Trigger related stuff (TODO: Move)
extern POINT2D baseWall[MAXWALLS];
extern POINT3D baseSprite[MAXSPRITES];

extern int baseFloor[MAXSECTORS];
extern int baseCeil[MAXSECTORS];

// View related stuff (TODO: Move)
extern int nInterpolations;

extern bool  gInterpolateSprite[512];
extern bool  gInterpolateWall[1024];
extern uint8 gInterpolateSector[128];

extern int gInterpolate;
extern bool gViewInterpolate;	// Defaults to true

struct INTERPOLATE
{
	void   *ptr;
	uint32 start_value;	// FIXME: Signed/unsigned?
	uint32 prev_value;	// FIXME: Same as above
	uint8  type;
};

extern INTERPOLATE gInterpolation[4096];

// TODO: Decrease size?
extern int32 xvel[MAXSPRITES];
extern int32 yvel[MAXSPRITES];
extern int32 zvel[MAXSPRITES];

#pragma pack(push, 1)
struct VIEW
{
	uint32 field_0;
	uint32 field_4;
	uint32 field_8;
	uint32 field_C;
	uint32 field_10;
	uint32 field_14;
	uint32 field_18;
	uint32 field_1C;
	uint32 field_20;
	uint32 field_24;
	uint32 field_28;
	int32  ang;
	uint32 field_30;
	int32  another_z;
	uint32 field_38;
	uint32 field_3C;
	uint32 field_40;
	int    nSprite;
	int32  x;
	int32  y;
	int32  z;
	int32  xvel;
	int32  yvel;
	int32  zvel;
	int16  sectnum;
	uint32 field_62;
	uint8  field_66;
	uint8  field_67;
	uint8  field_68;
	uint16 flags;
	uint32 ccode;
	uint32 field_6F;
	uint32 field_73;
};
CT_ASSERT(sizeof(VIEW) == 119);

struct POSTURE
{
	int32 field_0;
	int32 field_4;
	int32 field_8;
	int32 field_C;
	int32 field_10;
	int32 field_14;
	int32 field_18;
	int32 field_1C;
	int32 field_20;
	int32 field_24;
	int32 field_28;
};
CT_ASSERT(sizeof(POSTURE) == 44);
#pragma pack(pop)

extern VIEW predict;
extern VIEW predictOld;

extern VIEW predictFifo[256];
extern int gPredictTail;

extern NEW_INPUT gFifoInput[2048];
extern int gNetFifoHead[8];

extern PROFILE gProfile[8];
extern PLAYER  gPlayer[8];
extern PLAYER *gView;
extern PLAYER *gMe;
extern int     gViewIndex;
extern VIEW    gPrevView[8];

extern const POSTURE gPosture[2][3];

extern EPISODEINFO gEpisodeInfo[5];
extern int gEpisodeCount;

extern int gSecretsFound[2];
extern int gSecrets;

// These might be engine globals
extern bool ready2send;
//extern int  startposx;
//extern int  startposy;
//extern int  startposz;
//extern int  startang;
//extern int  startsectnum;

#define MAX_XSECTORS 2048
#define MAX_XWALLS   2048
#define MAX_XSPRITES 2048

extern XSECTOR xsector[MAX_XSECTORS];
extern XWALL   xwall[MAX_XWALLS];
extern XSPRITE xsprite[MAX_XSPRITES];

// For doing fire with
extern uint8 *gCLU;

void StartLevel( GAMEOPTIONS *pOptions );
void StartNetworkLevel();
void levelStartGame();
void ProcessFrame();

struct PLAYERTEMP
{
	int32  x, y, z;
	uint32 ang;
	int    sector;
};
extern PLAYERTEMP gPlayerTemp;

// Debugging stuff, remove this later
#define NUM_DEBUG_INTERPOLATIONS 32
extern int nCurrentInterpolate;
extern int gDebugInterpolates[NUM_DEBUG_INTERPOLATIONS];

extern int32 rposx, rposy, rposz, rang, rsector;	// Last render position/angle/sector

struct ZONE
{
	int32  x;
	int32  y;
	int32  z;
	uint16 sector;
	uint16 ang;
};

extern ZONE gStartZone[8];

struct SUICIDE
{
	const char *pzMessage;
	int wavId;
};

struct VICTORY
{
	const char *pzMessage;
	int wavId;
};

extern const SUICIDE gSuicide[6];
extern const VICTORY gVictory[26];

extern int gDetail;
extern int MusicVolume;
extern int FXVolume;
extern int gMouseSensitivity;
extern int gTurnSpeed;
extern int gMessageCount;
extern int gMessageTime;
extern int gAimReticle;
extern int gShowWeapon;
extern int gSlopeTilting;
extern int gViewVBobbing;
extern int gViewHBobbing;

extern int numplayers;

// Interface callbacks
class CGameMenuItemSlider;
class CGameMenuItemZBool;
class CGameMenuItemChain;
class CGameMenuItemZEdit;
class CGameMenuEvent;

void SetGamma(CGameMenuItemSlider *pSlider);
void SetMusicVol(CGameMenuItemSlider *pSlider);
void SetSoundVol(CGameMenuItemSlider *pSlider);
void SetCDVol(CGameMenuItemSlider *pSlider);
void SetMouseSensitivity(CGameMenuItemSlider *pSlider);
void SetCrosshair(CGameMenuItemZBool *pBool);
void SetShowWeapons(CGameMenuItemZBool *pBool);
void SetSlopeTilting(CGameMenuItemZBool *pBool);
void SetViewBobbing(CGameMenuItemZBool *pBool);
void SetViewSwaying(CGameMenuItemZBool *pBool);
void SetMessages(CGameMenuItemZBool *pBool);
void SetParentalLock(CGameMenuItemZBool *pBool);
void Quit(CGameMenuItemChain *pChain);
void SetDifficultyAndStart(CGameMenuItemChain *pChain);
void StartNetGame(CGameMenuItemChain *pChain);
void SaveGame(CGameMenuItemZEdit *pEdit, CGameMenuEvent *pEvent);
void LoadGame(CGameMenuItemZEdit *pEdit, CGameMenuEvent *pEvent);

#endif // __GAME_H__
