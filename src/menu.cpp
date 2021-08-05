#include "stdafx.h"
#include "game.h"
#include "resource.h"
#include "gamemenu.h"
#include "debug.h"


static void SetupLoadingScreen()
{
	menuLoading.Add(&itemLoadingText, true);
	menuLoading.Add(&itemBloodQAV, false);
}


static void SetupKeyListMenu()
{
	menuKeys.Add(&itemKeysTitle, false);
	menuKeys.Add(&itemKeyList, true);
	menuKeys.Add(&itemBloodQAV, false);
}


static void SetupMessagesMenu()
{
	menuMessages.Add(&itemMessagesTitle, false);
	menuMessages.Add(&boolMessages, true);
	menuMessages.Add(&sliderMsgCount, false);
	menuMessages.Add(&sliderMsgTime, false);
	menuMessages.Add(&boolMsgFont, false);
	menuMessages.Add(&boolMsgIncoming, false);
	menuMessages.Add(&boolMsgSelf, false);
	menuMessages.Add(&boolMsgOther, false);
	menuMessages.Add(&boolMsgRespawn, false);
	menuMessages.Add(&itemBloodQAV, false);
}


static void SetupControlsMenu()
{
	// Clamp and set mouse sensitivity
	sliderMouseSpeed.m_nValue = Clamp(gMouseSensitivity, sliderMouseSpeed.m_nMin, sliderMouseSpeed.m_nMax);

	menuControls.Add(&itemControlsTitle, false);
	menuControls.Add(&sliderMouseSpeed, true);
	menuControls.Add(&sliderTurnSpeed, false);
	menuControls.Add(&itemChainKeyList, false);
	menuControls.Add(&itemBloodQAV, false);
}


static void SetupSaveGameMenu()
{
	menuSaveGame.Add(&itemSaveTitle, false);
	menuSaveGame.Add(&itemSaveGame1, true);
	menuSaveGame.Add(&itemSaveGame2, false);
	menuSaveGame.Add(&itemSaveGame3, false);
	menuSaveGame.Add(&itemSaveGame4, false);
	menuSaveGame.Add(&itemSaveGame5, false);
	menuSaveGame.Add(&itemSaveGame6, false);
	menuSaveGame.Add(&itemSaveGame7, false);
	menuSaveGame.Add(&itemSaveGame8, false);
	menuSaveGame.Add(&itemSaveGame9, false);
	menuSaveGame.Add(&itemSaveGame10, false);
	menuSaveGame.Add(&itemSaveGamePic, false);
	menuSaveGame.Add(&itemBloodQAV, false);
}


static void SetupLoadGameMenu()
{
	menuLoadGame.Add(&itemLoadTitle, false);
	menuLoadGame.Add(&itemLoadGame1, true);
	menuLoadGame.Add(&itemLoadGame2, false);
	menuLoadGame.Add(&itemLoadGame3, false);
	menuLoadGame.Add(&itemLoadGame4, false);
	menuLoadGame.Add(&itemLoadGame5, false);
	menuLoadGame.Add(&itemLoadGame6, false);
	menuLoadGame.Add(&itemLoadGame7, false);
	menuLoadGame.Add(&itemLoadGame8, false);
	menuLoadGame.Add(&itemLoadGame9, false);
	menuLoadGame.Add(&itemLoadGame10, false);
	menuLoadGame.Add(&itemLoadGamePic, false);
	itemLoadGame1.field_31 = 0;
	itemLoadGame2.field_31 = 0;
	itemLoadGame3.field_31 = 0;
	itemLoadGame4.field_31 = 0;
	itemLoadGame5.field_31 = 0;
	itemLoadGame6.field_31 = 0;
	itemLoadGame7.field_31 = 0;
	itemLoadGame8.field_31 = 0;
	itemLoadGame9.field_31 = 0;
	itemLoadGame10.field_31 = 0;
	menuLoadGame.Add(&itemBloodQAV, false);
}


static void SetupDifficultyMenu()
{
	menuDifficulty.Add(&itemDifficultyTitle, false);
	menuDifficulty.Add(&itemDifficulty1, false);
	menuDifficulty.Add(&itemDifficulty2, false);
	menuDifficulty.Add(&itemDifficulty3, true);
	menuDifficulty.Add(&itemDifficulty4, false);
	menuDifficulty.Add(&itemDifficulty5, false);
	menuDifficulty.Add(&itemBloodQAV, false);
}


static void SetupLevelMenuItem(int nEpisode)
{
	dassert(nEpisode >= 0 && nEpisode < gEpisodeCount);

}


static void SetupOptionsMenu()
{
	// Clamp and set slider values
	sliderDetail.m_nValue = Clamp(gDetail, sliderDetail.m_nMin, sliderDetail.m_nMax);
	sliderGamma.m_nValue  = Clamp(gGamma, sliderGamma.m_nMin, sliderGamma.m_nMax);
	sliderMusic.m_nValue  = Clamp(MusicVolume, sliderMusic.m_nMin, sliderMusic.m_nMax);
	sliderSound.m_nValue  = Clamp(FXVolume, sliderSound.m_nMin, sliderSound.m_nMax);
	// TODO: Redbook?

	// Set bool values
	boolCrosshair.m_field_20    = gAimReticle;
	boolShowWeapons.m_field_20  = gShowWeapon;
	boolSlopeTilting.m_field_20 = gSlopeTilting;
	boolViewBobbing.m_field_20  = gViewVBobbing;
	boolViewSwaying.m_field_20  = gViewHBobbing;

	// TODO: Fix the following
//	boolMessages.m_field_20     = gGameMessageMgr.field_0;

	menuOptions.Add(&itemOptionsTitle, false);
	menuOptions.Add(&itemOption1, true);
	menuOptions.Add(&sliderDetail, false);
	menuOptions.Add(&sliderGamma, false);
	menuOptions.Add(&sliderMusic, false);
	menuOptions.Add(&sliderSound, false);
	menuOptions.Add(&sliderCDAudio, false);
	menuOptions.Add(&boolCrosshair, false);
	menuOptions.Add(&boolShowWeapons, false);
	menuOptions.Add(&boolSlopeTilting, false);
	menuOptions.Add(&boolViewBobbing, false);
	menuOptions.Add(&boolViewSwaying, false);
	menuOptions.Add(&itemChainParentalLock, false);
	menuOptions.Add(&itemBloodQAV, false);
}


static void SetupEpisodeMenu()
{
	menuEpisode.Add(&itemEpisodeTitle, 0);
	int nFoo;
	gMenuTextMgr.GetFontInfo(1, NULL, NULL, &nFoo);
	for (int i = 0; i < 4; i++)
	{
		if (i < gEpisodeCount)
		{
			CGameMenuItemChain *pItem = &itemEpisodes[i];
			pItem->m_nFontId  = 1;
			pItem->m_x        = 0;
			pItem->m_y        = i * (nFoo + 4) + 45;
			pItem->m_field_14 = 320;
			pItem->m_field_20 = 1;
			pItem->m_field_24 = &menuDifficulty;
			pItem->m_field_28 = 3;
			pItem->m_pzText   = gEpisodeInfo[i].title;
			pItem->m_field_18 |= 3;
			menuEpisode.Add(&itemEpisodes[i], i == 0);
			if (i == 0)
				SetupLevelMenuItem(i);
		}
	}
	menuEpisode.Add(&itemBloodQAV, 0);
}


static void SetupMainMenu()
{
	menuMain.Add(&itemMainTitle, 0);
	menuMain.Add(&itemMain1, 1);
	if (gGameOptions.nGameType > 0)
	{
	}
	menuMain.Add(&itemMain2, 0);
	menuMain.Add(&itemMain3, 0);
	menuMain.Add(&itemMain4, 0);
	menuMain.Add(&itemMain5, 0);
	menuMain.Add(&itemMain6, 0);
	menuMain.Add(&itemBloodQAV, 0);
}


static void SetupMainMenuWithSave()
{
	menuMainWithSave.Add(&itemMainSaveTitle, false);
	menuMainWithSave.Add(&itemMainSave1, true);
	// TODO: Multiplayer stuff
	menuMainWithSave.Add(&itemMainSave2, false);
	menuMainWithSave.Add(&itemMainSave3, false);
	menuMainWithSave.Add(&itemMainSave4, false);
	menuMainWithSave.Add(&itemMainSave5, false);
	menuMainWithSave.Add(&itemMainSave6, false);
	menuMainWithSave.Add(&itemMainSave7, false);
	menuMainWithSave.Add(&itemBloodQAV, false);
}


static void SetupQuitMenu()
{
	menuQuit.Add(&itemQuitTitle, false);
	menuQuit.Add(&itemQuitText1, false);
	menuQuit.Add(&itemQuitYesNo, true);
	menuQuit.Add(&itemBloodQAV, false);
}


void SetupMenus()
{
	SetupLoadingScreen();	// Done
	SetupKeyListMenu();		// Done
	SetupMessagesMenu();	// Done
	SetupControlsMenu();	// Done
	SetupSaveGameMenu();	// Done
	SetupLoadGameMenu();	// Done
	SetupOptionsMenu();		// Done
	SetupDifficultyMenu();	// Done
	SetupEpisodeMenu();		// Done
	SetupMainMenu();
	SetupMainMenuWithSave();
	SetupQuitMenu();
}
