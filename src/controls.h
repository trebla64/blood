#ifndef __CONTROLS_H__
#define __CONTROLS_H__


enum Buttons
{
	BTN_MOVE_FORWARD = 0,
	BTN_MOVE_BACKWARD,	// 1
	BTN_TURN_LEFT,		// 2
	BTN_TURN_RIGHT,		// 3
	BTN_TURN_AROUND,	// 4
	BTN_STRAFE,			// 5
	BTN_STRAFE_LEFT,	// 6
	BTN_STRAFE_RIGHT,	// 7
	BTN_JUMP,			// 8
	BTN_CROUCH,			// 9
	BTN_RUN,			// 10
	BTN_AUTORUN,
	BTN_OPEN,
	BTN_WEAPON_FIRE,
	BTN_WEAPON_SPECIAL_FIRE,
	BTN_AIM_UP,
	BTN_AIM_DOWN,
	BTN_AIM_CENTER,
	BTN_LOOK_UP,
	BTN_LOOK_DOWN,
	BTN_TILT_LEFT,
	BTN_TILT_RIGHT,
	BTN_WEAPON_1,
	BTN_WEAPON_2,
	BTN_WEAPON_3,
	BTN_WEAPON_4,
	BTN_WEAPON_5,
	BTN_WEAPON_6,
	BTN_WEAPON_7,
	BTN_WEAPON_8,
	BTN_WEAPON_9,
	BTN_WEAPON_10,
	BTN_INVENTORY_USE,
	BTN_INVENTORY_LEFT,
	BTN_INVENTORY_RIGHT,
	BTN_MAP_TOGGLE,
	BTN_MAP_FOLLOW_MODE,
	BTN_SHRINK_SCREEN,
	BTN_ENLARGE_SCREEN,
	BTN_SEND_MESSAGE,
	BTN_SEE_COOP_VIEW,
	BTN_SEE_CHASE_VIEW,
	BTN_MOUSE_AIMING,
	BTN_TOGGLE_CROSSHAIR,
	BTN_NEXT_WEAPON,
	BTN_PREVIOUS_WEAPON,
	BTN_HOLSTER_WEAPON,
	BTN_SHOW_OPPONENTS_WEAPON,
	BTN_BEAST_VISION,
	BTN_CRYSTAL_BALL,
	BTN_JUMP_BOOTS,
	BTN_MEDKIT,
	BTN_PROXIMITY_BOMBS,
	BTN_REMOVE_BOMBS,

	NUM_BUTTONS
};


void CONTROL_GetMouseDelta();
void CONTROL_GetMouseSensitivity();
void CONTROL_SetMouseSensitivity();
void CONTROL_GetMouseButtons();
void CONTROL_StartMouse();
void CONTROL_GetJoyAbs();
void CONTROL_FilterJoyDelta();
void CONTROL_GetJoyDelta();
void CONTROL_JoyButtons();
void CONTROL_SetJoyScale();
void CONTROL_SetupJoy();
void CONTROL_CenterJoystick();
void CONTROL_SetJoystickHatBias();
void CONTROL_GetJoystickHatBias();
void CONTROL_StartJoy();
void CONTROL_ExternalStartup();
void CONTROL_ExternalSetup();
void CONTROL_FillExternalInfo();
void CONTROL_PollExternalControl();
void CONTROL_GetTime();
void CONTROL_CheckRange();
void CONTROL_SetFlag();
void CONTROL_KeyboardFunctionPressed();
void CONTROL_ClearKeyboardFunction();
void CONTROL_DefineFlag();
void CONTROL_FlagActive();
void CONTROL_MapKey();
void CONTROL_GetKeyMap();
void CONTROL_PrintKeyMap();
void CONTROL_PrintControlFlag();
void CONTROL_PrintAxes();
void CONTROL_MapButton();
void CONTROL_GetButtonMap();
void CONTROL_MapAnalogAxis();
void CONTROL_GetAnalogAxisMap();
void CONTROL_SetAnalogAxisScale();
void CONTROL_GetAnalogAxisScale();
void CONTROL_MapDigitalAxis();
void CONTROL_GetDigitalAxisMap();
void CONTROL_ClearFlags();
void CONTROL_ClearAssignments();
void CONTROL_GetDeviceButtons();
void CONTROL_DigitizeAxis();
void CONTROL_ScaleAxis();
void CONTROL_ApplyAxis();
void CONTROL_PollDevices();
void CONTROL_AxisFunctionState();
void CONTROL_ButtonFunctionState();
void CONTROL_GetUserInput();
void CONTROL_ClearUserInput();
void CONTROL_ClearButton(int nButton);
void CONTROL_GetInput();
void CONTROL_WaitRelease();
void CONTROL_Ack();
void CONTROL_Startup();
void CONTROL_Shutdown();


void ctrlInit();
void ctrlTerm();
void ctrlGetInput();


#endif // __CONTROLS_H__
