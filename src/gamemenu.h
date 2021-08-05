#ifndef __GAMEMENU_H__
#define __GAMEMENU_H__


#define kMaxGameMenuItems 32


class CGameMenuItem;
class ResHandle;
struct QFONT;


class CGameMenuEvent
{
public:
	uint16 code;
	uint8  key;
	CGameMenuEvent(uint16 code, uint8 key = 0)	{ this->code = code; this->key = key; }
};


class CGameMenu
{
	int m_nItems;
	int m_nFocus;
	int field_8;
	CGameMenuItem *m_pItems[kMaxGameMenuItems];

public:
	CGameMenu();
	~CGameMenu();

	uint8 Event( const CGameMenuEvent &event );
	void  SetFocusItem( int nItem );
	bool  IsFocusItem( CGameMenuItem *pItem ) const;
	bool  CanSelectItem( int nItem ) const;
	void  Draw();
	void  Add(CGameMenuItem *pItem, bool foo);
	void  FocusPrevItem();
	void  FocusNextItem();
	void  InitializeItems(const CGameMenuEvent &event);
};


class CGameMenuMgr
{
	bool m_bInitialized;
	CGameMenu *m_pGameMenu;
	CGameMenu *m_pSubMenus[8];
	int        m_nSubMenus;

public:
	CGameMenuMgr();
	~CGameMenuMgr();

	void InitializeMenu();
	void Push( CGameMenu *pMenu, int unk );
	void Pop();
	void Draw();
	void Clear();
	void Process();
	void Deactivate();

	static bool m_bActive;
};


class CGameMenuItem
{
public:
	CGameMenu * m_pParent;
	int         m_field_14;
	const char *m_pzText;
	int         m_nFontId;
	int         m_x;
	int         m_y;
	uint8       m_field_18;

public:
	CGameMenuItem();
	void SetParent(CGameMenu *pParent)	{ m_pParent = pParent; }
	CGameMenu *GetParent()				{ return m_pParent; }

	// Virtual functions
	virtual void  Draw() = 0;
	virtual uint8 Event(const CGameMenuEvent &event);
};


class CGameMenuItemTitle : public CGameMenuItem
{
	int m_nPic;

	CGameMenuItemTitle()	{ assert(0); }	// Shouldn't be able to call this

public:
	CGameMenuItemTitle(const char *foo, int a, int b, int c, int d);

	virtual void Draw();
};


class CGameMenuItemText : public CGameMenuItem
{
	int m_field_20;

	CGameMenuItemText()	{ assert(0); }

public:
	CGameMenuItemText(const char *pzText, int nFontId, int nX, int nY, int e);
	virtual void Draw();
};

class CGameMenuItemChain;
typedef void (*pfnChainCallback)(CGameMenuItemChain *pChain);

class CGameMenuItemChain : public CGameMenuItem
{
	CGameMenuItemChain()	{ assert(0); }

public:
	int              m_field_20;
	CGameMenu       *m_field_24;
	int              m_field_28;
	pfnChainCallback m_pCallback;
	int              m_field_30;

public:
	CGameMenuItemChain(const char *, int, int, int, int, int, CGameMenu *, int, void (*)(CGameMenuItemChain *), int);

	virtual void  Draw();
	virtual uint8 Event(const CGameMenuEvent &e);
};


class CGameMenuItemQAV : public CGameMenuItem
{
	CGameMenuItemQAV()	{ assert(0); }

public:
	CGameMenuItemQAV(const char *a, int b, int c, int d, const char *e);

	virtual void  Draw();
	virtual uint8 Event(const CGameMenuEvent &e);
};


class CGameMenuItemKeyList : public CGameMenuItem
{
	CGameMenuItemKeyList()	{ assert(0); }

public:
	CGameMenuItemKeyList(const char *a, int b, int c, int d, int e, int f, int g, void (*h)());

	virtual void Draw()	{}
};

class CGameMenuItemSlider;
typedef void (*pfnSliderCallback)(CGameMenuItemSlider *);

class CGameMenuItemSlider : public CGameMenuItem
{
	CGameMenuItemSlider()	{ assert(0); }

public:
	int               m_nJunk;
	int               m_nValue;
	int               m_nMin;
	int               m_nMax;
	int               m_nUnknown;
	pfnSliderCallback m_pCallback;
	int               m_nBkgPic;
	int               m_nSliderPic;

public:
	CGameMenuItemSlider(const char *pzText, int nFontId, int x, int y, int e, int nCurrentValue, int nMin, int nMax, int i, void (*pCallback)(CGameMenuItemSlider *), int nBackPic, int nSliderPic);
	CGameMenuItemSlider(const char *pzText, int nFontId, int x, int y, int e, int *f, int g, int h, int i, void (*pCallback)(CGameMenuItemSlider *), int nBackPic, int nSliderPic);

	virtual void Draw();
};


class CGameMenuItemZBool;
typedef void (*pfnZBoolCallback)(CGameMenuItemZBool *);

class CGameMenuItemZBool : public CGameMenuItem
{
	CGameMenuItemZBool()	{ assert(0); }

public:
	uint8            m_field_20;
	char *           m_pzOn;
	char *           m_pzOff;
	pfnZBoolCallback m_pCallback;

public:
	CGameMenuItemZBool(const char *pzText, int nFontId, int x, int y, int e, uint8 f, void (*pCallback)(CGameMenuItemZBool *), char *pzOn, char *pzOff);

	virtual void  Draw();
	virtual uint8 Event(const CGameMenuEvent &e);
};


class CGameMenuItemYesNoQuit : public CGameMenuItem
{
	CGameMenuItemYesNoQuit()	{ assert(0); }

public:
	int m_field_20;
	int m_field_24;
	int m_field_28;

public:
	CGameMenuItemYesNoQuit(const char *pzText, int nFontId, int x, int y, int e, int f, int g, int h);

	virtual void  Draw();
	virtual uint8 Event(const CGameMenuEvent &e);
};


class CGameMenuItemZEdit;
typedef void (*pfnZEditCallback)(CGameMenuItemZEdit *, CGameMenuEvent *);
class CGameMenuItemZEdit : public CGameMenuItem
{
	CGameMenuItemZEdit()	{ assert(0); }

public:
	int   field_20;
	int   field_24;
	int   field_28;
	int   field_2C;
	uint8 field_30;
	uint8 field_31;
	uint8 field_32;
	uint8 field_33;

public:
	CGameMenuItemZEdit(char *a, int b, int c, int d, int e, const char *f, int g, unsigned char h, pfnZEditCallback i, int j);

	virtual void Draw();
};


class CGameMenuItemBitmapLS : public CGameMenuItem
{
public:
	CGameMenuItemBitmapLS()	{}	// TODO: FIXME Should be private

	virtual void Draw()	{}
};


class CMenuTextMgr
{
	int       m_field_0;
	ResHandle m_hFont;
	QFONT    *m_pFont;
	int       m_field_C;
	int       m_field_10;

public:
	CMenuTextMgr()	{}

	void DrawText(const char *, int, int, int, int, int, uint8);
	void LockFont(int a);
	void UnlockFont();
	void GetFontInfo(int, char *, int *, int *);
	void GetFontInfo(char *, int *, int *);
};


extern CGameMenuMgr gGameMenuMgr;
extern CMenuTextMgr gMenuTextMgr;

// Game menus
extern CGameMenu menuMain;
extern CGameMenu menuLevelStat;
extern CGameMenu menuFragStat;
extern CGameMenu menuMainWithSave;
extern CGameMenu menuNetMain;
extern CGameMenu menuNetStart;
extern CGameMenu menuEpisode;
extern CGameMenu menuDifficulty;
extern CGameMenu menuOptions;
extern CGameMenu menuControls;
extern CGameMenu menuMessages;
extern CGameMenu menuKeys;
extern CGameMenu menuSaveGame;
extern CGameMenu menuLoadGame;
extern CGameMenu menuLoading;
extern CGameMenu menuSounds;
extern CGameMenu menuQuit;
extern CGameMenu menuCredits;
extern CGameMenu menuOrder;
extern CGameMenu menuParentalLock;
extern CGameMenu menuSorry;

// Titles
extern CGameMenuItemTitle itemMainTitle;
extern CGameMenuItemTitle itemEpisodeTitle;
extern CGameMenuItemTitle itemDifficultyTitle;
extern CGameMenuItemTitle itemKeysTitle;
extern CGameMenuItemTitle itemOptionsTitle;
extern CGameMenuItemTitle itemQuitTitle;
extern CGameMenuItemTitle itemMessagesTitle;
extern CGameMenuItemTitle itemControlsTitle;
extern CGameMenuItemTitle itemSaveTitle;
extern CGameMenuItemTitle itemLoadTitle;
extern CGameMenuItemTitle itemMainSaveTitle;

// Chain objects
extern CGameMenuItemChain itemMain1;
extern CGameMenuItemChain itemMain2;
extern CGameMenuItemChain itemMain3;
extern CGameMenuItemChain itemMain4;
extern CGameMenuItemChain itemMain5;
extern CGameMenuItemChain itemMain6;
extern CGameMenuItemChain itemMainSave1;
extern CGameMenuItemChain itemMainSave2;
extern CGameMenuItemChain itemMainSave3;
extern CGameMenuItemChain itemMainSave4;
extern CGameMenuItemChain itemMainSave5;
extern CGameMenuItemChain itemMainSave6;
extern CGameMenuItemChain itemMainSave7;
extern CGameMenuItemChain itemDifficulty1;
extern CGameMenuItemChain itemDifficulty2;
extern CGameMenuItemChain itemDifficulty3;
extern CGameMenuItemChain itemDifficulty4;
extern CGameMenuItemChain itemDifficulty5;
extern CGameMenuItemChain itemOption1;
extern CGameMenuItemChain itemChainParentalLock;
extern CGameMenuItemChain itemChainKeyList;
extern CGameMenuItemChain itemNetStart9;
extern CGameMenuItemChain itemEpisodes[4];

// QAVs
extern CGameMenuItemQAV	itemBloodQAV;
extern CGameMenuItemQAV	itemCreditsQAV;
extern CGameMenuItemQAV itemHelp1QAV;
extern CGameMenuItemQAV itemHelp2QAV;
extern CGameMenuItemQAV itemHelp3QAV;
extern CGameMenuItemQAV itemHelp3BQAV;

// Lists
extern CGameMenuItemKeyList itemKeyList;

// Texts
extern CGameMenuItemText  itemLoadingText;
extern CGameMenuItemText  itemQuitText1;
extern CGameMenuItemText  itemSorryText1;
extern CGameMenuItemText  itemSorryText2;
extern CGameMenuItemText  itemSorryText3;

// Sliders
extern CGameMenuItemSlider sliderDetail;
extern CGameMenuItemSlider sliderGamma;
extern CGameMenuItemSlider sliderMusic;
extern CGameMenuItemSlider sliderSound;
extern CGameMenuItemSlider sliderCDAudio;
extern CGameMenuItemSlider sliderMouseSpeed;
extern CGameMenuItemSlider sliderTurnSpeed;
extern CGameMenuItemSlider sliderMsgCount;
extern CGameMenuItemSlider sliderMsgTime;
extern CGameMenuItemSlider sliderSoundsMusic;
extern CGameMenuItemSlider sliderSoundsSound;
extern CGameMenuItemSlider sliderSoundsCDAudio;

// Bools
extern CGameMenuItemZBool boolCrosshair;
extern CGameMenuItemZBool boolShowWeapons;
extern CGameMenuItemZBool boolSlopeTilting;
extern CGameMenuItemZBool boolViewBobbing;
extern CGameMenuItemZBool boolViewSwaying;
extern CGameMenuItemZBool boolMessages;
extern CGameMenuItemZBool boolMsgFont;
extern CGameMenuItemZBool boolMsgIncoming;
extern CGameMenuItemZBool boolMsgSelf;
extern CGameMenuItemZBool boolMsgOther;
extern CGameMenuItemZBool boolMsgRespawn;
extern CGameMenuItemZBool itemParentalLockToggle;

// ZEdit
extern CGameMenuItemZEdit itemSaveGame1;
extern CGameMenuItemZEdit itemSaveGame2;
extern CGameMenuItemZEdit itemSaveGame3;
extern CGameMenuItemZEdit itemSaveGame4;
extern CGameMenuItemZEdit itemSaveGame5;
extern CGameMenuItemZEdit itemSaveGame6;
extern CGameMenuItemZEdit itemSaveGame7;
extern CGameMenuItemZEdit itemSaveGame8;
extern CGameMenuItemZEdit itemSaveGame9;
extern CGameMenuItemZEdit itemSaveGame10;
extern CGameMenuItemZEdit itemLoadGame1;
extern CGameMenuItemZEdit itemLoadGame2;
extern CGameMenuItemZEdit itemLoadGame3;
extern CGameMenuItemZEdit itemLoadGame4;
extern CGameMenuItemZEdit itemLoadGame5;
extern CGameMenuItemZEdit itemLoadGame6;
extern CGameMenuItemZEdit itemLoadGame7;
extern CGameMenuItemZEdit itemLoadGame8;
extern CGameMenuItemZEdit itemLoadGame9;
extern CGameMenuItemZEdit itemLoadGame10;
//extern CGameMenuItemZEdit itemNetStart8;

// Pics
extern CGameMenuItemBitmapLS itemSaveGamePic;
extern CGameMenuItemBitmapLS itemLoadGamePic;

// Quit
extern CGameMenuItemYesNoQuit itemQuitYesNo;

#endif // __GAMEMENU_H__
