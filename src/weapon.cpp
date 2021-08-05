#include "stdafx.h"
#include "game.h"
#include "qav.h"
#include "resource.h"
#include "sfx.h"
#include "debug.h"
#include "error.h"


#define kQAVEnd 113
#define kMaxClients 64


typedef void (*pfnCallback)(int, PLAYER *);


static QAV *weaponQAV[kQAVEnd];
static pfnCallback clientCallback[kMaxClients];


static int nClientFirePitchfork;
static int nClientFireSpray;
static int nClientThrowCan;
static int nClientDropCan;
static int nClientExplodeCan;
static int nClientThrowBundle;
static int nClientDropBundle;
static int nClientExplodeBundle;
static int nClientThrowProx;
static int nClientDropProx;
static int nClientThrowRemote;
static int nClientDropRemote;
static int nClientFireRemote;
static int nClientFireShotgun;
static int nClientEjectShell;
static int nClientFireTommy;
static int nClientFireSpread;
static int nClientAltFireSpread;
static int nClientFireFlare;
static int nClientAltFireFlare;
static int nClientFireVoodoo;
static int nClientFireSpear;
static int nClientFireShadow;
static int nClientAltFireShadow;
static int nClientFireReanimator;
static int nClientFireStaff;
static int nClientFireStaff2;
static int nClientFireBeast;


static void FreeQAVs()
{
	dprintf("Freeing QAVs...\n");
	for (int i = 0; i < kQAVEnd; i++)
		delete weaponQAV[i];
}


static void FirePitchfork(int unused, PLAYER *pPlayer)
{
}


static void FireSpray(int unused, PLAYER *pPlayer)
{
}


static void ThrowCan(int unused, PLAYER *pPlayer)
{
}


static void DropCan(int unused, PLAYER *pPlayer)
{
}


static void ExplodeCan(int unused, PLAYER *pPlayer)
{
}


static void ThrowBundle(int unused, PLAYER *pPlayer)
{
}


static void DropBundle(int unused, PLAYER *pPlayer)
{
}


static void ExplodeBundle(int unused, PLAYER *pPlayer)
{
}


static void ThrowProx(int unused, PLAYER *pPlayer)
{
}


static void DropProx(int unused, PLAYER *pPlayer)
{
}


static void ThrowRemote(int unused, PLAYER *pPlayer)
{
}


static void DropRemote(int unused, PLAYER *pPlayer)
{
}


static void FireRemote(int unused, PLAYER *pPlayer)
{
}


#define kMaxShotgunBarrels 4	// This was defined as 4 in the game... seriously
static void FireShotgun(int nTrigger, PLAYER *pPlayer)
{
	dassert(nTrigger > 0 && nTrigger <= kMaxShotgunBarrels);
	if (nTrigger == 1)
	{
		sfxPlay3DSound(pPlayer->pSprite, 411, 1, 0);
	}
	else
	{
		sfxPlay3DSound(pPlayer->pSprite, 412, 1, 0);
	}
}


static void EjectShell(int unused, PLAYER *pPlayer)
{
}


static void FireTommy(int unused, PLAYER *pPlayer)
{
}


static void FireSpread(int unused, PLAYER *pPlayer)
{
}


static void AltFireSpread(int unused, PLAYER *pPlayer)
{
}


static void FireFlare(int unused, PLAYER *pPlayer)
{
}


static void AltFireFlare(int unused, PLAYER *pPlayer)
{
}


static void FireVoodoo(int unused, PLAYER *pPlayer)
{
}


static void FireSpear(int unused, PLAYER *pPlayer)
{
}


static void FireShadow(int unused, PLAYER *pPlayer)
{
}


static void AltFireShadow(int unused, PLAYER *pPlayer)
{
}


static void FireReanimator(int unused, PLAYER *pPlayer)
{
}


static void FireStaff(int unused, PLAYER *pPlayer)
{
}


static void FireStaff2(int unused, PLAYER *pPlayer)
{
}


static void FireBeast(int unused, PLAYER *pPlayer)
{
}


static int qavRegisterClient(void (*pCallback)(int, PLAYER *))
{
	static int nClients = 0;

	int res = nClients;
	clientCallback[nClients] = pCallback;
	nClients++;

	return res;
}


void WeaponInit()
{
	for (int i = 0; i < kQAVEnd; i++)
	{
		ResHandle h = gSysRes.Lookup(i, "QAV");
		if (h.isNull())
			ThrowError("Could not load QAV %d\n", i);

		weaponQAV[i] = new QAV((QAVDATA *)gSysRes.Lock(h));
	}
	atexit(FreeQAVs);

	nClientFirePitchfork  = qavRegisterClient(FirePitchfork);
	nClientFireSpray      = qavRegisterClient(FireSpray);
	nClientThrowCan       = qavRegisterClient(ThrowCan);
	nClientDropCan        = qavRegisterClient(DropCan);
	nClientExplodeCan     = qavRegisterClient(ExplodeCan);
	nClientThrowBundle    = qavRegisterClient(ThrowBundle);
	nClientDropBundle     = qavRegisterClient(DropBundle);
	nClientExplodeBundle  = qavRegisterClient(ExplodeBundle);
	nClientThrowProx      = qavRegisterClient(ThrowProx);
	nClientDropProx       = qavRegisterClient(DropProx);
	nClientThrowRemote    = qavRegisterClient(ThrowRemote);
	nClientDropRemote     = qavRegisterClient(DropRemote);
	nClientFireRemote     = qavRegisterClient(FireRemote);
	nClientFireShotgun    = qavRegisterClient(FireShotgun);
	nClientEjectShell     = qavRegisterClient(EjectShell);
	nClientFireTommy      = qavRegisterClient(FireTommy);
	nClientFireSpread     = qavRegisterClient(FireSpread);
	nClientAltFireSpread  = qavRegisterClient(AltFireSpread);
	nClientFireFlare      = qavRegisterClient(FireFlare);
	nClientAltFireFlare   = qavRegisterClient(AltFireFlare);
	nClientFireVoodoo     = qavRegisterClient(FireVoodoo);
	nClientFireSpear      = qavRegisterClient(FireSpear);
	nClientFireShadow     = qavRegisterClient(FireShadow);
	nClientAltFireShadow  = qavRegisterClient(AltFireShadow);
	nClientFireReanimator = qavRegisterClient(FireReanimator);
	nClientFireStaff      = qavRegisterClient(FireStaff);
	nClientFireStaff2     = qavRegisterClient(FireStaff2);
	nClientFireBeast      = qavRegisterClient(FireBeast);
}


void WeaponDraw(PLAYER *pPlayer, int a, int b, int c)
{
	dassert(pPlayer != NULL);
//	int nQAVIndex = pPlayer->nQAVIndex;
	int nQAVIndex = 10;	// TODO: Fix this later
	if (nQAVIndex == -1)
		return;

	QAV *pQAV = weaponQAV[nQAVIndex];
	pQAV->Draw(0, a, c);
}


void WeaponPlay(PLAYER *pPlayer)
{
	dassert(pPlayer != NULL);
	if (pPlayer->nQAVIndex == -1)
		return;

	QAV *pQAV = weaponQAV[pPlayer->nQAVIndex];
	int nPlayTime = pQAV->GetDuration() - pPlayer->weaponTimer;
	pQAV->Play(nPlayTime - 4, nPlayTime, pPlayer->weaponCallback, pPlayer);
}


void WeaponProcess(PLAYER *pPlayer)
{
	WeaponPlay(pPlayer);

	// TODO: Implement auto aiming?
}


static void StartQAV(PLAYER *pPlayer, int nWeaponQAV, int callback, bool fLoop)
{
	dassert(nWeaponQAV < kQAVEnd);
	pPlayer->nQAVIndex      = nWeaponQAV;
	pPlayer->weaponTimer    = weaponQAV[nWeaponQAV]->GetDuration();
	pPlayer->weaponCallback = callback;
	pPlayer->fLoopQAV       = fLoop;

	weaponQAV[nWeaponQAV]->Preload();
	pPlayer->weaponTimer -= 4;
}
