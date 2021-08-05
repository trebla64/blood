#include "stdafx.h"
#include "resource.h"
#include "gamemenu.h"
#include "game.h"
#include "key.h"
#include "tile.h"
#include "view.h"
#include "debug.h"

#include "jfbuild/build.h"
#include "jfbuild/baselayer.h"

bool CGameMenuMgr::m_bActive = false;
CGameMenuMgr gGameMenuMgr;
CMenuTextMgr gMenuTextMgr;


static const char * const kpzOnDefault  = "On";
static const char * const kpzOffDefault = "Off";


// Game menus
CGameMenu menuMain;
CGameMenu menuLevelStat;
CGameMenu menuFragStat;
CGameMenu menuMainWithSave;
CGameMenu menuNetMain;
CGameMenu menuNetStart;
CGameMenu menuEpisode;
CGameMenu menuDifficulty;
CGameMenu menuOptions;
CGameMenu menuControls;
CGameMenu menuMessages;
CGameMenu menuKeys;
CGameMenu menuSaveGame;
CGameMenu menuLoadGame;
CGameMenu menuLoading;
CGameMenu menuSounds;
CGameMenu menuQuit;
CGameMenu menuCredits;
CGameMenu menuOrder;
CGameMenu menuParentalLock;
CGameMenu menuSorry;

// Titles
CGameMenuItemTitle itemMainTitle("BLOOD", 1, 160, 20, 2038);
CGameMenuItemTitle itemMainSaveTitle("BLOOD", 1, 160, 20, 2038);
CGameMenuItemTitle itemEpisodeTitle("EPISODES", 1, 160, 20, 2038);
CGameMenuItemTitle itemDifficultyTitle("DIFFICULTY", 1, 160, 20, 2038);
CGameMenuItemTitle itemOptionsTitle("OPTIONS", 1, 160, 20, 2038);
CGameMenuItemTitle itemControlsTitle("CONTROLS", 1, 160, 20, 2038);
CGameMenuItemTitle itemMessagesTitle("MESSAGES", 1, 160, 20, 2038);
CGameMenuItemTitle itemKeysTitle("KEY SETUP", 1, 160, 20, 2038);
CGameMenuItemTitle itemSaveTitle("Save Game", 1, 160, 20, 2038);
CGameMenuItemTitle itemLoadTitle("Load Game", 1, 160, 20, 2038);
CGameMenuItemTitle itemNetStartTitle("NETWORK GAME", 1, 160, 20, 2038);
CGameMenuItemTitle itemSoundsTitle("SOUNDS", 1, 160, 20, 2038);
CGameMenuItemTitle itemQuitTitle("QUIT", 1, 160, 20, 2038);
CGameMenuItemTitle itemParentalLockTitle("PARENTAL LOCK", 1, 160, 20, 2038);

// Chains
CGameMenuItemChain itemMain1("NEW GAME", 1, 0, 45, 320, 1, &menuEpisode, -1, 0, 0);
CGameMenuItemChain itemMain2("OPTIONS", 1, 0, 65, 320, 1, &menuOptions, -1, 0, 0);
CGameMenuItemChain itemMain3("LOAD GAME", 1, 0, 85, 320, 1, &menuLoadGame, -1, 0, 0);
CGameMenuItemChain itemMain4("HELP / ORDERING", 1, 0, 105, 320, 1, &menuOrder, -1, 0, 0);
CGameMenuItemChain itemMain5("CREDITS", 1, 0, 125, 320, 1, &menuCredits, -1, 0, 0);
CGameMenuItemChain itemMain6("QUIT", 1, 0, 145, 320, 1, &menuQuit, -1, 0, 0);
CGameMenuItemChain itemMainSave1("NEW GAME", 1, 0, 45, 320, 1, &menuEpisode, -1, 0, 0);
CGameMenuItemChain itemMainSave2("OPTIONS", 1, 0, 65, 320, 1, &menuOptions, -1, 0, 0);
CGameMenuItemChain itemMainSave3("SAVE GAME", 1, 0, 85, 320, 1, &menuSaveGame, -1, 0, 0);
CGameMenuItemChain itemMainSave4("LOAD GAME", 1, 0, 105, 320, 1, &menuLoadGame, -1, 0, 0);
CGameMenuItemChain itemMainSave5("HELP / ORDERING", 1, 0, 125, 320, 1, &menuOrder, -1, 0, 0);
CGameMenuItemChain itemMainSave6("CREDITS", 1, 0, 145, 320, 1, &menuCredits, -1, 0, 0);
CGameMenuItemChain itemMainSave7("QUIT", 1, 0, 165, 320, 1, &menuQuit, -1, 0, 0);
CGameMenuItemChain itemDifficulty1("STILL KICKING", 1, 0, 60, 320, 1, 0, -1, SetDifficultyAndStart, 0);
CGameMenuItemChain itemDifficulty2("PINK ON THE INSIDE", 1, 0, 80, 320, 1, 0, -1, SetDifficultyAndStart, 1);
CGameMenuItemChain itemDifficulty3("LIGHTLY BROILED", 1, 0, 100, 320, 1, 0, -1, SetDifficultyAndStart, 2);
CGameMenuItemChain itemDifficulty4("WELL DONE", 1, 0, 120, 320, 1, 0, -1, SetDifficultyAndStart, 3);
CGameMenuItemChain itemDifficulty5("EXTRA CRISPY", 1, 0, 140, 320, 1, 0, -1, SetDifficultyAndStart, 4);
CGameMenuItemChain itemOption1("CONTROLS...", 3, 0, 50, 320, 1, &menuControls, -1, 0, 0);
CGameMenuItemChain itemChainParentalLock("PARENTAL LOCK", 3, 0, 160, 320, 1, &menuParentalLock, -1, 0, 0);
CGameMenuItemChain itemChainKeyList("Configure Keys...", 1, 0, 110, 320, 1, &menuKeys, -1, 0, 0);
CGameMenuItemChain itemNetStart9("START GAME", 1, 20, 155, 200, 0, 0, -1, StartNetGame, 0);
CGameMenuItemChain itemEpisodes[4] =
{
	CGameMenuItemChain(NULL, 0, 0, 0, 0, 0, 0, 0, NULL, 0),
	CGameMenuItemChain(NULL, 0, 0, 0, 0, 0, 0, 0, NULL, 0),
	CGameMenuItemChain(NULL, 0, 0, 0, 0, 0, 0, 0, NULL, 0),
	CGameMenuItemChain(NULL, 0, 0, 0, 0, 0, 0, 0, NULL, 0)
};

// Text
CGameMenuItemText  itemLoadingText("LOADING...", 1, 160, 100, 1);
CGameMenuItemText  itemQuitText1("Do you really want to quit?", 0, 160, 100, 1);
CGameMenuItemText  itemSorryText1("Sorry, loading and saving games", 0, 160, 90, 1);
CGameMenuItemText  itemSorryText2("are not supported", 0, 160, 100, 1);
CGameMenuItemText  itemSorryText3("in this demo version of Blood.", 0, 160, 110, 1);

// QAV
CGameMenuItemQAV itemBloodQAV("", 3, 160, 100, "BDRIP");
CGameMenuItemQAV itemCreditsQAV("", 3, 160, 100, "CREDITS");
CGameMenuItemQAV itemHelp1QAV("", 3, 160, 100, "HELP1");
CGameMenuItemQAV itemHelp2QAV("", 3, 160, 100, "HELP2");
CGameMenuItemQAV itemHelp3QAV("", 3, 160, 100, "HELP3");
CGameMenuItemQAV itemHelp3BQAV("", 3, 160, 100, "HELP3B");

// Lists
CGameMenuItemKeyList itemKeyList("", 3, 56, 40, 200, 16, 54, NULL);

// Sliders
CGameMenuItemSlider sliderDetail("DETAIL:", 3, 66, 60, 180, gDetail, 0, 4, 1, NULL, -1, -1);
CGameMenuItemSlider sliderGamma("GAMMA:", 3, 66, 70, 180, gGamma, 0, 15, 2, SetGamma, -1, -1);
CGameMenuItemSlider sliderMusic("MUSIC:", 3, 66, 80, 180, MusicVolume, 0, 256, 48, SetMusicVol, -1, -1);
CGameMenuItemSlider sliderSound("SOUND:", 3, 66, 90, 180, FXVolume, 0, 256, 48, SetSoundVol, -1, -1);
CGameMenuItemSlider sliderCDAudio("CD AUDIO:", 3, 66, 100, 180, 0, 0, 256, 48, SetCDVol, -1, -1);
CGameMenuItemSlider sliderMouseSpeed("Mouse Sensitivity:", 1, 10, 70, 300, gMouseSensitivity, 0, 131072, 4096, SetMouseSensitivity, -1, -1);
CGameMenuItemSlider sliderTurnSpeed("Key Turn Speed:", 1, 10, 90, 300, gTurnSpeed, 64, 128, 4, NULL, -1, -1);
CGameMenuItemSlider sliderMsgCount("MESSAGE COUNT:", 3, 66, 80, 180, gMessageCount, 1, 16, 1, NULL, -1, -1);
CGameMenuItemSlider sliderMsgTime("MESSAGE TIME:", 3, 66, 90, 180, gMessageTime, 1, 8, 1, NULL, -1, -1);
CGameMenuItemSlider sliderSoundsMusic("MUSIC:", 3, 40, 60, 180, MusicVolume, 0, 256, 48, SetMusicVol, -1, -1);
CGameMenuItemSlider sliderSoundsSound("SOUND:", 3, 40, 70, 180, FXVolume, 0, 256, 48, SetSoundVol, -1, -1);
CGameMenuItemSlider sliderSoundsCDAudio("CD AUDIO:", 3, 40, 80, 180, 0, 0, 256, 48, SetCDVol, -1, -1);

// Bools
CGameMenuItemZBool boolCrosshair("CROSSHAIR:", 3, 66, 110, 180, gAimReticle, SetCrosshair, 0, 0);
CGameMenuItemZBool boolShowWeapons("SHOW WEAPONS:", 3, 66, 120, 180, gShowWeapon, SetShowWeapons, 0, 0);
CGameMenuItemZBool boolSlopeTilting("SLOPE TILTING:", 3, 66, 130, 180, gSlopeTilting, SetSlopeTilting, 0, 0);
CGameMenuItemZBool boolViewBobbing("VIEW BOBBING:", 3, 66, 140, 180, gViewVBobbing, SetViewBobbing, 0, 0);
CGameMenuItemZBool boolViewSwaying("VIEW SWAYING:", 3, 66, 150, 180, gViewHBobbing, SetViewSwaying, 0, 0);
CGameMenuItemZBool boolMessages("MESSAGES:", 3, 66, 70, 180, 0, SetMessages, 0, 0);
CGameMenuItemZBool boolMsgFont("LARGE FONT:", 3, 66, 100, 180, 0, NULL, 0, 0);
CGameMenuItemZBool boolMsgIncoming("INCOMING:", 3, 66, 110, 180, 0, NULL, 0, 0);
CGameMenuItemZBool boolMsgSelf("SELF PICKUP:", 3, 66, 120, 180, 0, NULL, 0, 0);
CGameMenuItemZBool boolMsgOther("OTHER PICKUP:", 3, 66, 130, 180, 0, NULL, 0, 0);
CGameMenuItemZBool boolMsgRespawn("RESPAWN:", 3, 66, 140, 180, 0, NULL, 0, 0);
CGameMenuItemZBool itemParentalLockToggle("LOCK:", 3, 66, 70, 180, 0, SetParentalLock, 0, 0);

// ZEdit
CGameMenuItemZEdit itemSaveGame1(0, 3, 80,  60, 320, "Empty", 32, 1, SaveGame, 0);
CGameMenuItemZEdit itemSaveGame2(0, 3, 80,  70, 320, "Empty", 32, 1, SaveGame, 1);
CGameMenuItemZEdit itemSaveGame3(0, 3, 80,  80, 320, "Empty", 32, 1, SaveGame, 2);
CGameMenuItemZEdit itemSaveGame4(0, 3, 80,  90, 320, "Empty", 32, 1, SaveGame, 3);
CGameMenuItemZEdit itemSaveGame5(0, 3, 80, 100, 320, "Empty", 32, 1, SaveGame, 4);
CGameMenuItemZEdit itemSaveGame6(0, 3, 80, 110, 320, "Empty", 32, 1, SaveGame, 5);
CGameMenuItemZEdit itemSaveGame7(0, 3, 80, 120, 320, "Empty", 32, 1, SaveGame, 6);
CGameMenuItemZEdit itemSaveGame8(0, 3, 80, 130, 320, "Empty", 32, 1, SaveGame, 7);
CGameMenuItemZEdit itemSaveGame9(0, 3, 80, 140, 320, "Empty", 32, 1, SaveGame, 8);
CGameMenuItemZEdit itemSaveGame10(0, 3, 80, 150, 320, "Empty", 32, 1, SaveGame, 9);
CGameMenuItemZEdit itemLoadGame1(0, 3, 80,  60, 320, "Empty", 32, 1, LoadGame, 0);
CGameMenuItemZEdit itemLoadGame2(0, 3, 80,  70, 320, "Empty", 32, 1, LoadGame, 1);
CGameMenuItemZEdit itemLoadGame3(0, 3, 80,  80, 320, "Empty", 32, 1, LoadGame, 2);
CGameMenuItemZEdit itemLoadGame4(0, 3, 80,  90, 320, "Empty", 32, 1, LoadGame, 3);
CGameMenuItemZEdit itemLoadGame5(0, 3, 80, 100, 320, "Empty", 32, 1, LoadGame, 4);
CGameMenuItemZEdit itemLoadGame6(0, 3, 80, 110, 320, "Empty", 32, 1, LoadGame, 5);
CGameMenuItemZEdit itemLoadGame7(0, 3, 80, 120, 320, "Empty", 32, 1, LoadGame, 6);
CGameMenuItemZEdit itemLoadGame8(0, 3, 80, 130, 320, "Empty", 32, 1, LoadGame, 7);
CGameMenuItemZEdit itemLoadGame9(0, 3, 80, 140, 320, "Empty", 32, 1, LoadGame, 8);
CGameMenuItemZEdit itemLoadGame10(0, 3, 80, 150, 320, "Empty", 32, 1, LoadGame, 9);
//CGameMenuItemZEdit itemNetStart8("USER MAP:", 1, 20, 140, 280, zUserMapName, 13, 0, 0, 0);

// Pics
CGameMenuItemBitmapLS itemSaveGamePic;
CGameMenuItemBitmapLS itemLoadGamePic;

// Quit
CGameMenuItemYesNoQuit itemQuitYesNo("[Y/N]", 0, 20, 110, 280, 1, -1, 0);

CGameMenu::CGameMenu()
{
	m_nItems = 0;
	field_8  = -1;
	m_nFocus = -1;
}


CGameMenu::~CGameMenu()
{
}


void CGameMenu::SetFocusItem( int nItem )
{
	dassert(nItem >= 0 && nItem < m_nItems && nItem < 32);
	if (CanSelectItem(nItem))
	{
		field_8  = nItem;
		m_nFocus = nItem;
	}
}


bool CGameMenu::IsFocusItem( CGameMenuItem *pItem ) const
{
	dassert(m_nFocus >= 0 && m_nFocus < m_nItems && m_nFocus < 32);
	return (pItem == m_pItems[m_nFocus]);
}


bool CGameMenu::CanSelectItem( int nItem ) const
{
	dassert(nItem >= 0 && nItem < m_nItems && nItem < 32);
	CGameMenuItem *pItem = m_pItems[nItem];
	return ((pItem->m_field_18 & 1) && (pItem->m_field_18 & 2));
}


uint8 CGameMenu::Event( const CGameMenuEvent &event )
{
//	if (event.code) dprintf("CGameMenu got event %u\n", event.code);
	if (m_nItems <= 0)
		return 1;

	if (event.code != 0x8000 && m_nFocus >= 0)
		return m_pItems[m_nFocus]->Event(event);

	if (field_8 >= 0)
		m_nFocus = field_8;
	InitializeItems(event);

	return 0;
}


void CGameMenu::InitializeItems(const CGameMenuEvent &event)
{
	for (int i = 0; i < m_nItems; i++)
		m_pItems[i]->Event(event);
}


void CGameMenu::Draw()
{
	for (int i = 0; i < m_nItems; i++)
		if (i == m_nFocus || i != m_nFocus && !(m_pItems[i]->m_field_18 & 8))
			m_pItems[i]->Draw();
}


void CGameMenu::Add(CGameMenuItem *pItem, bool bSetFocus)
{
	dassert(pItem != NULL);
	dassert(m_nItems < kMaxGameMenuItems);
	m_pItems[m_nItems] = pItem;
	pItem->SetParent(this);
	if (bSetFocus)
	{
		field_8  = m_nItems;
		m_nFocus = m_nItems;
	}
	m_nItems++;
}


void CGameMenu::FocusPrevItem()
{
	dassert(m_nFocus >= -1 && m_nFocus < m_nItems && m_nFocus < 32);
	int nItem;
	do
	{
		nItem = m_nFocus - 1;
		if (nItem < 0)
			nItem += m_nItems;
		m_nFocus = nItem;
	}
	while (!CanSelectItem(nItem));
}


void CGameMenu::FocusNextItem()
{
	dassert(m_nFocus >= -1 && m_nFocus < m_nItems && m_nFocus < 32);
	int nItem;
	do
	{
		nItem = m_nFocus + 1;
		if (nItem >= m_nItems)
			nItem = 0;
		m_nFocus = nItem;
	}
	while (!CanSelectItem(nItem));
}


CGameMenuMgr::CGameMenuMgr()
{
	dassert( !m_bInitialized );
	m_bInitialized = true;
	Clear();
}


CGameMenuMgr::~CGameMenuMgr()
{
	m_bInitialized = false;
	Clear();
}


// TODO: Fixme
void CGameMenuMgr::InitializeMenu()
{
	if ( m_pGameMenu )
		m_pGameMenu->Event(CGameMenuEvent(0x8000));
}


void CGameMenuMgr::Push( CGameMenu *pMenu, int unk )
{
	dassert( pMenu != NULL );
	if ( m_nSubMenus != 8 )
	{
		m_pGameMenu = pMenu;
		m_pSubMenus[ m_nSubMenus ] = pMenu;
		m_nSubMenus++;
		if ( unk >= 0 )
			pMenu->SetFocusItem( unk );
		m_bActive = true;
		gInputMode = 1;
		InitializeMenu();
	}
}


void CGameMenuMgr::Pop()
{
	if (m_nSubMenus <= 0)
		return;

	m_nSubMenus--;
	if (m_nSubMenus == 0)
		Deactivate();
	else
		m_pGameMenu = m_pSubMenus[m_nSubMenus - 1];
}


void CGameMenuMgr::Draw()
{
	if ( m_pGameMenu )
		m_pGameMenu->Draw();
}


void CGameMenuMgr::Clear()
{
	m_pGameMenu = NULL;
	memset(m_pSubMenus, 0, sizeof(m_pSubMenus));
	m_nSubMenus = 0;
}

#if 0
if ( key_c < 0x4Du )
{
  if ( key_c < 0x1Cu )
  {
    if ( key_c < 0xEu )
    {
      if ( key_c == 1 )
      {
        v4.code = 7;
        goto send_it;
      }
      goto LABEL_48;
    }
    if ( key_c <= 0xEu )
    {
      v4.code = 9;
      goto send_it;
    }
    if ( key_c != 15 )
    {
      v4.code = 1;
      goto send_it;
    }
    if ( !byte_3B1F22 && !byte_3B1F2E )
      goto LABEL_10;
    goto LABEL_9;
  }
  if ( key_c > 0x1Cu )
  {
    if ( key_c < 0x48u )
    {
      if ( key_c == 57 )
        v4.code = 8;
      else
        v4.code = 1;
      goto send_it;
    }
    if ( key_c > 0x48u )
    {
      if ( key_c != 75 )
      {
        v4.code = 1;
        goto send_it;
      }
      goto LABEL_13;
    }
LABEL_9:
    v4.code = 2;
    goto send_it;
  }
}
else
{
  if ( key_c <= 0x4Du )
    goto LABEL_14;
  if ( key_c >= 0xC8u )
  {
    if ( key_c > 0xC8u )
    {
      if ( key_c >= 0xCDu )
      {
        if ( key_c > 0xCDu )
        {
          if ( key_c < 0xD0u )
          {
LABEL_48:
            v4.code = 1;
send_it:
            if ( Event(this_c->m_pGameMenu, &v4) )
              Pop(this_c);
            return;
          }
          if ( key_c > 0xD0u )
          {
            if ( key_c != -45 )
              goto LABEL_48;
LABEL_15:
            v4.code = 10;
            goto send_it;
          }
LABEL_10:
          v4.code = 3;
          goto send_it;
        }
LABEL_14:
        v4.code = 5;
        goto send_it;
      }
      if ( key_c != 203 )
        goto LABEL_48;
LABEL_13:
      v4.code = 4;
      goto send_it;
    }
    goto LABEL_9;
  }
  if ( key_c < 0x53u )
  {
    if ( key_c != 80 )
      goto LABEL_48;
    goto LABEL_10;
  }
  if ( key_c <= 0x53u )
    goto LABEL_15;
  if ( key_c != -100 )
    goto LABEL_48;
}
v4.code = 6;
goto send_it;
#endif

void CGameMenuMgr::Process()
{
	if (!m_pGameMenu)
		return;

	CGameMenuEvent e(0);
	uint8 key = keyGet();
	if (key == 0)
		return;
	keyFlushStream();

	uint16 code;
	switch (key)
	{
		case BKEY_UP:     code = 2; break;
		case BKEY_DOWN:   code = 3; break;
		case BKEY_LEFT:   code = 4; break;
		case BKEY_RIGHT:  code = 5; break;
		case BKEY_ENTER:  code = 6; break;
		case BKEY_ESCAPE: code = 7; break;
		default:          code = 1; break;
	}

	if (m_pGameMenu->Event(CGameMenuEvent(code, key)))
		Pop();
	return;

if ( key < 0x4Du )
{
  if ( key < 0x1Cu )
  {
    if ( key < 0xEu )
    {
      if ( key == 1 )
      {
        e.code = 7;
        goto send_it;
      }
      goto LABEL_48;
    }
    if ( key <= 0xEu )
    {
      e.code = 9;
      goto send_it;
    }
    if ( key != 15 )
    {
      e.code = 1;
      goto send_it;
    }
//    if ( !byte_3B1F22 && !byte_3B1F2E )
//      goto LABEL_10;
    goto LABEL_9;
  }
  if ( key > 0x1Cu )
  {
    if ( key < 0x48u )
    {
      if ( key == 57 )
        e.code = 8;
      else
        e.code = 1;
      goto send_it;
    }
    if ( key > 0x48u )
    {
      if ( key != 75 )
      {
        e.code = 1;
        goto send_it;
      }
      goto LABEL_13;
    }
LABEL_9:
    e.code = 2;
    goto send_it;
  }
}
else
{
  if ( key <= 0x4Du )
    goto LABEL_14;
  if ( key >= 0xC8u )
  {
    if ( key > 0xC8u )
    {
      if ( key >= 0xCDu )
      {
        if ( key > 0xCDu )
        {
          if ( key < 0xD0u )
          {
LABEL_48:
            e.code = 1;
			goto send_it;
//send_it:
//            if ( Event(this_c->m_pGameMenu, &e) )
//              Pop(this_c);
//            return;
          }
          if ( key > 0xD0u )
          {
            if ( key != -45 )
              goto LABEL_48;
LABEL_15:
            e.code = 10;
            goto send_it;
          }
LABEL_10:
		  // Down key
          e.code = 3;
          goto send_it;
        }
LABEL_14:
        e.code = 5;
        goto send_it;
      }
      if ( key != 203 )
        goto LABEL_48;
LABEL_13:
      e.code = 4;
      goto send_it;
    }
    goto LABEL_9;
  }
  if ( key < 0x53u )
  {
    if ( key != 80 )
      goto LABEL_48;
    goto LABEL_10;
  }
  if ( key <= 0x53u )
    goto LABEL_15;
  if ( key != -100 )
    goto LABEL_48;
}
e.code = 6;
goto send_it;

send_it:
	if (m_pGameMenu->Event(e))
		Pop();
}


void CGameMenuMgr::Deactivate()
{
	Clear();
	keyFlushStream();
	m_bActive = 0;
	gInputMode = 0;
}


CGameMenuItem::CGameMenuItem()
{
	m_pzText   = NULL;
	m_field_14 = 0;
	m_x        = 0;
	m_y        = 0;
	m_nFontId  = -1;
	m_pParent  = NULL;
	m_field_18 = 3;
	m_field_18 &= 0xF7;
}


uint8 CGameMenuItem::Event(const CGameMenuEvent &event)
{
//	if (event.code) dprintf("CGameMenuItem got event %u\n", event.code);
	switch (event.code)
	{
		case 2:
			m_pParent->FocusPrevItem();
			return 0;

		case 3:
			m_pParent->FocusNextItem();
			return 0;

		case 7:
			return 1;
	}
	return 0;
}


CGameMenuItemTitle::CGameMenuItemTitle(const char *pzText, int nFontId, int x, int y, int nPic)
{
//	dprintf("CGameMenuItemTitle::CGameMenuItemTitle(\"%s\", %d, %d, %d, %d)\n", pzText, nFontId, x, y, nPic);
	m_field_14 = 0;	// Parent clears
	m_pzText   = pzText;
	m_nFontId  = nFontId;
	m_x        = x;
	m_y        = y;
	m_nPic     = nPic;
	m_field_18 &= 0xFD;
}


void CGameMenuItemTitle::Draw()
{
	if (!m_pzText)
		return;

	int nHeight;
	gMenuTextMgr.GetFontInfo(m_nFontId, NULL, NULL, &nHeight);
	rotatesprite(160<<16, m_y << 16, 65536, 0, m_nPic, -128, 0, 8|2/*4*/, 0, 0, xdim - 1, ydim - 1);
	viewDrawText(m_nFontId, m_pzText, m_x, m_y - nHeight / 2, -128, 0, 1);
}


CGameMenuItemChain::CGameMenuItemChain(const char *pzText, int nFontId, int nX, int nY, int e, int f, CGameMenu *g, int h, void (*i)(CGameMenuItemChain *), int j)
{
	m_pzText    = pzText;
	m_nFontId   = nFontId;
	m_x         = nX;
	m_y         = nY;
	m_field_14  = e;
	m_field_20  = f;
	m_field_24  = g;
	m_field_28  = h;
	m_pCallback = i;
	m_field_30  = j;
}


void CGameMenuItemChain::Draw()
{
	if (!m_pzText)
		return;

	int nFoo = 32;
	if (GetParent()->IsFocusItem(this))
		nFoo = 32 - (totalclock & 0x3F);

	int x = m_x;
	int nWidth;
	gMenuTextMgr.GetFontInfo(m_nFontId, (char *)m_pzText, &nWidth, NULL);

	switch (m_field_20)
	{
		case 1:
			x = m_field_14 / 2 + m_x - nWidth / 2;
			break;

		case 2:
			x = m_x + m_field_14 - 1 - nWidth;
			break;
	}

	gMenuTextMgr.DrawText(m_pzText, m_nFontId, x, m_y, nFoo, 0, 1);
}


uint8 CGameMenuItemChain::Event(const CGameMenuEvent &e)
{
//	if (e.code) dprintf("CGameMenuItemChain got event %u\n", e.code);

	if (e.code != 6)
		return CGameMenuItem::Event(e);

	if (m_pCallback)
		m_pCallback(this);
	if (m_field_24)
		gGameMenuMgr.Push(m_field_24, m_field_28);

	return 0;
}


CGameMenuItemText::CGameMenuItemText(const char *pzText, int nFontId, int nX, int nY, int e)
{
	m_field_14 = 0;
	m_pzText   = pzText;
	m_nFontId  = nFontId;
	m_x        = nX;
	m_y        = nY;
	m_field_20 = e;
	m_field_18 &= 0xFD;
}


void CGameMenuItemText::Draw()
{
	if (!m_pzText)
		return;

	int nWidth;
	gMenuTextMgr.GetFontInfo(m_nFontId, (char *)m_pzText, &nWidth, NULL);

	int x = m_x;

	switch (m_field_20)
	{
		case 1:
			x = m_x - nWidth / 2;
			break;

		case 2:
			x = m_x - nWidth;
			break;
	}

	gMenuTextMgr.DrawText(m_pzText, m_nFontId, x, m_y, -128, 0, 0);
}


void CMenuTextMgr::DrawText(const char *string, int nFontId, int c, int d, int nShade, int nPLU, uint8 g)
{
	dassert(string != NULL);
	LockFont(nFontId);
	uint8 *v14 = (uint8 *)((getpalookup(0, nShade) << 8) + palookup[nPLU]);
	uint8 *v15 = (uint8 *)((getpalookup(0, 127) << 8) + palookup[nPLU]);
	begindrawing();	//{{{
	int x = c;
	for (const char *s = string; *s; s++)
	{
		if (g)
			viewDrawChar(m_pFont, *s, x + 1, d + 1, v15);
		viewDrawChar(m_pFont, *s, x, d, v14);
		x += m_pFont->info[*s].hspace + m_pFont->charSpace;
	}
	enddrawing();	//}}}
	UnlockFont();
}


void CMenuTextMgr::LockFont(int nFontId)
{
	if (!m_hFont.isNull())
		UnlockFont();
	m_hFont = gSysRes.Lookup(nFontId, "QFN");
	dassert(!m_hFont.isNull());
	m_pFont = (QFONT *)gSysRes.Lock(m_hFont);
	GetFontInfo(NULL, &m_field_C, &m_field_10);
}


void CMenuTextMgr::UnlockFont()
{
	if (!m_hFont.isNull())
	{
		gSysRes.Unlock(m_hFont);
		m_field_0 = -1;
//		m_hFont   = NULL;
		m_pFont   = NULL;
	}
}


void CMenuTextMgr::GetFontInfo(int nFontId, char *string, int *c, int *d)
{
	LockFont(nFontId);
	GetFontInfo(string, c, d);
	UnlockFont();
}


void CMenuTextMgr::GetFontInfo(char *string, int *a, int *b)
{
	dassert(!m_hFont.isNull());
	if (string)
	{
		int nWidth = -m_pFont->charSpace;
		for (char *s = string; *s; s++)
			nWidth += m_pFont->info[*s].hspace + m_pFont->charSpace;
		if (a) *a = nWidth;
	}
	else
	{
		if (a) *a = m_pFont->sizeX;
	}
	if (b) *b = m_pFont->sizeY;
}


CGameMenuItemQAV::CGameMenuItemQAV(const char *a, int b, int c, int d, const char *e)
{
	m_field_18 &= 0xFD;
}


void CGameMenuItemQAV::Draw()
{
//	assert("CGameMenuItemQAV not implemented" && 0);
}


uint8 CGameMenuItemQAV::Event(const CGameMenuEvent &e)
{
	if (e.code < 6)
	{
		if (e.code < 4) return CGameMenuItem::Event(e);
		if (e.code > 4)
		{
			m_pParent->FocusNextItem();
			return 0;
		}
		// code = 4
		m_pParent->FocusPrevItem();
		return 0;
	}
	// code = 6
	if (e.code <= 6)
	{
		m_pParent->FocusNextItem();
		return 0;
	}
	if (e.code < 9)
	{
		if (e.code != 8)
			return CGameMenuItem::Event(e);
		m_pParent->FocusNextItem();
		return 0;
	}
	// code = 9
	if (e.code <= 9)
	{
		m_pParent->FocusPrevItem();
		return 0;
	}
	if (e.code != 0x8000)
		return CGameMenuItem::Event(e);

	// Initialize QAV here
	return 0;
}


CGameMenuItemKeyList::CGameMenuItemKeyList(const char *a, int b, int c, int d, int e, int f, int g, void (*h)())
{
}


CGameMenuItemSlider::CGameMenuItemSlider(const char *pzText, int nFontId, int x, int y, int e, int nCurrentValue, int nMin, int nMax, int i, void (*pCallback)(CGameMenuItemSlider *), int nBackPic, int nSliderPic)
{
	m_pzText   = pzText;
	m_nFontId  = nFontId;
	m_x        = x;
	m_y        = y;
	m_field_14 = e;
	m_nMin     = nMin;
	m_nMax     = nMax;
	m_nUnknown = i;
	int nValue = m_nMax;
	if (nCurrentValue >= m_nMin)
	{
		if (nCurrentValue <= nValue)
			nValue = nCurrentValue;
	}
	else
	{
		nValue = m_nMin;
	}
	m_nValue     = nValue;
	m_nBkgPic    = 2204;
	m_pCallback  = pCallback;
	m_nSliderPic = 2206;
	if (nBackPic >= 0)
		m_nBkgPic = nBackPic;
	if (nSliderPic >= 0)
		m_nSliderPic = nSliderPic;
//	assert(m_nBkgPic == 2204);
//	assert(m_nSliderPic == 2206);
}


CGameMenuItemSlider::CGameMenuItemSlider(const char *pzText, int nFontId, int x, int y, int e, int *f, int g, int h, int i, void (*pCallback)(CGameMenuItemSlider *), int nBackPic, int nSliderPic)
{
	m_nBkgPic    = 2204;
	m_nSliderPic = 2206;
	if (nBackPic >= 0)
		m_nBkgPic = nBackPic;
	if (nSliderPic >= 0)
		m_nSliderPic = nSliderPic;
}


void CGameMenuItemSlider::Draw()
{
	int nHeight = tilesizy[m_nBkgPic];
//	gMenuTextMgr.GetFontInfo(m_nFontId, NULL, NULL, &nHeight);
	int v5 = 32;
	if (m_pParent->IsFocusItem(this))
		v5 = 32 - (totalclock & 0x3F);
	if (m_pzText)
		gMenuTextMgr.DrawText(m_pzText, m_nFontId, m_x, m_y, v5, 0, 0);

	int nBaseX = m_x + m_field_14 - 1 - tilesizx[m_nBkgPic] / 2;

//	__asm int 3
	rotatesprite(
		nBaseX << 16,
		(nHeight / 2 + m_y) << 16,
		65536,
		0, m_nBkgPic, 0, 0, 8|2, 0, 0, xdim - 1, ydim - 1);

	int nRange = m_nMax - m_nMin;
	dassert(nRange > 0);

	rotatesprite(
		nBaseX << 16,
		(nHeight / 2 + m_y) << 16,
		65536,
		0, m_nSliderPic, 0, 0, 8|2, 0, 0, xdim - 1, ydim - 1);
}


CGameMenuItemZBool::CGameMenuItemZBool(const char *pzText, int nFontId, int x, int y, int e, uint8 f, void (*pCallback)(CGameMenuItemZBool *), char *pzOn, char *pzOff)
{
	m_pzText    = pzText;
	m_nFontId   = nFontId;
	m_x         = x;
	m_y         = y;
	m_field_14  = e;
	m_field_20  = f;
	m_pCallback = pCallback;
	m_pzOn      = (char *)kpzOnDefault;
	if (pzOn)
		m_pzOn = pzOn;
	m_pzOff     = (char *)kpzOffDefault;
	if (pzOff)
		m_pzOff = pzOff;
}


void CGameMenuItemZBool::Draw()
{
	int nShade = 32;
	if (m_pParent->IsFocusItem(this))
		nShade = 32 - (totalclock & 0x3F);

	if (m_pzText)
		gMenuTextMgr.DrawText(m_pzText, m_nFontId, m_x, m_y, nShade, 0, 0);

	char *pzText = m_pzOff;
	if (m_field_20)
		pzText = m_pzOn;

	int nWidth;
	gMenuTextMgr.GetFontInfo(m_nFontId, pzText, &nWidth, NULL);
	gMenuTextMgr.DrawText(pzText, m_nFontId, m_x + m_field_14 - 1 - nWidth, m_y, nShade, 0, 0);
}


uint8 CGameMenuItemZBool::Event(const CGameMenuEvent &e)
{
	if (e.code < 6 || (e.code > 6 && e.code != 8))
		return CGameMenuItem::Event(e);

	// Toggle state and call callback
	m_field_20 = !m_field_20;
	if (m_pCallback)
		m_pCallback(this);
	return 0;
}


CGameMenuItemYesNoQuit::CGameMenuItemYesNoQuit(const char *pzText, int nFontId, int x, int y, int e, int f, int g, int h)
{
	m_pzText   = pzText;
	m_nFontId  = nFontId;
	m_x        = x;
	m_y        = y;
	m_field_14 = e;
	m_field_20 = f;
	m_field_24 = g;
	m_field_28 = h;
}


void CGameMenuItemYesNoQuit::Draw()
{
	if (!m_pzText)
		return;

	int nShade = 32;
	if (m_pParent->IsFocusItem(this))
		nShade = 32 - (totalclock & 0x3F);

	int nWidth;
	gMenuTextMgr.GetFontInfo(m_nFontId, (char *)m_pzText, &nWidth, NULL);

	int x = m_x;
	switch (m_field_20)
	{
		case 1:
			x = m_field_14 / 2 + m_x - nWidth / 2;
			break;

		case 2:
			x = m_x + m_field_14 - 1 - nWidth;
			break;
	}

	gMenuTextMgr.DrawText(m_pzText, m_nFontId, x, m_y, nShade, 0, 1);
}


uint8 CGameMenuItemYesNoQuit::Event(const CGameMenuEvent &e)
{
	switch (e.code)
	{
		case 1:
			switch (e.key)
			{
				case BKEY_Y: Quit(NULL);         return 0;
				case BKEY_N: gGameMenuMgr.Pop(); return 0;
				default: return 0;
			}
			break;

		case 6:
			Quit(NULL);
			return 0;

		default:
			return CGameMenuItem::Event(e);
	}

	return 0;
}


CGameMenuItemZEdit::CGameMenuItemZEdit(char *a, int b, int c, int d, int e, const char *f, int g, unsigned char h, pfnZEditCallback i, int j)
{
}


void CGameMenuItemZEdit::Draw()
{
}
