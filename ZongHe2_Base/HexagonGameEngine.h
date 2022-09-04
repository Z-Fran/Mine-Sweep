/****************************************
HexagonGameEngine.h
Author: Root
Date: 2019/12/26
Description:
�����˳�����������Ϸģ���ͷ�ļ�����HexagonGameEngine�����ռ��ֹͬ����Ⱦ
������˵����
void InitGame()	   ��ʼ����Ϸ
void Play()		   ��ʼ��Ϸ
void DestroyGame() ������Ϸ
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

	//ȫ�ֱ���
	extern size_t MapWidth, MapHeight;	//ѡ���ͼ�Ŀ�Ⱥ͸߶�
	extern int MineNum;					//ѡ��ĵ�����
	extern bool PlayAgain;				//�Ƿ����¿�ʼ������һ��
}
#endif // GAMEENGINE_H