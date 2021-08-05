#ifndef __NET_H__
#define __NET_H__


void netCheckSync();
void netGetPackets();
void netBroadcastMyLogoff();
void netBroadcastPlayerInfo(int);
void netBroadcastNewGame();
void netWaitForEveryone();
void netSendEmptyPackets();
void netGetInput();
void netInitialize();
void netPlayerQuit(int);


#endif // __NET_H__
