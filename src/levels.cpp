#include "stdafx.h"
#include "game.h"
#include "ini_file.h"
#include "player.h"
#include "db.h"
#include "error.h"
#include "debug.h"

#include "jfbuild/compat.h"


#define kMaxMessages 32


void levelClearSecrets()
{
	gSecrets = 0;
	memset(gSecretsFound, 0, sizeof(gSecretsFound));
}


void levelSetupSecret(int nSecret)
{
	gSecrets = nSecret;
}


void levelTriggerSecret(int nSecret)
{
	if (nSecret < 0 || nSecret >= 2)
		ThrowError("Invalid secret type %d triggered.", nSecret);
	gSecretsFound[nSecret]++;
	if (gGameOptions.nGameType == 0)
	{
		// TODO: More stuff here
	}
}


void CheckSectionAbend(const char *pzSection)
{
	if (!pzSection || !BloodINI.FindSection(pzSection))
		ThrowError("Section [%s] expected in BLOOD.INI");
}


// Unused in beta
void CheckKeyAbend(char *a, char *b)
{
}


LEVELINFO *levelGetInfoPtr(int nEpisode, int nLevel)
{
	dassert(nEpisode >= 0 && nEpisode < gEpisodeCount);
	EPISODEINFO *pEpisodeInfo = &gEpisodeInfo[nEpisode];
	dassert(nLevel >= 0 && nLevel < pEpisodeInfo->nLevels);
	return &pEpisodeInfo->levels[nLevel];
}


// Unused in beta
const char *levelGetFilename(int a, int b)
{
	return NULL;
}


const char *levelGetMessage(int nMessage)
{
	dassert(nMessage < kMaxMessages);
	// TODO
	return NULL;
}


const char *levelGetTitle()
{
	LEVELINFO *pLevelInfo = &gEpisodeInfo[gGameOptions.nEpisode].levels[gGameOptions.nLevel];
	if (pLevelInfo->title[0])
		return pLevelInfo->title;
	return NULL;
}


// Unused in beta
const char *levelGetAuthor()
{
	return NULL;
}


void levelSetupOptions(int nEpisode, int nLevel)
{
	LEVELINFO *pLevelInfo = &gEpisodeInfo[nEpisode].levels[nLevel];
	gGameOptions.nEpisode = nEpisode;
	gGameOptions.nLevel   = nLevel;
	Bsnprintf(gGameOptions.zLevelName, sizeof(gGameOptions.zLevelName), "%s", pLevelInfo->zLevelName);
	gGameOptions.uMapCRC = dbReadMapCRC(pLevelInfo->zLevelName);
	Bsnprintf(gGameOptions.zLevelSong, sizeof(gGameOptions.zLevelSong), "%s", pLevelInfo->zSong);
	gGameOptions.nTrackNumber = 0;	// TODO
}


void levelLoadMapInfo(IniFile *pIniFile, LEVELINFO *pLevelInfo, char *foo)
{
}


void levelLoadDefaults()
{
	char section[64];
	char key[16];

	dprintf("levelLoadDefaults()\n");
	memset(gEpisodeInfo, 0, sizeof(gEpisodeInfo));
	int nEpisode = 0;
	while (true)
	{
		if (nEpisode >= 4)
			break;

		Bsnprintf(section, sizeof(section) - 1, "Episode%d", nEpisode + 1);
		if (!BloodINI.FindSection(section))
			break;

		EPISODEINFO *pInfo = &gEpisodeInfo[nEpisode];
		Bsnprintf(pInfo->title, sizeof(pInfo->title) - 1, "%s", BloodINI.GetKeyString(section, "Title"));

		int nLevel = 0;
		while (nLevel < 16)
		{
			LEVELINFO *pLevelInfo = &pInfo->levels[nLevel];
			Bsnprintf(key, sizeof(key) - 1, "Map%d", nLevel + 1);
			if (!BloodINI.KeyExists(section, key))
				break;
			const char *lol = BloodINI.GetKeyString(section, key);
			Bsnprintf(pLevelInfo->zLevelName, sizeof(pLevelInfo->zLevelName) - 1, "%s", lol);
			nLevel++;
		}
		nEpisode++;
		pInfo->nLevels = nLevel;
	}
	gEpisodeCount = nEpisode;
}


void levelAddUserMap(const char *map)
{
}


void levelGetNextLevels(int nEpisode, int nLevel, int *pnEndingA, int *pnEndingB)
{
	dassert(pnEndingA != NULL && pnEndingB != NULL);
	LEVELINFO *pLevelInfo = &gEpisodeInfo[nEpisode].levels[nLevel];
	int ending_a = pLevelInfo->ending_a;
	if (ending_a >= 0)
		ending_a--;
	int ending_b = pLevelInfo->ending_b;
	if (ending_b >= 0)
		ending_b--;
	*pnEndingA = ending_a;
	*pnEndingB = ending_b;
}


void levelEndLevel(int foo)
{
}


void levelStartGame()
{
	dprintf("*** levelStartGame()\n");
	for (int i = 0; i < 8; i++)
	{
		if (gGameOptions.nGameType > GT_SINGLE || numplayers == 1)
			gProfile[i].nSkill = gSkill;
		playerInit(i, 0);
	}
	levelSetupOptions(gGameOptions.nEpisode, gGameOptions.nLevel);
	StartLevel(&gGameOptions);
}


void levelRestart()
{
}
