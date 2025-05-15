/****************************************
HexagonGameEngine.h
Author: Root
Date: 2019/12/26
Description:
Defines the hexagonal game module header file, using HexagonGameEngine namespace to prevent naming conflicts
Function descriptions:
void InitGame()      Initialize the game
void Play()          Start the game
void DestroyGame()   Destroy the game
*****************************************/
#pragma once
#ifndef HexagonGAMEENGINE_H
#define HexagonGAMEENGINE_H
#include "Graphic.h"
#include "Controller.h"
#include <time.h>
namespace HexagonGameEngine {
	void InitGame();
	void Play();
	void DestroyGame();

	//Global variables
	extern size_t MapWidth, MapHeight;	//Selected map width and height
	extern int MineNum;					//Selected number of mines
	extern bool PlayAgain;				//Whether to start a new game
}
#endif // GAMEENGINE_H