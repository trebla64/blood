#include "stdafx.h"
#include "debug.h"

#include "jfbuild/baselayer.h"


void keyFlushStream()
{
	keyfifoplc = keyfifoend;
}


uint8 keyGet()
{
	while (1)
	{
		if (keyfifoplc == keyfifoend)
			return 0;
		uint8 code = *(uint8 *)&keyfifo[keyfifoplc];
		keyfifoplc = (keyfifoplc + 1) & (KEYFIFOSIZ - 1);
		uint8 state = *(uint8 *)&keyfifo[keyfifoplc];
		keyfifoplc = (keyfifoplc + 1) & (KEYFIFOSIZ - 1);
		switch (state)
		{
			case 0:
//				dprintf("Button %02x went up\n", code);
				break;
			case 1:
//				dprintf("Button %02x went down\n", code);
				return code;
				break;
		}
	}
}
