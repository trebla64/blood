#include "stdafx.h"
#include "controls.h"


static const char * const gamefunctions[] =
{
	"Move_Forward",				// 0
	"Move_Backward",			// 1
	"Turn_Left",				// 2
	"Turn_Right",				// 3
	"Turn_Around",				// 4
	"Strafe",					// 5
	"Strafe_Left",				// 6
	"Strafe_Right",				// 7
	"Jump",						// 8
	"Crouch",					// 9
	"Run",						// 10
	"AutoRun",					// 11
	"Open",						// 12
	"Weapon_Fire",				// 13
	"Weapon_Special_Fire",		// 14
	"Aim_Up",					// 15
	"Aim_Down",					// 16
	"Aim_Center",				// 17
	"Look_Up",					// 18
	"Look_Down",				// 19
	"Tilt_Left",				// 20
	"Tilt_Right",				// 21
	"Weapon_1",					// 22
	"Weapon_2",					// 23
	"Weapon_3",					// 24
	"Weapon_4",					// 25
	"Weapon_5",					// 26
	"Weapon_6",					// 27
	"Weapon_7",					// 28
	"Weapon_8",					// 29
	"Weapon_9",					// 30
	"Weapon_10",				// 31
	"Inventory_Use",			// 32
	"Inventory_Left",			// 33
	"Inventory_Right",			// 34
	"Map_Toggle",				// 35
	"Map_Follow_Mode",			// 36
	"Shrink_Screen",			// 37
	"Enlarge_Screen",			// 38
	"Send_Message",				// 39
	"See_Coop_View",			// 40
	"See_Chase_View",			// 41
	"Mouse_Aiming",				// 42
	"Toggle_Crosshair",			// 43
	"Next_Weapon",				// 44
	"Previous_Weapon",			// 45
	"Holster_Weapon",			// 46
	"Show_Opponents_Weapon",	// 47
	"BeastVision",				// 48
	"CrystalBall",				// 49
	"JumpBoots",				// 50
	"MedKit",					// 51
	"ProximityBombs",			// 52
	"RemoveBombs"				// 53
};
CT_ASSERT((NUM_ELEMENTS(gamefunctions) == NUM_BUTTONS) && (NUM_BUTTONS == 54));


void CONFIG_ReadSetup()
{
	CONTROL_ClearAssignments();
}


int CONFIG_FunctionNameToNum(const char *pzName)
{
	return -1;
}
