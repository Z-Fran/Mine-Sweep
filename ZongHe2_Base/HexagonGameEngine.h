/****************************************
HexagonGameEngine.h
Author: Root
Date: 2019/12/26
Description:
定义了程序六边形游戏模块的头文件，用HexagonGameEngine命名空间防止同名污染
各函数说明：
void InitGame()	   初始化游戏
void Play()		   开始游戏
void DestroyGame() 结束游戏
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

	//全局变量
	extern size_t MapWidth, MapHeight;	//选择地图的宽度和高度
	extern int MineNum;					//选择的地雷数
	extern bool PlayAgain;				//是否重新开始或再来一局
}
#endif // GAMEENGINE_H