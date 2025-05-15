/****************************************
Controller.h
Author: Root
Date: 2019/12/09
Description:
Defines the controller framework header file, using Controller namespace to prevent naming conflicts
Function descriptions:
void FlushInput()      Flush the input buffer
char GetKeyHit()       Get keyboard input
COORD GetCursorPos()   Get current cursor position (coordinates within console)
COORD GetCursorHitPos(DWORD type = 1) Get current click position, type determines whether to get left/right click or double click
void InitController()  Initialize the controller
*****************************************/
#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <windows.h>

namespace Controller {
	void FlushInput();
	char GetKeyHit();
	COORD GetCursorPos();
	COORD GetCursorHitPos(DWORD type = 1);
	void InitController();
}
#endif