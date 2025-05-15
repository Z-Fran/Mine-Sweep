/****************************************
Controller.cpp
Author: Root
Date: 2019/12/09
Description:
Defines the controller framework, using Controller namespace to prevent naming conflicts
£¡*£¡ Note: Unless you fully understand all internal operations, it is not recommended to modify this file!*£¡
*****************************************/
#include "Controller.h"

namespace Controller {
	COORD pos = { 0 };                   // Mouse coordinates
	HANDLE hStdIn = NULL;				 // Standard input handle
	INPUT_RECORD inRecArr[256] = { 0 };	 // Input buffer array
	INPUT_RECORD inRec = { 0 };			 // Final input to be obtained
	DWORD dRead = 0;					 // Length of read input buffer
	char key = '0';						 // Keyboard key
	const DWORD MOVE = 0, LEFT_CLICK = 1, RIGHT_CLICK = 2, _DOUBLE_CLICK = 3; // Some constants

	/****************************************
	Function:  updateInput(DWORD type, DWORD hitMode = MOVE)
	Parameter: DWORD type, hitMode
	Return:    bool
	Description:
	Sets the inRec variable to the input type (mouse or keyboard) to be obtained. Returns true and sets inRec variable if the required input is found, otherwise returns false.
	hitMode specifies the mouse click mode: left click/right click or double click
	*****************************************/
	bool updateInput(DWORD type, DWORD hitMode = MOVE) {
		// Loop search starting from the last data in the input buffer
		for (int i = (int)dRead; i > 0; i--) {
			// If this data type matches the input
			if (inRecArr[i - 1].EventType == type) {
				// Check hitMode, skip switch if it's MOVE
				switch (hitMode) {
				case LEFT_CLICK:
					// Left click
					if (inRecArr[i - 1].Event.MouseEvent.dwButtonState != 1) {
						continue;
					}
					break;
				case RIGHT_CLICK:
					// Right click
					if (inRecArr[i - 1].Event.MouseEvent.dwButtonState != RIGHTMOST_BUTTON_PRESSED) {
						continue;
					}
					break;
				case _DOUBLE_CLICK:
					// Double click
					if (inRecArr[i - 1].Event.MouseEvent.dwEventFlags != DOUBLE_CLICK) {
						continue;
					}
					break;
				}
				// Set inRec and return true to indicate required input was found
				inRec = inRecArr[i - 1];
				return true;
			}
		}
		return false;
	}

	/****************************************
	Function:  FlushInput()
	Parameter: None(void)
	Return:    None(void)
	Description:
	Refreshes the input buffer and reads all data from the buffer
	*****************************************/
	void FlushInput() {
		if (WaitForSingleObject(hStdIn, 0) == WAIT_OBJECT_0) {
			// If there is data in the buffer, read it out, maximum 256
			ReadConsoleInputA(hStdIn, &inRecArr[0], 256, &dRead);
		}
		else {
			// Otherwise set Read to 0
			dRead = 0;
		}
	}

	/****************************************
	Function:  GetKeyHit()
	Parameter: None(void)
	Return:    char
	Description:
	Gets keyboard input, returns '\0' if no key is hit
	*****************************************/
	char GetKeyHit() {
		if (updateInput(KEY_EVENT)) {
			if (inRec.Event.KeyEvent.bKeyDown) {
				key = (char)inRec.Event.KeyEvent.wVirtualKeyCode;
				return key;
			}
		}
		return '\0';
	}

	/****************************************
	Function:  GetCursorPos()
	Parameter: None(void)
	Return:    COORD
	Description:
	Gets mouse input, always returns the last updated coordinates
	*****************************************/
	COORD GetCursorPos() {
		if (updateInput(MOUSE_EVENT)) {
			pos = inRec.Event.MouseEvent.dwMousePosition;
		}
		return pos;
	}

	/****************************************
	Function:  GetCursorHitPos()
	Parameter: DWORD type
	Return:    COORD
	Description:
	Gets mouse click input, type specifies left click(1)/right click(2)/double click(3), returns 0,0 if no click
	*****************************************/
	COORD GetCursorHitPos(DWORD type) {
		if (updateInput(MOUSE_EVENT, type)) {
			return inRec.Event.MouseEvent.dwMousePosition;
		}
		return { 0,0 };
	}

	/****************************************
	Function:  InitController()
	Parameter: None(void)
	Return:    None(void)
	Description:
	Initializes the controller
	*****************************************/
	void InitController() {
		hStdIn = GetStdHandle(STD_INPUT_HANDLE); // Get standard input handle
		SetConsoleMode(hStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);  // Set console to allow mouse input
	}
}