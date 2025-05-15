/****************************************
GameEngine.h
Author: Root
Date: 2019/12/09
Description:
Defines the game module header file, using GameEngine namespace to prevent naming conflicts
Function descriptions:
void InitGame()      Initialize the game
void Play()          Start the game
void DestroyGame()   Destroy the game
*****************************************/
#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "Graphic.h"
#include "Controller.h"
#include <time.h>
namespace GameEngine {
	void InitGame();
	void Play();
	void DestroyGame();

	//Global variables
	extern size_t MapWidth, MapHeight;	//Selected map width and height
	extern int MineNum;					//Selected number of mines
	extern bool PlayAgain;				//Whether to start a new game
}
#endif // GAMEENGINE_H