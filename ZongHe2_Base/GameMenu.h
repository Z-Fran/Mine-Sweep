/****************************************
GameMenu.h
Author: Root
Date: 2019/12/09
Description:
Defines the menu rendering/selection module header file, using GameMenu namespace to prevent naming conflicts
Function descriptions:
void InitMenu()      Initialize the menu and render menu options
void WaitChoice()    Wait for user input (menu selection)
*****************************************/
#pragma once
#ifndef GAMEMENU_H
#define GAMEMENU_H
#include "Graphic.h"
#include "Controller.h"
#include "GameEngine.h"
#include "HexagonGameEngine.h"
#include <time.h>

namespace GameMenu {
	void InitMenu();
	void WaitChoice();
}
#endif // GAMEMENU_H