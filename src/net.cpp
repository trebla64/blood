#include "stdafx.h"
#include "game.h"
#include "controls.h"

#include "jfbuild/build.h"


void netCheckSync()
{
}


void netGetPackets()
{
}


void netBroadcastMyLogoff()
{
}


void netBroadcastPlayerInfo(int)
{
}


void netBroadcastNewGame()
{
}


void netWaitForEveryone()
{
}


void netSendEmptyPackets()
{
}


void netGetInput()
{
//	if (numplayers > 1)
//		netGetPackets();
	ctrlGetInput();
	NEW_INPUT *pInput = &gFifoInput[8 * (gNetFifoHead[0] & 0xFF)] + 0;
	memcpy(pInput, &gInput, sizeof(NEW_INPUT));
	gNetFifoHead[0]++;
}


void netInitialize()
{
}


void netPlayerQuit(int)
{
}
