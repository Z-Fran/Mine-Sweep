/****************************************
Graphic.h
Author: Root
Date: 2019/12/09
Description:
Defines the rendering framework header file, using Graphic namespace to prevent naming conflicts
Function descriptions:
void MovePos()  Move current position to specified coordinates
void ModChar()  Change character at current position to new newChar
void ModColor() Change character color/background at current position (which one to modify is specified by mode) to new color value
void PutChar()  Output an outChar at current position
void PutCharWithColor()      Output an outChar at current position with specified color
void PutString()             Output a string outString at current position
void PutStringWithColor()    Output a string outString at current position with specified color
void DrawRect()              Draw a rectangle with width and height at current position
void DrawRectWithColor()     Draw a rectangle with width and height at current position with specified color
void ClearScreen()           Force clear the screen
void InitGraphic()           Initialize graphics parameters and screen size to width and height
void DestroyGraphic()        Destroy parameters
void Update()                Update all parameters to screen, meaning all parameter changes are not real-time, only when this function is called will the screen be updated
*****************************************/
#pragma once
#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <stdio.h>
#include <windows.h>

namespace Graphic {
	void MovePos(SHORT x, SHORT y);
	void ModChar(const char newChar);
	void ModColor(DWORD mode, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void PutChar(const char outChar);
	void PutCharWithColor(const char outChar, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void PutString(const char* outString);
	void PutStringWithColor(const char* outString, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void DrawRect(const SHORT width, const SHORT height);
	void DrawRectWithColor(const SHORT width, const SHORT height, const BYTE textR, const BYTE textG, const BYTE textB, const BYTE backR, const BYTE backG, const BYTE backB);
	void ClearScreen();
	void InitGraphic(int width, int height);
	void DestroyGraphic();
	void Update();
}
#endif