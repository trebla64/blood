#include "stdafx.h"
#include "game.h"
#include "key.h"
#include "trig.h"
#include "controls.h"
#include "view.h"
#include "debug.h"

#include "jfbuild/baselayer.h"
#include "jfbuild/pragmas.h"


struct CONTROLFLAGS
{
	uint8 field_0;
	uint8 field_1;
	uint8 field_2;
	uint8 field_3;
	int   field_4;
};

struct KeyMapping
{
	uint8 foo;
	uint8 bar;
};

//static uint32       CONTROL_ButtonState1;
//static uint32       CONTROL_ButtonState2;
static uint64       CONTROL_ButtonState;
static uint64       CONTROL_ButtonHeldState;
static CONTROLFLAGS CONTROL_Flags[64];
static KeyMapping   CONTROL_KeyMapping[64];


void CONTROL_GetMouseDelta()
{
}


void CONTROL_GetMouseSensitivity()
{
}


void CONTROL_SetMouseSensitivity()
{
}


void CONTROL_GetMouseButtons()
{
}


void CONTROL_StartMouse()
{
}


void CONTROL_GetJoyAbs()
{
}


void CONTROL_FilterJoyDelta()
{
}


void CONTROL_GetJoyDelta()
{
}


void CONTROL_JoyButtons()
{
}


void CONTROL_SetJoyScale()
{
}


void CONTROL_SetupJoy()
{
}


void CONTROL_CenterJoystick()
{
}


void CONTROL_SetJoystickHatBias()
{
}


void CONTROL_GetJoystickHatBias()
{
}


void CONTROL_StartJoy()
{
}


void CONTROL_ExternalStartup()
{
}


void CONTROL_ExternalSetup()
{
}


void CONTROL_FillExternalInfo()
{
}


void CONTROL_PollExternalControl()
{
}


void CONTROL_GetTime()
{
}


void CONTROL_CheckRange()
{
}


void CONTROL_SetFlag()
{
}


void CONTROL_KeyboardFunctionPressed()
{
}


void CONTROL_ClearKeyboardFunction()
{
}


void CONTROL_DefineFlag()
{
}


void CONTROL_FlagActive()
{
}


void CONTROL_MapKey(int nKey, uint8 foo, uint8 bar)
{
	dassert(nKey >= 0 && nKey < 64);
	CONTROL_KeyMapping[nKey].foo = foo;
	CONTROL_KeyMapping[nKey].bar = bar;
}


void CONTROL_GetKeyMap(int nKey, int *foo, int *bar)
{
	dassert(nKey >= 0 && nKey < 64);
	dassert(foo != NULL);
	dassert(bar != NULL);
	*foo = CONTROL_KeyMapping[nKey].foo;
	*bar = CONTROL_KeyMapping[nKey].bar;
}


void CONTROL_PrintKeyMap()
{
}


void CONTROL_PrintControlFlag()
{
}


void CONTROL_PrintAxes()
{
}


void CONTROL_MapButton()
{
}


void CONTROL_GetButtonMap()
{
}


void CONTROL_MapAnalogAxis()
{
}


void CONTROL_GetAnalogAxisMap()
{
}


void CONTROL_SetAnalogAxisScale()
{
}


void CONTROL_GetAnalogAxisScale()
{
}


void CONTROL_MapDigitalAxis()
{
}


void CONTROL_GetDigitalAxisMap()
{
}


void CONTROL_ClearFlags()
{
}


void CONTROL_ClearAssignments()
{
	memset(CONTROL_KeyMapping, 127, sizeof(CONTROL_KeyMapping));
}


void CONTROL_GetDeviceButtons()
{
}


void CONTROL_DigitizeAxis()
{
}


void CONTROL_ScaleAxis()
{
}


void CONTROL_ApplyAxis()
{
}


void CONTROL_PollDevices()
{
}


void CONTROL_AxisFunctionState()
{
}


void CONTROL_ButtonFunctionState()
{
}


void CONTROL_GetUserInput()
{
}


void CONTROL_ClearUserInput()
{
}


void CONTROL_ClearButton(int nButton)
{
//	if (nButton <= 31)
//		CONTROL_ButtonState1 &= ~(1 << nButton);
//	else
//		CONTROL_ButtonState2 &= ~(1 << (nButton - 32));
//	CONTROL_Flags[nButton].field_4 = 1;
	CONTROL_ButtonState &= ~(uint64(1) << nButton);
}


// Only called by ctrlGetInput()
void CONTROL_GetInput()
{
	CONTROL_ButtonHeldState = CONTROL_ButtonState;
	CONTROL_ButtonState = 0;

	if (keystatus[BKEY_W])
		CONTROL_ButtonState |= (uint64(1) << BTN_MOVE_FORWARD);
	if (keystatus[BKEY_S])
		CONTROL_ButtonState |= (uint64(1) << BTN_MOVE_BACKWARD);
	if (keystatus[BKEY_A])
		CONTROL_ButtonState |= (uint64(1) << BTN_TURN_LEFT);
	if (keystatus[BKEY_D])
		CONTROL_ButtonState |= (uint64(1) << BTN_TURN_RIGHT);
	if (keystatus[0xd])
	{
		keystatus[0xD] = 0;
		CONTROL_ButtonState |= (uint64(1) << BTN_ENLARGE_SCREEN);
		dprintf("Enlarging screen...\n");
	}
	if (keystatus[0xc])
	{
		keystatus[0xC] = 0;
		CONTROL_ButtonState |= (uint64(1) << BTN_SHRINK_SCREEN);
		dprintf("Shrinking screen...\n");
	}
}


void CONTROL_WaitRelease()
{
}


void CONTROL_Ack()
{
}


void CONTROL_Startup()
{
}


void CONTROL_Shutdown()
{
}


void ctrlInit()
{
}


void ctrlTerm()
{
}


void ctrlGetInput()
{
	static int iTurnCount = 0;

	memset(&gInput, 0, sizeof(gInput));
	CONTROL_GetInput();
	if (gQuitRequest)
		gInput.buttonFlags |= 0x10;

	if (gGameStarted && gInputMode != 1 && gInputMode != 2)
	{
//		if (BIT_ISSET(CONTROL_ButtonState2, 7))
//		{
//			CONTROL_ClearButton(39);
//			keyFlushStream();
//			gInputMode = 2;
//			return;
//		}

//		if (BIT_ISSET(CONTROL_ButtonState, BTN_SHRINK_SCREEN))
		if (CONTROL_ButtonState & (uint64(1) << BTN_SHRINK_SCREEN))
		{
			if (gViewMode == 3)
			{
				CONTROL_ClearButton(BTN_SHRINK_SCREEN);
				viewResizeView(gViewSize + 1);
			}
			if (gViewMode == 2 || gViewMode == 4)
			{
				// TODO: Zoom stuff
			}
		}

//		if (BIT_ISSET(CONTROL_ButtonState, BTN_ENLARGE_SCREEN))
		if (CONTROL_ButtonState & (uint64(1) << BTN_ENLARGE_SCREEN))
		{
			if (gViewMode == 3)
			{
				CONTROL_ClearButton(BTN_ENLARGE_SCREEN);
				viewResizeView(gViewSize - 1);
			}
		}

		int bRunning = BIT_ISSET(CONTROL_ButtonState, BTN_RUN) || gAutoRun;
		if (BIT_ISSET(CONTROL_ButtonState, BTN_MOVE_FORWARD))
			gInput.forward += 4 * (bRunning + 1);
		if (BIT_ISSET(CONTROL_ButtonState, BTN_MOVE_BACKWARD))
			gInput.forward -= 4 * (bRunning + 1);

		bool bTurnLeft  = false;
		bool bTurnRight = false;

		// XXX: Remove, always strafing
		CONTROL_ButtonState |= (1 << BTN_STRAFE);

		if (BIT_ISSET(CONTROL_ButtonState, BTN_STRAFE))
		{
			// When holding strafe button, turn left and right functions as strafing
			if (BIT_ISSET(CONTROL_ButtonState, BTN_TURN_LEFT))
				gInput.strafe += 4 * (bRunning + 1);
			if (BIT_ISSET(CONTROL_ButtonState, BTN_TURN_RIGHT))
				gInput.strafe -= 4 * (bRunning + 1);
		}
		else
		{
			// Not holding strafe button
			if (BIT_ISSET(CONTROL_ButtonState, BTN_STRAFE_LEFT))
				gInput.strafe += 4 * (bRunning + 1);
			if (BIT_ISSET(CONTROL_ButtonState, BTN_STRAFE_RIGHT))
				gInput.strafe -= 4 * (bRunning + 1);
			// Actually turning left and right
			if (BIT_ISSET(CONTROL_ButtonState, BTN_TURN_LEFT))  bTurnLeft = true;
			if (BIT_ISSET(CONTROL_ButtonState, BTN_TURN_RIGHT)) bTurnRight = true;
		}

		if (bTurnLeft || bTurnRight)
			iTurnCount += 4;
		else
			iTurnCount = 0;

		if (bTurnLeft)
		{
			int foo = gTurnSpeed;
			if ((12 * iTurnCount) < gTurnSpeed)
				foo = 12 * iTurnCount;
	//		gInput.turn -= foo;
		}

		if (bTurnRight)
		{
			int foo = gTurnSpeed;
			if ((12 * iTurnCount) < gTurnSpeed)
				foo = 12 * iTurnCount;
	//		gInput.turn += foo;
		}

	//	if (bRunning && iTurnCount > 24)
	//		gInput.turn *= 2;

		// This appears to be strafe/turn clamping code (TODO)
		if (BIT_ISSET(CONTROL_ButtonState, BTN_STRAFE))
		{
		}
		else
		{
		}
	}

}
