/****************************************
HexagonGameEngine.cpp
Author: Root
Date: 2019/12/26
Description:
定义了六边形模式程序游戏模块，用HexagonGameEngine命名空间防止同名污染，是你主要需要修改的文件
*****************************************/
#include "HexagonGameEngine.h"

namespace HexagonGameEngine {
	// 引入两个命名空间
	using namespace Graphic;
	using namespace Controller;

	size_t mapWidth = 0, mapHeight = 0;				 // 地图宽度和高度
	UCHAR* mapCanvas = nullptr;						 // 地图数组指针，若做基础项请不要直接尝试操作这个指针
	COORD pos = { 0 }, posOld = { 0 };				 // 鼠标的位置
	COORD posChoice = { 0 }, posChoiceOld = { 0 };	 // 选中地图坐标的位置
	char key = '\0';								 // 键盘的按键
	bool gameFlag = false;							 // 游戏运行状态
	size_t frame = 0;								 // 已渲染帧数
	clock_t tic = clock();							 // 游戏开始时刻
	int operation = 0;								 // 执行的操作
	bool isFirst = true;							 // 是否为第一步

	// 如果你有新加的变量，建议加在下方和预置变量做区别
	UCHAR* mapMine = nullptr;						 // 地雷数组指针
	int MineNum;									 // 地雷数量(全局变量)
	int mineNum;									 // 地雷数量
	size_t MapWidth, MapHeight;				         // 地图宽度和高度(全局变量)
	size_t mineFlag;									 // 检测玩家标记的地雷数量
	int numChecked;									 // 翻开的区域大小
	int numClick;									 // 点开的次数	
	int score;										 // 得分
	clock_t startTime;								 // 开始时间
	bool gameOver = 0;								 // 游戏失败标志
	bool gameWin = 0;								 // 游戏胜利标志
	bool PlayAgain;									 // 重新开始或再来一局标志
	bool start = false;								 // 开始游戏标志
	char str[50] = "";								 // 用于显示字符串
	int biasX = 5;									 // UI文字相对于地图位置的偏移量

	//用于画地图，每个数字对应两行字符串
	const char zero1[10] = "    "; const char zero2[10] = "    ";
	const char one1[10] = " Ⅰ "; const char one2[10] = " ① ";
	const char two1[10] = " Ⅱ "; const char two2[10] = " ② ";
	const char three1[10] = " Ⅲ "; const char three2[10] = " ③ ";
	const char four1[10] = " Ⅳ "; const char four2[10] = " ④ ";
	const char five1[10] = " Ⅴ "; const char five2[10] = " ⑤ ";
	const char six1[10] = " Ⅵ "; const char six2[10] = " ⑥ ";
	const char seven1[10] = "┏-┓ "; const char seven2[10] = "┗-┛ ";//表示未翻开
	const char eight1[10] = "★★"; const char eight2[10] = "★★";	   //表示地雷
	const char nine1[10] = "小心"; const char nine2[10] = "有雷";	   //表示标记

	/****************************************
	Function:  renderMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	绘制当前地图
	*****************************************/
	void renderMap() {
		int foreColorR = 255, foreColorG = 255, foreColorB = 255;	//字体颜色
		int backColorR = 0, backColorG = 0, backColorB = 0;			//背景颜色

		//绘制地图
		for (size_t j = 0; j < mapWidth + 1; j++) {
			for (size_t i = 0; i < 2 * mapHeight + (j == 0 || j == mapWidth ? 0 : 1); i++) {
				MovePos((SHORT)j * 5 + 2, (SHORT)i + 2);
				if (j % 2 == 0) {
					if (i % 2 == 0) {
						PutStringWithColor("/", 255, 255, 255, 0, 0, 0);
					}
					else {
						PutStringWithColor("\\", 255, 255, 255, 0, 0, 0);
					}
				}
				else {
					if (i % 2 == 0) {
						PutStringWithColor("\\", 255, 255, 255, 0, 0, 0);
					}
					else {
						PutStringWithColor("/", 255, 255, 255, 0, 0, 0);
					}
				}

			}
			//边角的修缮
			if (j == mapWidth && mapWidth % 2 == 0) {
				MovePos((SHORT)j * 5 + 2, 2);
				PutStringWithColor(" ", 255, 255, 255, 0, 0, 0);
				MovePos((SHORT)j * 5 + 2, ((SHORT)mapHeight) * 2 + 2);
				PutStringWithColor("/", 255, 255, 255, 0, 0, 0);
			}
			Update(); // 将地图更新到屏幕
		}

		//绘制地雷地图
		for (size_t y = 0; y < mapHeight; y++) {
			for (size_t x = 0; x < mapWidth; x++) {
				const size_t mapIndex = y * mapWidth + x;		// 确定地图数组的下标
				char numMap1[10] = "";							// 确定数字字符串
				char numMap2[10] = "";							// 确定数字字符串

				//设置当前要打印的字符串及其颜色
				switch (mapCanvas[mapIndex]) {
				case 0:
					strcpy_s(numMap1, zero1);
					strcpy_s(numMap2, zero2);
					foreColorR = 0;
					foreColorG = 128;
					foreColorB = 0;
					break;
				case 1:
					strcpy_s(numMap1, one1);
					strcpy_s(numMap2, one2);
					foreColorR = 0;
					foreColorG = 255;
					foreColorB = 0;
					break;
				case 2:
					strcpy_s(numMap1, two1);
					strcpy_s(numMap2, two2);
					foreColorR = 123;
					foreColorG = 104;
					foreColorB = 238;
					break;
				case 3:
					strcpy_s(numMap1, three1);
					strcpy_s(numMap2, three2);
					foreColorR = 255;
					foreColorG = 0;
					foreColorB = 255;
					break;
				case 4:
					strcpy_s(numMap1, four1);
					strcpy_s(numMap2, four2);
					foreColorR = 0;
					foreColorG = 255;
					foreColorB = 255;
					break;
				case 5:
					strcpy_s(numMap1, five1);
					strcpy_s(numMap2, five2);
					foreColorR = 148;
					foreColorG = 0;
					foreColorB = 211;

					break;
				case 6:
					strcpy_s(numMap1, six1);
					strcpy_s(numMap2, six2);
					foreColorR = 255;
					foreColorG = 255;
					foreColorB = 0;
					break;
				case 7:	//未翻开
					strcpy_s(numMap1, seven1);
					strcpy_s(numMap2, seven2);
					foreColorR = 216;
					foreColorG = 191;
					foreColorB = 216;
					break;
				case 8:	//地雷
					strcpy_s(numMap1, eight1);
					strcpy_s(numMap2, eight2);
					foreColorR = 178;
					foreColorG = 34;
					foreColorB = 34;
					break;
				case 9:	//旗帜
					strcpy_s(numMap1, nine1);
					strcpy_s(numMap2, nine2);
					foreColorR = 255;
					foreColorG = 215;
					foreColorB = 0;
					break;
				}

				int k = 2;
				if (x % 2 != 0) {
					k = 3;
				}//偶数列和奇数列坐标不同

				//绘制字符串
				Update(); // 将地图更新到屏幕
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_GRID_HORIZONTAL);	//设置上划线
				MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k);
				PutStringWithColor(numMap1, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
				Update(); // 将地图更新到屏幕
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE);	//设置下划线
				MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k + 1);
				PutStringWithColor(numMap2, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
				Update(); // 将地图更新到屏幕
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// 恢复系统默认	
			}
		}
		Update(); // 将地图更新到屏幕
	}

	/****************************************
	Function:  InitGame()
	Parameter: None(void)
	Return:    None(void)
	Description:
	初始化游戏
	*****************************************/
	void InitGame() {
		ClearScreen();								 // 清屏

		//初始化地图信息和游戏信息
		posChoiceOld = { 0 };		//防止上一次游戏对这次游戏的影响
		mapWidth = MapWidth;
		mapHeight = MapHeight;
		mineFlag = mineNum = MineNum;
		numClick = numChecked = score = 0;
		gameOver = gameWin = 0;
		PlayAgain = 0;
		start = false;

		// 初始化画板数组
		mapCanvas = new UCHAR[mapWidth * mapHeight];
		UCHAR* p = mapCanvas; //改一维数组指针操作
		for (p = mapCanvas; p < mapCanvas + mapWidth * mapHeight; p++) {
			*p = 7;		//7表示未翻开
		}
		renderMap();	// 绘制地图
	}

	/****************************************
	Function:  renderChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	绘制选中的地块的背景
	*****************************************/
	void renderChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);        // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // 确定地图下标
		int foreColorR = 255, foreColorG = 255, foreColorB = 255; //字体颜色	
		int backColorR = 0, backColorG = 0, backColorB = 0;		  //背景颜色
		char numMap1[10] = "";							// 确定数字字符串
		char numMap2[10] = "";							// 确定数字字符串

		//设置当前要打印的字符串及其颜色
		switch (mapCanvas[mapIndex]) {
		case 0:
			strcpy_s(numMap1, zero1);
			strcpy_s(numMap2, zero2);
			foreColorR = 0;
			foreColorG = 128;
			foreColorB = 0;
			break;
		case 1:
			strcpy_s(numMap1, one1);
			strcpy_s(numMap2, one2);
			foreColorR = 0;
			foreColorG = 255;
			foreColorB = 0;
			break;
		case 2:
			strcpy_s(numMap1, two1);
			strcpy_s(numMap2, two2);
			foreColorR = 123;
			foreColorG = 104;
			foreColorB = 238;
			break;
		case 3:
			strcpy_s(numMap1, three1);
			strcpy_s(numMap2, three2);
			foreColorR = 255;
			foreColorG = 0;
			foreColorB = 255;
			break;
		case 4:
			strcpy_s(numMap1, four1);
			strcpy_s(numMap2, four2);
			foreColorR = 0;
			foreColorG = 255;
			foreColorB = 255;
			break;
		case 5:
			strcpy_s(numMap1, five1);
			strcpy_s(numMap2, five2);
			foreColorR = 148;
			foreColorG = 0;
			foreColorB = 211;
			break;
		case 6:
			strcpy_s(numMap1, six1);
			strcpy_s(numMap2, six2);
			foreColorR = 255;
			foreColorG = 255;
			foreColorB = 0;
			break;
		case 7:	//未翻开
			strcpy_s(numMap1, seven1);
			strcpy_s(numMap2, seven2);
			foreColorR = 218;
			foreColorG = 112;
			foreColorB = 214;
			break;
		case 8:	//地雷
			strcpy_s(numMap1, eight1);
			strcpy_s(numMap2, eight2);
			foreColorR = 178;
			foreColorG = 34;
			foreColorB = 34;
			break;
		case 9:	//旗帜
			strcpy_s(numMap1, nine1);
			strcpy_s(numMap2, nine2);
			foreColorR = 255;
			foreColorG = 215;
			foreColorB = 0;
			break;
		}

		int k = 2;
		if (x % 2 != 0) {
			k = 3;
		}//偶数列和奇数列坐标不同

		//绘制字符串
		Update(); // 将地图更新到屏幕
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_GRID_HORIZONTAL);	//设置上划线
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k);
		PutStringWithColor(numMap1, foreColorR, foreColorG, foreColorB, alpha, alpha, alpha);
		Update(); // 将地图更新到屏幕
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE);	//设置下划线
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k + 1);
		PutStringWithColor(numMap2, foreColorR, foreColorG, foreColorB, alpha, alpha, alpha);
		Update(); // 将地图更新到屏幕
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// 恢复系统默认	
	}

	/****************************************
	Function:  clearChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	清除choice中指示位置的背景，功能同上，不详做介绍
	*****************************************/
	void clearChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // 确定地图下标
		int foreColorR = 255, foreColorG = 255, foreColorB = 255; //字体颜色	
		int backColorR = 0, backColorG = 0, backColorB = 0;		  //背景颜色
		char numMap1[10] = "";							// 确定数字字符串
		char numMap2[10] = "";							// 确定数字字符串

		//设置当前要打印的字符串及其颜色
		switch (mapCanvas[mapIndex]) {
		case 0:
			strcpy_s(numMap1, zero1);
			strcpy_s(numMap2, zero2);
			foreColorR = 0;
			foreColorG = 128;
			foreColorB = 0;
			break;
		case 1:
			strcpy_s(numMap1, one1);
			strcpy_s(numMap2, one2);
			foreColorR = 0;
			foreColorG = 255;
			foreColorB = 0;
			break;
		case 2:
			strcpy_s(numMap1, two1);
			strcpy_s(numMap2, two2);
			foreColorR = 123;
			foreColorG = 104;
			foreColorB = 238;
			break;
		case 3:
			strcpy_s(numMap1, three1);
			strcpy_s(numMap2, three2);
			foreColorR = 255;
			foreColorG = 0;
			foreColorB = 255;
			break;
		case 4:
			strcpy_s(numMap1, four1);
			strcpy_s(numMap2, four2);
			foreColorR = 0;
			foreColorG = 255;
			foreColorB = 255;
			break;
		case 5:
			strcpy_s(numMap1, five1);
			strcpy_s(numMap2, five2);
			foreColorR = 148;
			foreColorG = 0;
			foreColorB = 211;
			break;
		case 6:
			strcpy_s(numMap1, six1);
			strcpy_s(numMap2, six2);
			foreColorR = 255;
			foreColorG = 255;
			foreColorB = 0;
			break;
		case 7:	//未翻开
			strcpy_s(numMap1, seven1);
			strcpy_s(numMap2, seven2);
			foreColorR = 216;
			foreColorG = 191;
			foreColorB = 216;
			break;
		case 8:	//地雷
			strcpy_s(numMap1, eight1);
			strcpy_s(numMap2, eight2);
			foreColorR = 178;
			foreColorG = 34;
			foreColorB = 34;
			break;
		case 9:	//旗帜
			strcpy_s(numMap1, nine1);
			strcpy_s(numMap2, nine2);
			foreColorR = 255;
			foreColorG = 215;
			foreColorB = 0;
			break;
		}

		int k = 2;
		if (x % 2 != 0) {
			k = 3;
		}//偶数列和奇数列坐标不同

		//绘制字符串
		Update(); // 将地图更新到屏幕
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_GRID_HORIZONTAL);	//设置上划线
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k);
		PutStringWithColor(numMap1, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
		Update(); // 将地图更新到屏幕
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE);	//设置下划线
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k + 1);
		PutStringWithColor(numMap2, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
		Update(); // 将地图更新到屏幕
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// 恢复系统默认	
	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查输入和操作，并设置相关变量
	*****************************************/
	void checkChoice() {
		FlushInput();							// 刷新输入缓冲区
		pos = GetCursorPos();					// 获取鼠标坐标
		COORD hitLeftPos = GetCursorHitPos();	// 获取左键单击坐标
		COORD hitRightPos = GetCursorHitPos(2);	// 获取右键单击坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);
		key = GetKeyHit();						// 获取键盘输入
		operation = 0;							// 当前操作 (0无操作，1为挖开地块，2为标值旗帜，或者自己指定)

		// 检查键盘输入
		switch (key) {
		case VK_ESCAPE:
			// ESC键，将游戏运行状态置否以退出游戏
			gameFlag = false;
			break;

			// 上下左右键移动选中的坐标
		case VK_UP:
			if (posChoice.Y > 0)posChoice.Y--;
			break;
		case VK_RIGHT:
			if (posChoice.X < (SHORT)mapWidth - 1)posChoice.X++;
			break;
		case VK_DOWN:
			if (posChoice.Y < (SHORT)mapHeight - 1)posChoice.Y++;
			break;
		case VK_LEFT:
			if (posChoice.X > 0)posChoice.X--;
			break;

			// 回车和空格分别对应操作1和2
		case VK_RETURN:
			operation = 1;
			break;
		case VK_SPACE:
			operation = 2;
			break;
		}

		// 将鼠标控制台的坐标放缩至地图坐标
		for (size_t i = 0; i < mapHeight; i++) {
			for (size_t j = 0; j < mapWidth; j++) {
				if ((size_t)pos.X >= 5 * j + 3 && (size_t)pos.X <= 5 * j + 7 && (size_t)pos.Y >= 2 * i + 2 + (j % 2 == 0 ? 0 : 1)
					&& (size_t)pos.Y <= 2 * i + 3 + (j % 2 == 0 ? 0 : 1)) {
					posChoice.X = (SHORT)j;
					posChoice.Y = (SHORT)i;
					break;
				}
			}
		}

		//左击"返回"
		if (!(hitLeftPos.Y != 1 || hitLeftPos.X < (MapWidth / 2 - 1) * 5 - 10 || hitLeftPos.X >(MapWidth / 2 - 1) * 5 - 2)) {
			gameFlag = false;
		}
		//左击"重新开始或再来一局"
		else if (!(hitLeftPos.Y != 1 || hitLeftPos.X < (MapWidth / 2 - 1) * 5 + 16 || hitLeftPos.X >(MapWidth / 2 - 1) * 5 + 24)) {
			operation = 3;
		}
		//左击方块
		else if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y && (size_t)pos.X >= 3 && (size_t)pos.X <= 5 * (mapWidth - 1) + 7 && (size_t)pos.Y >= 2
			&& (size_t)pos.Y <= 2 * (mapHeight - 1) + 4) {
			operation = 1;
		}
		//右击方块
		else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y && (size_t)pos.X >= 3 && (size_t)pos.X <= 5 * (mapWidth - 1) + 7 && (size_t)pos.Y >= 2
			&& (size_t)pos.Y <= 2 * (mapHeight - 1) + 4) {
			operation = 2;
		}
	}

	/****************************************
	Function:  initMap(COORD choice)
	Parameter: COORD choice，走下第一步的位置
	Return:    None(void)
	Description:
	初始化地图和地雷位置，保证第一步踩不到地雷
	*****************************************/
	void initMap(COORD choice) {
		startTime = clock();							 // 记录游戏开始时刻
		const SHORT posX = choice.X, posY = choice.Y;

		// 初始化地雷数组
		mapMine = new UCHAR[mapWidth * mapHeight]; 
		UCHAR* p = mapMine; //改一维数组指针操作
		for (p = mapMine; p < mapMine + mapWidth * mapHeight; p++) {
			*p = 0;
		}//0表示没有地雷

		//在适当的随机位置生成地雷
		srand((unsigned)time(NULL));	//用于产生随机数
		int x, y;
		int n = 0;
		while (n < mineNum) {
			x = rand() % mapWidth;
			y = rand() % mapHeight;
			if (0 == *(mapMine + y * mapWidth + x) && !(x == posX && y == posY) && !(abs(x - posX) <= 1 && abs(y - posY) <= 1)) {
				*(mapMine + y * mapWidth + x) = 1;
				n++;
			}
		}
	}

	/****************************************
	Function:  countMine(SHORT x, SHORT y)
	Parameter: x(SHORT), 横坐标
			   y(SHORT), 纵坐标
	Return:    int , 6邻域内地雷数量
	Description:
	计算六邻域内地雷总数
	*****************************************/
	int countMine(SHORT x, SHORT y) {
		int num = 0;
		size_t xNeibor[6] = { (size_t)0, (size_t)-1, (size_t)1, (size_t)-1, (size_t)0, (size_t)1 };	//用于计算六邻域坐标
		size_t yNeibor[6] = { (size_t)-1, (size_t)0, (size_t)0, (size_t)1, (size_t)1, (size_t)1 };	//用于计算六邻域坐标
		if (x % 2 == 0) {
			yNeibor[3] = yNeibor[5] = -1;
		}//偶数列和奇数列六邻域的坐标不同

		//计数
		for (int i = 0; i < 6; i++) {
			if (x + xNeibor[i] >= 0 && x + xNeibor[i] < mapWidth && y + yNeibor[i] >= 0 && y + yNeibor[i] < mapHeight) {
				num += mapMine[(y + yNeibor[i]) * mapWidth + x + xNeibor[i]];
			}
		}
		numChecked++;	//已翻开的区域增加
		return num;
	}

	/****************************************
	Function:  mineCheck(SHORT x, SHORT y)
	Parameter: x(SHORT), 横坐标
			   y(SHORT), 纵坐标
	Return:    None(void)
	Description:
	搜索相邻格，翻开连续的空白，直到遇到数字的边界
	*****************************************/
	void mineCheck(SHORT x, SHORT y) {
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // 确定地图坐标在数组中的下标

		if (7 == mapCanvas[y * mapWidth + x]) {	 //如果没有翻开
			mapCanvas[mapIndex] = countMine(x, y);//更新地图数组
			if (0 == mapCanvas[mapIndex]) {	//如果周围没有地雷，则向六邻域延申
				size_t xNeibor[6] = { (size_t)0, (size_t)-1, (size_t)1, (size_t)-1, (size_t)0, (size_t)1 };	//用于计算六邻域坐标
				size_t yNeibor[6] = { (size_t)-1, (size_t)0, (size_t)0, (size_t)1, (size_t)1, (size_t)1 };	//用于计算六邻域坐标
				if (x % 2 == 0) {
					yNeibor[3] = yNeibor[5] = -1;
				}//偶数列和奇数列六邻域的坐标不同
				
				 //向周围扩展
				for (int i = 0; i < 6; i++) {
					if (x + xNeibor[i] >= 0 && x + xNeibor[i] < mapWidth && y + yNeibor[i] >= 0 && y + yNeibor[i] < mapHeight) {
						mineCheck(SHORT(x + xNeibor[i]), SHORT(y + yNeibor[i]));
					}
				}
			}
		}
	}

	/****************************************
	Function:  digBlock(COORD choice)
	Parameter: COORD choice ， 左击的位置
	Return:    None(void)
	Description:
	翻开地块，并翻开连续的空白，，直到数字的边界
	*****************************************/
	void digBlock(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		COORD loc;

		if (isFirst) {
			// 如果是第一步走，则先初始化地图，注意不要在落点设置一个地雷
			initMap(choice);
			isFirst = false; // 将第一步设置为否
		}

		if (7 == mapCanvas[y * mapWidth + x]) {//没踩过
			numClick++;	//点击次数增加
			if (1 == mapMine[y * mapWidth + x]) {//踩到雷
				gameOver = 1;	//标志游戏失败
				for (int i = 0; i < mapWidth; i++) {
					for (int j = 0; j < mapHeight; j++) {
						//失败则显示出所有地雷
						if (mapMine[j * mapWidth + i] == 1) {
							mapCanvas[j * mapWidth + i] = 8;
						}
						else {
							mapCanvas[j * mapWidth + i] = countMine(i, j);
						}
					}
				}
			}
			else mineCheck(x, y);	//没有踩到雷则搜索相邻格

			//如果没有翻开的地块等于地雷数，则游戏胜利
			if (numChecked == mapHeight * mapWidth - mineNum) {
				gameWin = 1;
			}

			//更新地图
			for (int i = 0; i < mapWidth; i++) {
				for (int j = 0; j < mapHeight; j++) {
					loc.X = i; loc.Y = j;
					clearChoiceBackground(loc);
					Update();
				}
			}

			//计算得分
			score += (int)(mapHeight * mapWidth - mineNum - numClick) * numChecked / 10;
		}
	}

	/****************************************
	Function:  flagBlock()
	Parameter: COORD choice ， 右击的位置
	Return:    None(void)
	Description:
	将右击的地块标记为地雷
	*****************************************/
	void flagBlock(COORD choice) {
		//计算地图坐标
		const SHORT x = choice.X, y = choice.Y;
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x;
		MovePos(2 + x * 4, y * 2 + 3);

		//修改地块
		switch (mapCanvas[mapIndex]) {
		case 7:
			if (mineFlag > 0) { //剩余地雷不为0才能继续增加标记
				mapCanvas[mapIndex] = 9;	//修改标记后地块的样式
				mineFlag--;
			}
			break;
		case 9:
			mapCanvas[mapIndex] = 7;	//修改取消标记后地块的样式
			mineFlag++;
			break;
		}
	}

	/****************************************
	Function:  uiDesign()
	Parameter: None(void)
	Return:    None(void)
	Description:
	游戏UI设计
	*****************************************/
	void uiDesign() {
		pos = GetCursorPos();
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);

		//返回，重新开始，游戏胜利，游戏失败的设计
		MovePos((SHORT)(MapWidth / 2 - 1) * 5 - 10, (SHORT)1);
		if (gameOver == 1)PutStringWithColor("返    回  失 (ノへ~、）败 再来一局", 255, 255, 255, 0, 0, 0);
		else if (gameWin == 1)PutStringWithColor("返    回  胜 (*^▽^*) 利  再来一局", 255, 255, 255, 0, 0, 0);
		else if (start == false) {
			PutStringWithColor("返    回   ( ⊙ｏ⊙ )     点击开始", 255, 215, 100, 0, 0, 0);
			MovePos((SHORT)MapWidth * 5 + (SHORT)biasX, 5);
			sprintf_s(str, "游戏时间 %d s         ", 0);
			PutString(str);
		}
		else {
			PutStringWithColor("返    回   ( ⊙ｏ⊙ )     重新开始", 255, 255, 255, 0, 0, 0);
			//显示游戏时间
			MovePos((SHORT)MapWidth * 5 + biasX, (SHORT)4);
			sprintf_s(str, "                    ");
			PutString(str);
			MovePos((SHORT)MapWidth * 5 + biasX, (SHORT)5);
			if (isFirst)sprintf_s(str, "游戏时间 %d s         ", 0); else sprintf_s(str, "游戏时间 %d s", (clock() - startTime) / 1000 + 1);
			PutStringWithColor(str, 100 + (clock() - startTime) / 1000 % 10 * 15, 100 + (clock() - startTime) / 1000 % 10 * 15, 100 + (clock() - startTime) / 1000 % 10 * 15, 0, 0, 0);
		}
		if (!(pos.Y != 1 || pos.X < (MapWidth / 2 - 1) * 5 - 10 || pos.X >(MapWidth / 2 - 1) * 5 - 2)) {
			for (size_t x = (MapWidth / 2 - 1) * 5 - 10; x < (MapWidth / 2 - 1) * 5 - 2; x++) {
				MovePos((SHORT)x, 1); // 将坐标移到目标点
				ModColor(1, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
			}
		}
		//渲染背景
		if (!(pos.Y != 1 || pos.X < (MapWidth / 2 - 1) * 5 + 16 || pos.X >(MapWidth / 2 - 1) * 5 + 24)) {
			for (size_t x = (MapWidth / 2 - 1) * 5 + 16; x < (MapWidth / 2 - 1) * 5 + 24; x++) {
				MovePos((SHORT)x, 1); // 将坐标移到目标点
				ModColor(1, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
			}
		}

		//游戏信息的设计
		MovePos((SHORT)MapWidth * 5 + biasX, 3);
		sprintf_s(str, "当前选择(%u, %u)        ", posChoice.X, posChoice.Y);
		PutStringWithColor(str, 120 + posChoice.X * 3, 120 + posChoice.Y * 3, 230, 0, 0, 0);
		//剩余雷数
		MovePos((SHORT)MapWidth * 5 + biasX, 7);
		sprintf_s(str, "剩余雷数 %zd         ", mineFlag);
		PutStringWithColor(str, 220, 20 + (mineFlag * 2) % 50, 60 + (mineFlag * 2) % 50, 0, 0, 0);
		//已走步数
		MovePos((SHORT)MapWidth * 5 + biasX, 9);
		sprintf_s(str, "已走步数 %d           ", numClick);
		PutStringWithColor(str, 255 - numClick % 50, 200 + numClick % 50, 0, 0, 0, 0);
		//扫清区域
		MovePos((SHORT)MapWidth * 5 + biasX, 11);
		sprintf_s(str, "扫清区域 %d        ", numChecked);
		PutStringWithColor(str, 127 + numChecked % 50, 255 - numChecked % 50, 0, 0, 0, 0);
		//得分
		MovePos((SHORT)MapWidth * 5 + biasX, 13);
		sprintf_s(str, "得    分 %d       ", score);
		PutStringWithColor(str, 205, 92 + score % 50, 92 + score % 50, 0, 0, 0);
	}

	/****************************************
	Function:  Play()
	Parameter: None(void)
	Return:    None(void)
	Description:
	开始游戏
	*****************************************/
	void Play() {
		gameFlag = true;
		isFirst = true;

		//等待点击开始
		while (false == start && gameFlag) {
			checkChoice(); // 检查输入,更新operation
			switch (operation) {
			case 3:
				start = true;
			}
			uiDesign();
			Update();
		}

		while (gameFlag) {
			checkChoice(); // 检查输入,更新operation
			// 查看当前坐标是否需要更新背景
			//posChoice为当前鼠标坐标映射到地图中的坐标
			if (posChoice.X != posChoiceOld.X || posChoice.Y != posChoiceOld.Y) {
				clearChoiceBackground(posChoiceOld);
				posChoiceOld = posChoice;
			}
			renderChoiceBackground(posChoice); ///闪烁

			//当没有胜利或失败
			if (0 == gameOver && 0 == gameWin) {
				uiDesign();
				//执行相应操作
				switch (operation) {
				case 1:
					// 翻开地块
					digBlock(posChoice);
					MovePos((SHORT)(MapWidth / 2 - 1) * 5 - 10, 1);
					PutStringWithColor("返    回   (づ⊙ｏ⊙)づ   重新开始", 255, 255, 255, 0, 0, 0);
					Update();
					Sleep(300);
					break;
				case 2:
					// 标记为地雷
					flagBlock(posChoice);
					break;
				case 3:
					//重新开始
					gameFlag = 0;
					PlayAgain = 1;
				}
			}
			//游戏失败
			else if (1 == gameOver) {
				uiDesign();
				switch (operation) {
				case 3:
					//再来一局
					gameFlag = 0;
					PlayAgain = 1;
				}
			}
			//游戏胜利
			else {
				uiDesign();
				switch (operation) {
				case 3:
					//再来一局
					gameFlag = 0;
					PlayAgain = 1;
				}
			}

			Update();    // 更新操作到屏幕
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);	    // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();						    // 更新上一次记录的时间
		}
	}

	/****************************************
	Function:  DestroyGame()
	Parameter: None(void)
	Return:    None(void)
	Description:
	结束游戏，回收动态申请的变量
	*****************************************/
	void DestroyGame() {
		delete[] mapCanvas;
		//delete[] mapMine;
	}


}