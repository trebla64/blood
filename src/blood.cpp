#include "stdafx.h"
#include "globals.h"
#include "resource.h"
#include "view.h"
#include "game.h"
#include "screen.h"
#include "gamemenu.h"
#include "player_msg.h"
#include "textio.h"
#include "trig.h"
#include "weapon.h"
#include "tile.h"
#include "levels.h"
#include "menu.h"
#include "key.h"
#include "sound.h"
#include "engine_access.h"
#include "timer.h"
#include "cfg.h"
#include "weather.h"
#include "font.h"
#include "gfx.h"
#include "gameutil.h"
#include "fire.h"
#include "error.h"
#include "debug.h"

#include "jfbuild/compat.h"
#include "jfbuild/build.h"
#include "jfbuild/baselayer.h"

enum INPUT_MODE
{
	IM_UNKNOWN = 0
};

uint8 syncstate = 0;
static int ScreenMode   = 0;	// Unused
static int ScreenWidth  = /*640*/1280/*1920*/;
static int ScreenHeight = /*400*/720/*1080*/;


static void ParseOptions()
{
}


// 120 ticks = 1 second
static void Wait(int nTicks)
{
	for (int endclock = totalclock + nTicks; endclock > totalclock;)
		if (handleevents())
			if (quitevent)
				break;
}


static void credLogos()
{
	setview(0, 0, xdim - 1, ydim - 1);
	clearview(0);
//	tileBlit(2050, 0, 0, 0);
	rotatesprite(160<<16, 100<<16, 65536, 0, 2050, 0, 0, 74, 0, 0, xdim - 1, ydim - 1);
	scrNextPage();
	Wait(240);
//	int endclock = totalclock + 240;
//	while (endclock > totalclock)
//	{
//		handleevents();
//		clearview(0);
//		rotatesprite(160<<16, 100<<16, 65536, 0, 2050, 0, 0, 74, 0, 0, xdim - 1, ydim - 1);
//		scrNextPage();
//	}

	clearview(0);
	for (int i = 64; i >= 0; i--)
	{
//		tileBlit(2046, 0, 0, i);
		rotatesprite(160<<16, 100<<16, 65536, 0, 2046, i, 0, 74, 0, 0, xdim - 1, ydim - 1);
		scrNextPage();
		Wait(15);
	}
	Wait(240);

//	endclock = totalclock + 240;
//	while (endclock > totalclock)
//	{
//		handleevents();
//		clearview(0);
//		rotatesprite(160<<16, 100<<16, 65536, 0, 2046, 0, 0, 74, 0, 0, xdim - 1, ydim - 1);
//		scrNextPage();
//	}
}


static void LocalKeys()
{
	while (1)
	{
		uint8 key = keyGet();
		if (!key)
			break;

		switch (key)
		{
			case BKEY_ESCAPE:
				keyFlushStream();
				if (gGameStarted)
				{
					// TODO: Close demo
					if (!CGameMenuMgr::m_bActive)
						gGameMenuMgr.Push(&menuMainWithSave, -1);
				}
				else
				{
					if (!CGameMenuMgr::m_bActive)
						gGameMenuMgr.Push(&menuMain, -1);
				}
				return;
		}
	}
}


#if 0
static int ticks[64] = {0};
static int tick_index = 0;
static void CalcFrameRate()
{
	int cur_ticks = ticks[tick_index];
	if (gFrameClock != cur_ticks)
	{
		(bleh) = 7680 / (gFrameClock - cur_ticks);
		ticks[tick_index] = gFrameClock;
	}
	tick_index = (tick_index + 1) & 63;
}
#endif


// Gets fired 120 times per second
static void mytimer_callback()
{
	static int last_update = 0;

	gGameClock++;

	last_update++;
	if (last_update >= 4)
	{
		last_update -= 4;
		gFrameClock++;
	}
}


static void MainLoop()
{
//	uint64 freq, last_ticks, loop_start, cur_ticks, last_movement;
	uint32 frames = 0, fps = 0;

//	QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
//	QueryPerformanceCounter((LARGE_INTEGER *)&loop_start);
//	last_ticks = 0;
//	last_movement = 0;
//	int last_update = 0;

	uint32 loop_start = getticks()<<16;
	uint32 last_ticks = 0;
	uint32 accum_ticks = 0;
	uint32 frame_accum = 0;
	uint32 frames_todo = 0;

	uint32 freq_120 = uint32((uint64(1000) << 16) / 120);
	uint32 freq_30  = uint32((uint64(1000) << 16) / 30);

	timerInit();

	while ( !gQuitGame && !syncstate )
	{
		// Give the OS a chance
		if (handleevents())
			if (quitevent)
				gQuitGame = true;

		timerUpdate();

		uint32 cur_ticks = getticks()<<16;
		cur_ticks -= loop_start;
		accum_ticks += cur_ticks - last_ticks;
		last_ticks = cur_ticks;

		while (accum_ticks >= freq_120)
		{
			accum_ticks -= freq_120;
			gGameClock++;
			frame_accum++;
			frames_todo++;
			if (frame_accum >= 4)
			{
				frame_accum -= 4;
				gFrameClock += 4;
			}
		}

#ifdef _DEBUG
		// Clear the screen with the ugly colour for debugging parts of the screen
		// that doesn't get updated
//		setview(0, 0, xdim - 1, ydim - 1);
//		clearview(0);
#endif

//		if ( gRedBookInstalled )
//			preprocess(&Redbook, v6);

		if (gInputMode < 0 || gInputMode > 2)
			ThrowError("Invalid gInputMode %d in game loop.", gInputMode);
		switch (gInputMode)
		{
			case 0:
				LocalKeys();
				break;

			case 1:
				if (CGameMenuMgr::m_bActive)
					gGameMenuMgr.Process();
				break;
		}
//		if ( gRedBookInstalled )
//			postprocess(&Redbook);
		if ( gGameStarted )
		{
//			if ( numplayers > 1 )
//				netGetPackets();
//			while ( gPredictTail < gNetFifoHead[myconnectindex] )

//			viewUpdatePrediction(&gFifoInput[8 * (gPredictTail & 0xFF)]);

			// Mouse hacks
			int cur_mousex, cur_mousey;
			readmousexy(&cur_mousex, &cur_mousey);

			gInput.turn = 0;
			gInput.turn += /*32 * */cur_mousex;

//			QueryPerformanceCounter((LARGE_INTEGER *)&cur_ticks);
//			cur_ticks -= loop_start;
//			float fDelta = float(double(cur_ticks - last_movement) / double(freq) * 30.0);
			viewUpdatePredictionTest(&gInput, frames_todo);
			frames_todo = 0;
//			last_movement = cur_ticks;

//			if ( numplayers == 1 )
//				gBufferJitter = 0;
//			while ( gNetFifoHead[myconnectindex] - gNetFifoTail > gBufferJitter )
//			{
//				for ( i = connecthead; i >= 0 && gNetFifoTail != gNetFifoHead[i]; i = connectpoint2[i] )
//					;
//				if ( i >= 0 )
//					break;
				FakeTimerHandler();
				ProcessFrame();
//			}
//			netCheckSync();
			viewDrawScreen();
		}
		else
		{
			clearview(0);
//			netGetPackets();
//			if ( gQuitRequest )
//				netBroadcastMyLogoff();
		}

		if ( gInputMode == 1 && CGameMenuMgr::m_bActive )
			gGameMenuMgr.Draw();

		if ( gInputMode == 2 )
		{
			gPlayerMsg.ProcessKeys();
			gPlayerMsg.Draw();
		}

		char buf[128];
		Bsnprintf(buf, sizeof(buf), "FPS: %u", gFrameRate);
//		viewDrawText(1, buf, 10, 10, -128, 0, 0);
//		gDbgFont.DrawChar(10, 10, 'A', gStdColor[15]);
		gDbgFont.DrawString(10, 10, buf, gStdColor[15]);
		Bsnprintf(buf, sizeof(buf), "x = %d, y = %d, z = %d, ang = %d, sect = %d",
			gPlayerTemp.x, gPlayerTemp.y, gPlayerTemp.z, gPlayerTemp.ang, gPlayerTemp.sector);
		gDbgFont.DrawString(10, 20, buf, gStdColor[15]);
		Bsnprintf(buf, sizeof(buf), "clock = %u, gNetFifoClock = %u, seconds = %u", gGameClock, gNetFifoClock, gGameClock / 120);
		gDbgFont.DrawString(10, 30, buf, gStdColor[15]);
		Bsnprintf(buf, sizeof(buf), "rposx = %d, rposy = %d, rposz = %d, rang = %d, rsector = %d",
			rposx, rposy, rposz, rang, rsector);
		gDbgFont.DrawString(10, 40, buf, gStdColor[15]);

		if (gGameStarted)
		{
#if 0
			gfxSetClipToScreen();
			for (int i = 0; i < numsectors; i++)
			{
				int wallptr = sector[i].wallptr;
				for (int j = 0; j < sector[i].wallnum; j++)
				{
					int point2 = wall[wallptr].point2;
					int64 x1 = (wall[wallptr].x - gPlayerTemp.x)/* >> 5*/;// + (xdim / 2);
					int64 y1 = (wall[wallptr].y - gPlayerTemp.y)/* >> 5*/;// + (ydim / 2);
					int64 x2 = (wall[point2].x - gPlayerTemp.x)/* >> 5*/;// + (xdim / 2);
					int64 y2 = (wall[point2].y - gPlayerTemp.y)/* >> 5*/;// + (ydim / 2);
					int64 cosval = costable[(-gPlayerTemp.ang - 512) & 2047];
					int64 sinval = costable[(-gPlayerTemp.ang - 1024) & 2047];
//					int32 nx1 = ((x1*cosval)>>30) - ((y1*sinval)>>30);
//					int32 ny1 = ((x1*sinval)>>30) + ((y1*cosval)>>30);
//					int32 nx2 = ((x2*cosval)>>30) - ((y2*sinval)>>30);
//					int32 ny2 = ((x2*sinval)>>30) + ((y2*cosval)>>30);

					int32 nx1 = (x1*cosval - y1*sinval) >> 35;
					int32 ny1 = (x1*sinval + y1*cosval) >> 35;
					int32 nx2 = (x2*cosval - y2*sinval) >> 35;
					int32 ny2 = (x2*sinval + y2*cosval) >> 35;
					nx1 += (xdim / 2);
					ny1 += (ydim / 2);
					nx2 += (xdim / 2);
					ny2 += (ydim / 2);
					if (ClipLine(&nx1, &ny1, &nx2, &ny2))
					{
						gfxDrawLine(nx1, ny1, nx2, ny2, gStdColor[15]);
					}
					wallptr++;
				}
			}
			gfxDrawCircle(xdim / 2, ydim / 2, 128>>5, gStdColor[15]);	// Player pos

			// Draw all dudes
			SPRITE *pSprite = sprite;
			for (int i = 0; i < MAXSPRITES; i++)
			{
				if (pSprite->type >= kDudeBase && pSprite->type < kDudeMax)
				{
					int64 x = pSprite->x - gPlayerTemp.x;
					int64 y = pSprite->y - gPlayerTemp.y;
					int64 cosval = costable[(-gPlayerTemp.ang - 512) & 2047];
					int64 sinval = costable[(-gPlayerTemp.ang - 1024) & 2047];
					int32 nx = (x*cosval - y*sinval) >> 35;
					int32 ny = (x*sinval + y*cosval) >> 35;
					nx += (xdim / 2);
					ny += (ydim / 2);
					int32 ex = 256>>5;
					int32 ey = 0;
					int nAngDiff = (pSprite->ang - gPlayerTemp.ang) & 2047;

					int dx = gPlayerTemp.x - pSprite->x;
					int dy = gPlayerTemp.y - pSprite->y;
					int nAngToPlayer = getangle(dx, dy);

					RotateVector(&ex, &ey, nAngDiff);
					ex += nx;
					ey += ny;
					int radius = 128>>5;
					if (nx >= radius && nx < (xdim - radius) && ny >= radius && ny < (ydim - radius))
						gfxDrawCircle(nx, ny, radius, gStdColor[kColRed]);
					// Render a line from the center of the sprite in the same direction as the angle of the sprite
					if (ClipLine(&nx, &ny, &ex, &ey))
						gfxDrawLine(nx, ny, ex, ey, gStdColor[kColRed]);
					if (nx >= 0 && nx < (xdim + radius - 50) && ny >= 0 && ny < (ydim + radius - 10))
					{
						nAngToPlayer = (-nAngToPlayer + 128) & 2047;
						int nOctant = nAngToPlayer / 256;
						Bsnprintf(buf, sizeof(buf), "%d, %d", nAngToPlayer, 8 - nOctant);
						gDbgFont.DrawString(nx+radius, ny+radius, buf, gStdColor[kColWhite]);
					}
				}
				pSprite++;
			}

			// Draw the actual player position as a blue circle
			int64 x = rposx - gPlayerTemp.x;
			int64 y = rposy - gPlayerTemp.y;
			int64 cosval = costable[(-gPlayerTemp.ang - 512) & 2047];
			int64 sinval = costable[(-gPlayerTemp.ang - 1024) & 2047];
			int32 nx = (x*cosval - y*sinval) >> 35;
			int32 ny = (x*sinval + y*cosval) >> 35;
			nx += (xdim / 2);
			ny += (ydim / 2);
			Bsnprintf(buf, sizeof(buf), "x = %d, y = %d", nx, ny);
			gDbgFont.DrawString(10, 70, buf, gStdColor[kColWhite]);
			int radius = 128/5;
			if (nx >= radius && nx < (xdim - radius) && ny >= radius && ny < (ydim - radius))
				gfxDrawCircle(nx, ny, radius, gStdColor[kColBlue]);
#endif
		}

		// Test clipping
//		begindrawing();	//{{{
//		uint8 *p = (uint8 *)frameplace + ylookup[100] + 100;
//		memset(p, gStdColor[15], 200);
//		for (int i = 0; i < 200; i++) {
//			*p = gStdColor[15];
//			p += bytesperline;
//		}
//		p -= bytesperline;
//		memset(p, gStdColor[15], 200);
//		enddrawing();	//}}}
//		gfxSetClip(100, 100, 100+200, 100+200);
//		int x1 = 110, y1 = 310, x2 = 200, y2 = 100 - 20;
//		gfxDrawLine(x1, y1, x2, y2, gStdColor[15]);
//		if (ClipLine(&x1, &y1, &x2, &y2)) {
//			gfxDrawPixel(x1, y1, gStdColor[4]);
//			gfxDrawPixel(x2, y2, gStdColor[4]);
//		}
//		gfxDrawPixel(110, 110, gStdColor[4]);
//		Bsnprintf(buf, sizeof(buf), "x1 = %d, y1 = %d, x2 = %d, y2 = %d", x1, y1, x2, y2);
//		gDbgFont.DrawString(10, 100+200+20, buf, gStdColor[15]);

//		CalcFrameRate();

		scrNextPage();
		gFrames++;

		// FPS stuff
//		frames++;
//		QueryPerformanceCounter((LARGE_INTEGER *)&cur_ticks);
//		cur_ticks -= loop_start;
//		if ((cur_ticks - last_ticks) >= freq)
//		{
//			last_ticks = cur_ticks;
//			fps = frames;
//			frames = 0;
//		}

		// Fire time
		if (gotpic[292] & 0x40)
		{
			FireProcess();
			gotpic[292] &= 0xBF;
		}

		if ( gStartNewGame )
		{
			if ( gGameOptions.nGameType )
			{
				StartNetworkLevel();
			}
			else
			{
				if ( gGameOptions.uGameFlags & 1 )
					StartLevel(&gGameOptions);
				else
					levelStartGame();
			}
			gStartNewGame = 0;
		}
	}
}


int BloodMain(int argc, char *argv[])
{
	dprintf("%s in %s\n", __FUNCTION__, __FILE__);

	assert(sizeof(GAMEOPTIONS) == 393);
	memcpy(&gGameOptions, &gSingleGameOptions, sizeof(GAMEOPTIONS));
	ParseOptions();
//	if ( !gExplicitSetup )
//		CONFIG_GetSetupFilename();
	CONFIG_ReadSetup();

//	Init(&gSysRes, "BLOOD.RFF", (int)&off_F0139);
	gSysRes.Init( "BLOOD.RFF", 0 );
	gGuiRes.Init( "GUI.RFF", 0 );

//	__asm { int     10h             ; - VIDEO - GET CURRENT VIDEO MODE }
//	LOBYTE(gOldDisplayMode) = Init(&gGuiRes, "GUI.RFF", 0);
//	gOldDisplayMode = (unsigned __int8)gOldDisplayMode;
//	sprintf_(&buffer, "BLOOD SHAREWARE BETA BUILD 0.91 [%s] -- DO NOT DISTRIBUTE", &gBuildDate);
//	tioInit(v11);
//	tioCenterString(78, v12, v15);
//	tioCenterString(78, v12, v15);
//	tioWindow(v12, v15);
//	keyInstall();
//	tioPrint((char *)&unk_F01BA);

	tioPrint( "Initializing 3d engine" );
	scrInit();

	// Initialize mouse (Required?)
	initmouse();
	initinput();
//	grabmouse(1);

	inittimer(120);
//	installusertimercallback(mytimer_callback);
	tioPrint("Creating standard color lookups");
	scrCreateStdColors();

	tioPrint("Loading tiles");
	if (!tileInit(0))
		ThrowError("ART files not found");

//	powerupInit();

	tioPrint( "Loading cosine table" );
	trigInit( &gSysRes );

//	tioPrint("Initializing view subsystem");
//	viewInit();

	tioPrint("Initializing dynamic fire");
	FireInit();

	tioPrint("Initializing weapon animations");
	WeaponInit();

//	prevErrorHandler = errSetHandler(GameErrorHandler);
//	LoadSavedInfo();
//	LoadDemoInfo(&gDemo);
//	sprintf_(&buffer, "There are %d demo(s) in the loop", dword_28A177);
//	tioPrint((char *)&buffer);
//	if ( !gDemo )
//	{
//		if ( dword_28A177 > 0 )
//		{
//			if ( !gGameOptions )
//				SetupPlayback(&gDemo, 0);
//		}
//	}
//	tioPrint("Loading control setup");
//	ctrlInit();
//	tioPrint("Installing timer");
//	UnlockClockStrobe();
//	timerRegisterClient(v12, v15);
//	timerInstall();
//	tioPrint("Initializing Redbook Audio");
//	if ( (unsigned __int16)cdrom_setup(&Redbook)
//		&& (unsigned __int16)cdrom_installed(&Redbook)
//		&& (HIBYTE(v3) = bNoCDAudio, !bNoCDAudio)
//		&& (LOBYTE(v4) = gGameOptions, !gGameOptions) )
//	{
//		LOBYTE(v3) = 0;
//		gRedBookInstalled = 1;
//		word_155E7D = v3;
//		HIBYTE(v4) = 0;
//		word_155E7B = 0;
//		word_155E7F = v4;
//		newdisk(&Redbook);
//		preprocess(&Redbook, 255);
//		SetVolume(&Redbook);
//		v10 = "Redbook Audio initialized";
//	}
//	else
//	{
//		v10 = "Redbook Audio not initialized";
//		gRedBookInstalled = 0;
//	}
//	tioPrint(v10);

	tioPrint("Initializing sound system");
	sndInit();

//	sfxInit();
//	tioPrint("Initializing network users");
//	netInitialize();
//	v5 = 787 * myconnectindex;
	gViewIndex = 0;	// TODO: Should be myconnectindex
	gView = &gPlayer[gViewIndex];
	gMe   = &gPlayer[gViewIndex];
//	tioPrint("Waiting for network players");
//	netWaitForEveryone();
	scrSetGameMode(ScreenMode, ScreenWidth, ScreenHeight);
	scrSetGamma(gGamma);
//	netBroadcastPlayerInfo(v14);
//	netWaitForEveryone();
	viewResizeView(gViewSize);
//	Initialize(windowx2 - windowx1 + 1, windowy2 - windowy1 + 1, &gYLookup, 0, 32, -1);

	// We might have a problem here passing it frameplace
	gWeather.Initialize(NULL, windowx1, windowy1,
		windowx2 - windowx1 + 1,
		windowy2 - windowy1 + 1,
		gYLookup,
		0, 32, -1);

	levelLoadDefaults();
	scrSetDac();
//	credLogos();

//	if ( bAddUserMap )
//		levelAddUserMap(v14);
	SetupMenus();
	if (gGameOptions.nGameType == GT_SINGLE)
	{
		// TODO: Flag check here
		gGameMenuMgr.Push(&menuMain, -1);
	}
//	if ( dword_28A177 > 0 )
//		Deactivate(&gGameMenuMgr);
	if (!bAddUserMap)
		gGameMenuMgr.Push(&menuMain, -1);
	ready2send = true;

	MainLoop();
	dprintf("Left main loop...\n");

//	if ( gDemo )
//		Close(&gDemo);
//	ready2send = 0;
//	if ( gGameOptions > 0 )
//	{
//		if ( numplayers > 1 )
//		{
//			netSendEmptyPackets();
//			sendlogon_();
//		}
//	}
	setview(0, 0, xdim - 1, ydim - 1);
// 2190 = some tiles for numbers
// 3808 = alphabet tiles

//	scrSetDac();
//	while (1)
//	{
//		handleevents();
//		if (keyGet()) break;
//		clearview(0);
//		rotatesprite(160<<16, 100<<16, 65536, 0, 2044, 0, 0, 64|8|2, 0, 0, xdim - 1, ydim - 1);
//		scrNextPage();
//	}

//	tileBlit(2044, 0, 0);
//	Sleep(1000);
//	v8 = gGameClock + 600;
//	while ( v8 > gGameClock && !keyGet() )
//		;
//	__asm { int     10h             ; - VIDEO -  }
	sndTerm();
//	timerRemove();
//	ctrlTerm();
//	UnlockClockStrobe();
	tileTerm();

	// We are responsible for freeing the following stuff, not the engine
	transluc = NULL;
	for (int i = 0; i < MAXPALOOKUPS; i++)
		palookup[i] = NULL;

	uninittimer();
	uninitengine();
//	CONFIG_WriteSetup();
//	if ( syncstate )
//		printf_("A packet was lost! (syncstate)\n", (char)v14);
//	if ( gRedBookInstalled )
//	{
//		StopSong(&Redbook);
//		cdrom_shutdown(&Redbook);
//	}
//	return errSetHandler(prevErrorHandler);

	return 0;
}


// Below is our entry-point from the OS layers
#ifdef __cplusplus
extern "C" int app_main(int argc, char **argv)
#else
int app_main(int argc, char **argv)
#endif
{
	return BloodMain(argc, argv);

	dprintf("app_main in %s\n", __FILE__);
	wm_setapptitle("Blood");
	if (initengine())
	{
		wm_msgbox("Error", "Failed initialising build engine: %s", engineerrstr);
		return 0;
	}
	if (setgamemode(0, 640, 480, 8) < 0)
	{
		wm_msgbox("Error", "Failed setting game mode");
		return 0;
	}
	char pal[768];
	memset(pal, 0, sizeof(pal));
	pal[(255*3)+0] = 63;
	pal[(255*3)+1] = 0;
	pal[(255*3)+2] = 0;
//	setbrightness(0, pal, 4|2);
	int foo = 0;
	setview(100, 100, 100+200, 100+200);
	dprintf("xdim = %d, ydim = %d, windowx1 = %d, windowx2 = %d\n", xdim, ydim, windowx1, windowx2);
	while (1)
	{
		if (handleevents())
		{
			// Got an event
			if (quitevent) break;
		}

		clearview(255);
		foo = (foo + 1) & 255;
		nextpage();
	}
	uninitengine();
	return 0;
}


extern "C" void faketimerhandler()
{
}
