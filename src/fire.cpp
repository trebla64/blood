#include "stdafx.h"
#include "game.h"
#include "resource.h"
#include "tile.h"
#include "error.h"


static uint8 FrameBuffer[17280];
static uint8 CoolTable[1024];
static uint8 SeedBuffer[16][128];


#ifdef _MSC_VER


static void BuildCoolTable()
{
	for (int i = 0; i < 1024; i++)
	{
		int _a;
		__asm {
			// Clobbers edx, eax
			push eax
			push edx

			mov edx, i
			sub edx, 6	// 6 = gDamping [Constant]
			mov eax, edx
			sar edx, 31
			shl edx, 2
			sbb eax, edx
			sar eax, 2
			mov _a, eax

			pop edx
			pop eax
		}
		if (_a >= 0) CoolTable[i] = _a & 0xFF;
	}
}


static void InitSeedBuffers()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			uint8 *p = &SeedBuffer[i][j << 1];
			int r = rand();
			p[0] = r;
			p[1] = r;
		}
	}
}


__declspec(naked) static void __cdecl CellularFrame(uint8 *buffer, int width, int height)
{
	__asm {
//#if 0
		// Ret addr	// 4
		push eax	// 8
		push edx	// 12
		push ebx	// 16
		push ecx	// 20
		push esi	// 24
		push edi	// 28
		push ebp	// 32

		mov esi, [esp + 32 + 0]
		mov edi, [esp + 32 + 4]
		mov eax, [esp + 32 + 8]

		imul edi
		mov ebp, eax
		sub eax, eax	// eax = 0
		sub ecx, ecx	// ecx = 0
		dec esi

label0:
		lea     edx, [esi+edi]
		inc     esi
		mov     al, [edx]
		mov     cl, [edx+1]
		mov     ebx, eax
		add     ebx, ecx
		mov     al, [edx+2]
		add     edx, edi
		add     ebx, eax
		mov     cl, [edx+1]
		add     ebx, ecx
		cmp     cl, 60h
		ja      short label2
		mov     al, CoolTable[ebx]
		dec     ebp
		mov     [esi], al
		jz      short label1
		lea     edx, [esi+edi]
		inc     esi
		mov     al, [edx]
		mov     cl, [edx+1]
		mov     ebx, eax
		add     ebx, ecx
		mov     al, [edx+2]
		add     edx, edi
		add     ebx, eax
		mov     cl, [edx+1]
		add     ebx, ecx
		cmp     cl, 60h
		ja      short label2
		mov     al, CoolTable[ebx]
		dec     ebp
		mov     [esi], al
		jz      short label1
		lea     edx, [esi+edi]
		inc     esi
		mov     al, [edx]
		mov     cl, [edx+1]
		mov     ebx, eax
		add     ebx, ecx
		mov     al, [edx+2]
		add     edx, edi
		add     ebx, eax
		mov     cl, [edx+1]
		add     ebx, ecx
		cmp     cl, 60h
		ja      short label2
		mov     al, CoolTable[ebx]
		dec     ebp
		mov     [esi], al
		jnz     short label0

//label1:
label2:
		add     ebx, ecx
		mov     al, [edx]
		mov     cl, [edx+2]
		add     edx, edi
		add     ebx, eax
		add     ebx, ecx
		mov     al, [edx+1]
		add     ebx, eax
		shr     ebx, 1
		mov     al, CoolTable[ebx]
		dec     ebp
		mov     [esi], al
		jz      short label1
		lea     edx, [esi+edi]
		inc     esi
		mov     al, [edx]
		mov     cl, [edx+1]
		mov     ebx, eax
		add     ebx, ecx
		mov     al, [edx+2]
		add     edx, edi
		add     ebx, eax
		mov     cl, [edx+1]
		add     ebx, ecx
		cmp     cl, 60h
		jbe     short label3
		add     ebx, ecx
		mov     al, [edx]
		mov     cl, [edx+2]
		add     edx, edi
		add     ebx, eax
		add     ebx, ecx
		mov     al, [edx+1]
		add     ebx, eax
		shr     ebx, 1
		mov     al, CoolTable[ebx]
		dec     ebp
		mov     [esi], al
		jnz     label0
		jmp label1

label3:
		mov al, CoolTable[ebx]
		dec ebp
		mov [esi], al
		jnz label0

label1:
		// Done
		pop ebp
		pop edi
		pop esi
		pop ecx
		pop ebx
		pop edx
		pop eax

//#endif
		ret
	}
}


static void DoFireFrame()
{
	int foo = rand();
	for (int i = 0; i < 3; i++)
		memcpy(&FrameBuffer[128 * i + 16896], &SeedBuffer[foo & 0xF][0], 128);
	CellularFrame(FrameBuffer, 128, 132);

	uint8 *fb  = FrameBuffer;
	uint8 *clu = gCLU;
	uint8 *tile = tileLoadTile(2342);
	for (int j = 0; j < 128; j++)
	{
		uint8 *tile_store = tile;
		for (int i = 0; i < 128; i++)
		{
			// Fetch from framebuffer and lookup in CLU
			*tile = clu[*fb];
			fb++;
			tile += 128;
		}
		tile = tile_store + 1;
	}
}


void FireInit()
{
	memset(FrameBuffer, 0, sizeof(FrameBuffer));
	memset(CoolTable, 0, sizeof(CoolTable));
	BuildCoolTable();
	InitSeedBuffers();

	ResHandle h = gSysRes.Lookup("RFIRE", "CLU");
	if (h.isNull())
		ThrowError("RFIRE.CLU not found");

	gCLU = (uint8 *)gSysRes.Lock(h);

	for (int i = 0; i < 100; i++)
		DoFireFrame();
}


void FireProcess()
{
	static int lastUpdate = 0;
	if ((gGameClock < lastUpdate) || (lastUpdate + 2) < gGameClock)
	{
		DoFireFrame();
		lastUpdate = gGameClock;
	}
}

#else

void FireInit()		{}
void FireProcess()	{}

#endif
