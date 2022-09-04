/****************************************
GameEngine.h
Author: Root
Date: 2019/12/09
Description:
�����˳�����Ϸģ���ͷ�ļ�����GameEngine�����ռ��ֹͬ����Ⱦ
������˵����
void InitGame()	   ��ʼ����Ϸ
void Play()		   ��ʼ��Ϸ
void DestroyGame() ������Ϸ
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

	//ȫ�ֱ���
	extern size_t MapWidth, MapHeight;	//ѡ���ͼ�Ŀ�Ⱥ͸߶�
	extern int MineNum;					//ѡ��ĵ�����
	extern bool PlayAgain;				//�Ƿ����¿�ʼ������һ��
}
#endif // GAMEENGINE_H