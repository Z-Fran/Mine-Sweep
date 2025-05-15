/****************************************
GameMenu.cpp
Author: Root
Date: 2019/12/09
Description:
Defines the program's menu rendering/management module, uses the GameMenu namespace to prevent naming conflicts.
*****************************************/
#include "GameMenu.h"

namespace GameMenu {
	// 引入两个命名空间
	using namespace Graphic;
	using namespace Controller;

	size_t frame = 0;                  // 定义已渲染的帧数 
	COORD pos = { 0 }, hitPos = { 0 }; // 定义鼠标当前的坐标和左键点击的坐标
	int choice = 0, oldChoice = 0;     // 定义选中的菜单项
	bool isChosen = false;             // 定义是否做出选择
	char key = '\0';                   // 定义当前键盘的按键
	clock_t tic = clock();             // 定义程序上一帧渲染的时刻

	// 如果你有新加的变量，建议加在下方和预置变量做区别
	char str[50];					   // 用于打印字符串
	bool fourOrSix = 1;				   //1为选择四边形模式，0为选择六边形模式

	/****************************************
	Function:  renderMenu()
	Parameter: mark(int),	选择显示哪层菜单
	Return:    None(void)
	Description:
	清屏并且打印菜单(主菜单和各子菜单)
	*****************************************/
	void renderMenu(int mark) {
		ClearScreen();  // 清屏
		switch (mark) {
		case 1://主菜单
			MovePos(12, 2); // 移动坐标到 14,2
			PutString("扫 雷 ☆ 大 作 战"); // 在这个坐标放置一个string
			MovePos(15, 4);
			PutString("开 始 战 斗");
			MovePos(15, 6);
			PutString("查 看 说 明");
			MovePos(15, 8);
			PutString("退 出 游 戏");
			break;

		case 2://选择难度菜单
			//模式选择部分
			MovePos(10, 2);
			PutString("* 选  择  模  式 *");
			MovePos(4, 3);
			PutString("------------------------------");
			if (1 == fourOrSix) {
				MovePos(4, 4);
				PutStringWithColor("* 经典四边形 *", 178, 34, 34, 0, 0, 0); //4-18, 20-34
				PutStringWithColor("  * 扭曲六边形 *", 255, 255, 255, 0, 0, 0);
			}//四边形模式显示红色
			else {
				MovePos(4, 4);
				PutStringWithColor("* 经典四边形 *", 255, 255, 255, 0, 0, 0); //4-18, 20-34
				PutStringWithColor("  * 扭曲六边形 *", 178, 34, 34, 0, 0, 0);
			}//六边形模式显示红色
			MovePos(4, 5);
			PutString("------------------------------");

			//难度选择部分
			MovePos(10, 6);
			PutString("* 选  择  难  度 *");
			MovePos(15, 8);
			PutString(" 初  级");
			MovePos(15, 10);
			PutString(" 中  级");
			MovePos(15, 12);
			PutString(" 高  级");
			MovePos(15, 14);
			PutString("自 定 义");
			MovePos(15, 16);
			PutString(" 返  回");
			break;

		case 3://自定义菜单
			//自定义地图大小
			MovePos(13, 2);
			PutString("*设 置 地 图 大 小*");
			MovePos(11, 4);
			PutString("----------------------");
			MovePos(17, 5);
			PutString("▲     ▲");		//调整地图按钮
			MovePos(11, 6);
			sprintf_s(str, "Width %2d  X  %2d Height", (int)GameEngine::MapWidth, (int)GameEngine::MapHeight);
			PutString(str);
			MovePos(17, 7);
			PutString("▼     ▼");		//调整地图按钮
			MovePos(11, 8);
			PutString("----------------------");

			//自定义地雷数
			MovePos(14, 10);
			sprintf_s(str, "地雷数 %4d ▲▼", (int)GameEngine::MineNum);		//调整地雷数按钮
			PutString(str);
			MovePos(11, 12);
			PutString("----------------------");
			MovePos(16, 14);
			PutString(" 开  始");
			MovePos(16, 16);
			PutString(" 返  回");
			break;

		case 4: //使用说明菜单
			//待显示的文字
			char* str[100] = { (char*)"  ",(char*)"  ",(char*)"危",(char*)"急",(char*)"！",(char*)"陈", (char*)"博",(char*)"士", (char*)"被",(char*)"邪",
				(char*)"恶",(char*)"大", (char*)"魔",(char*)"王", (char*)"锁", (char*)"在", (char*)"一",(char*)"个", (char*)"布",
				(char*)"满", (char*)"雷",(char*)"区", (char*)"的",(char*)"地", (char*)"带",(char*)"，", (char*)"你", (char*)"将",
				(char*)"被", (char*)"投",(char*)"放", (char*)"在",(char*)"这", (char*)"个",(char*)"区", (char*)"域",(char*)"中",
				(char*)"，",(char*)"需", (char*)"要",(char*)"你", (char*)"一", (char*)"步", (char*)"步",(char*)"扫",(char*)"清",
				(char*)"障", (char*)"碍",(char*)"，", (char*)"标",(char*)"记", (char*)"出", (char*)"所",(char*)"有",(char*)"的",
				(char*)"地", (char*)"雷",(char*)"，", (char*)"救",(char*)"出",(char*)"陈", (char*)"博",(char*)"士", (char*)"。" };
			SHORT x = 5, y = 2;

			MovePos(15, 2);
			PutString("游 戏 说 明");

			//逐字输出
			for (int i = 0; i < 64; i++) {
				if (i % 16 == 0) { //每行输出16个字符的长度
					x = 5;
					y += 2;
				}
				MovePos(x, y);
				PutStringWithColor(str[i], 255, 255, 255, 0, 0, 0);
				Update();
				Sleep(150); //等待
				x += 2;
			}
			MovePos(8, 12);
			PutString("★ 切记！这是你要找的地雷！");
			MovePos(16, 14);
			PutString(" 返  回");
			break;
		}
		Update();
	}

	/****************************************
	Function:  InitMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	初始化菜单，同时初始化图形框架和控制器框架
	*****************************************/
	void InitMenu() {
		InitGraphic(44, 25); // 初始化新的窗口大小为44,25
		InitController();    // 初始化控制器
		renderMenu(1);        // 绘制菜单（1代表绘制主菜单）
	}

	/****************************************
	Function:  randerChoiceBackground(int choice)
	Parameter: int choice
	Return:    None(void)
	Description:
	将当前选中的菜单项(int choice)背景渲染一个渐变消失的白色
	*****************************************/
	void randerChoiceBackground(int choice) {
		size_t y = 0;  // 定义当前选中的菜单项的实际y坐标
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // 定义背景白色当前的颜色值，通过已渲染的帧数进行运算（此处也可以改成使用时间）
		switch (choice) {
		case 1:
			y = 4;
			break;
		case 2:
			y = 6;
			break;
		case 3:
			y = 8;
			break;
		case 4:
			y = 10;
			break;
		case 5:
			y = 12;
			break;
		case 6:
			y = 14;
			break;
		case 7:
			y = 16;
			break;
		case 8:
			y = 18;
			break;
		case 101:
			y = 5;
			break;
		case 102:
			y = 5;
			break;
		case 103:
			y = 7;
			break;
		case 104:
			y = 7;
			break;
		case 105:
			y = 10;
			break;
		case 106:
			y = 10;
			break;
		case 201:
			y = 4;
			break;
		case 202:
			y = 4;
			break;
		}

		//choice<100表示一般菜单项
		if (choice < 100) {
			for (size_t x = 10; x < 30; x++) {
				MovePos((SHORT)x, (SHORT)y); // 将坐标移到目标点
				ModColor(1, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
			}
		}
		//100<choice<200表示自定义地图按钮
		else if (choice < 200) {
			size_t x = 0;
			switch (choice) {
			case 101:
				x = 17;
				break;
			case 102:
				x = 24;
				break;
			case 103:
				x = 17;
				break;
			case 104:
				x = 24;
				break;
			case 105:
				x = 26;
				break;
			case 106:
				x = 28;
				break;
			}
			for (size_t i = x; i < x + 2; i++) {
				MovePos((SHORT)i, (SHORT)y); // 将坐标移到目标点
				ModColor(1, 255, 255, 255, alpha, alpha, alpha); // 修改背景色
			}
		}
		//choice>200选择四边形和六边形选项
		else {
			size_t x = 0;
			switch (choice) {
			case 201:
				x = 4;
				break;
			case 202:
				x = 20;
				break;
			}
			for (size_t i = x; i < x + 14; i++) {
				MovePos((SHORT)i, (SHORT)y); // 将坐标移到目标点
				ModColor(1, 178, 34, 34, alpha, alpha, alpha); // 修改背景色
			}
		}
		Update();  // 更新界面
	}

	/****************************************
	Function:  clearChoiceBackground(int choice)
	Parameter: int choice
	Return:    None(void)
	Description:
	将上次选中的菜单项(int choice)背景恢复为黑色，内容大致和上一个函数一致，不多做阐述
	*****************************************/
	void clearChoiceBackground(int choice) {
		size_t y = 0;
		switch (choice) {
		case 1:
			y = 4;
			break;
		case 2:
			y = 6;
			break;
		case 3:
			y = 8;
			break;
		case 4:
			y = 10;
			break;
		case 5:
			y = 12;
			break;
		case 6:
			y = 14;
			break;
		case 7:
			y = 16;
			break;
		case 101:
			y = 5;
			break;
		case 102:
			y = 5;
			break;
		case 103:
			y = 7;
			break;
		case 104:
			y = 7;
			break;
		case 105:
			y = 10;
			break;
		case 106:
			y = 10;
			break;
		case 201:
			y = 4;
			break;
		case 202:
			y = 4;
			break;
		}
		//choice<100表示一般菜单项
		if (choice < 100) {
			for (size_t x = 10; x < 30; x++) {
				MovePos((SHORT)x, (SHORT)y);
				ModColor(1, 255, 255, 255, 0, 0, 0);
			}
		}
		//100<choice<200表示自定义地图按钮
		else if (choice < 200) {
			size_t x = 0;
			switch (choice) {
			case 101:
				x = 17;
				break;
			case 102:
				x = 24;
				break;
			case 103:
				x = 17;
				break;
			case 104:
				x = 24;
				break;
			case 105:
				x = 26;
				break;
			case 106:
				x = 28;
				break;
			}
			for (size_t i = x; i < x + 2; i++) {
				MovePos((SHORT)i, (SHORT)y); // 将坐标移到目标点
				ModColor(1, 255, 255, 255, 0, 0, 0); // 修改背景色
			}
		}
		//choice>200选择四边形和六边形选项
		else {
			size_t x = 0;
			switch (choice) {
			case 201:
				x = 4;
				break;
			case 202:
				x = 20;
				break;
			}
			for (size_t i = x; i < x + 14; i++) {
				MovePos((SHORT)i, (SHORT)y); // 将坐标移到目标点
				ModColor(1, 255, 255, 255, 0, 0, 0); // 修改背景色
			}
		}

	}

	/****************************************
	Function:  checkChoiceInIntro()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查使用说明菜单中键盘/鼠标输入，并更新选中的菜单项
	*****************************************/
	void checkChoiceInIntro() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		isChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 14 || pos.X < 10 || pos.X > 30)) {
			choice = 6;
		}
		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 14 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
	}

	/****************************************
	Function:  renderIntro()
	Parameter: None(void)
	Return:    None(void)
	Description:
	绘制游戏说明
	*****************************************/
	void renderIntro() {
		BYTE alpha = 255 - (BYTE)((frame % 50) * 5);
		renderMenu(4);	//渲染选择难度菜单
		bool runFlag = true;
		choice = oldChoice = 6;	//初始化选择项

		while (runFlag) {
			//渲染提示的字体颜色
			alpha = 255 - (BYTE)((frame % 50) * 5);
			for (size_t x = 8; x < 36; x++) {
				MovePos((SHORT)x, 12); // 将坐标移到目标点
				ModColor(2, 150 + alpha, 100 + alpha, alpha, 0, 0, 0); // 修改背景色
			}

			checkChoiceInIntro();  // 检查输入
			if (choice != oldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearChoiceBackground(oldChoice);
				oldChoice = choice;
			}
			randerChoiceBackground(choice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (isChosen) {
				switch (choice) {
				case 6:
					runFlag = false;
				}

			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();
			Update();
		}
	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查主菜单中键盘/鼠标输入，并更新选中的菜单项
	*****************************************/
	void checkChoice() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		isChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			choice = 1;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			choice = 2;
		}
		else if (!(pos.Y != 8 || pos.X < 10 || pos.X > 30)) {
			choice = 3;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_UP:
			// 上方向键
			if (choice > 1)choice--;
			break;
		case VK_DOWN:
			// 下方向键
			if (choice < 3)choice++;
			break;
		case VK_RETURN:
			// 回车，更新选中
			isChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 8 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
	}

	/****************************************
	Function:  checkChoiceInSelection()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查难度选择菜单中键盘/鼠标输入，并更新选中的菜单项
	*****************************************/
	void checkChoiceInSelection() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		isChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 8 || pos.X < 10 || pos.X > 30)) {
			choice = 3;
		}
		else if (!(pos.Y != 10 || pos.X < 10 || pos.X > 30)) {
			choice = 4;
		}
		else if (!(pos.Y != 12 || pos.X < 10 || pos.X > 30)) {
			choice = 5;
		}
		else if (!(pos.Y != 14 || pos.X < 10 || pos.X > 30)) {
			choice = 6;
		}
		else if (!(pos.Y != 16 || pos.X < 10 || pos.X > 30)) {
			choice = 7;
		}
		else if (!(pos.Y != 4 || pos.X < 4 || pos.X > 18)) {
			choice = 201;
		}
		else if (!(pos.Y != 4 || pos.X < 20 || pos.X > 34)) {
			choice = 202;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_UP:
			// 上方向键
			if (choice > 3)choice--;
			break;
		case VK_DOWN:
			// 下方向键
			if (choice < 7)choice++;
			break;
		case VK_RETURN:
			// 回车，更新选中
			isChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 8 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 10 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 12 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 14 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 16 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 4 || hitPos.X < 4 || hitPos.X > 18)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 4 || hitPos.X < 20 || hitPos.X > 34)) {
			isChosen = true;
		}
	}

	/****************************************
	Function:  checkChoiceInUserDefineMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	检查自定义地图菜单中键盘/鼠标输入，并更新选中的菜单项
	*****************************************/
	void checkChoiceInUserDefineMenu() {
		FlushInput();           // 刷新输入缓冲区
		pos = GetCursorPos();   // 获取鼠标输入
		key = GetKeyHit();		// 获取键盘输入
		hitPos = GetCursorHitPos();  // 获取鼠标单击输入
		isChosen = false;		// 重置选中状态

		// 检查鼠标位置，更新选中项
		if (!(pos.Y != 14 || pos.X < 10 || pos.X > 30)) {
			choice = 6;
		}
		else if (!(pos.Y != 16 || pos.X < 10 || pos.X > 30)) {
			choice = 7;
		}
		else if (!(pos.Y != 5 || pos.X < 17 || pos.X > 19)) {
			choice = 101;
		}
		else if (!(pos.Y != 5 || pos.X < 24 || pos.X > 26)) {
			choice = 102;
		}
		else if (!(pos.Y != 7 || pos.X < 17 || pos.X > 19)) {
			choice = 103;
		}
		else if (!(pos.Y != 7 || pos.X < 24 || pos.X > 26)) {
			choice = 104;
		}
		else if (!(pos.Y != 10 || pos.X < 26 || pos.X > 27)) {
			choice = 105;
		}
		else if (!(pos.Y != 10 || pos.X < 28 || pos.X > 29)) {
			choice = 106;
		}

		// 检查键盘按键，更新选中项
		switch (key) {
		case VK_UP:
			// 上方向键
			if (choice > 6)choice--;
			break;
		case VK_DOWN:
			// 下方向键
			if (choice < 7)choice++;
			break;
		case VK_RETURN:
			// 回车，更新选中
			isChosen = true;
			break;
		}

		// 检查鼠标点击的位置是否在菜单项中，如果是，更新选中
		if (!(hitPos.Y != 14 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 16 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 5 || hitPos.X < 17 || hitPos.X > 19)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 5 || hitPos.X < 24 || hitPos.X > 26)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 7 || hitPos.X < 17 || hitPos.X > 19)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 7 || hitPos.X < 24 || hitPos.X > 26)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 10 || hitPos.X < 26 || hitPos.X > 27)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 10 || hitPos.X < 28 || hitPos.X > 29)) {
			isChosen = true;
		}
	}

	/****************************************
	Function:  clearUserDefineMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	将自定义菜单中旧选择的背景清空
	*****************************************/
	void clearUserDefineMenu() {
		MovePos(17, 5);
		PutString("▲     ▲");		//调整地图按钮
		MovePos(11, 6);
		sprintf_s(str, "Width %2d  X  %2d Height", (int)GameEngine::MapWidth, (int)GameEngine::MapHeight);
		PutString(str);
		MovePos(17, 7);
		PutString("▼     ▼");		//调整地图按钮
		MovePos(14, 10);
		sprintf_s(str, "地雷数 %4d ▲▼", (int)GameEngine::MineNum);		//调整地雷数按钮
		PutString(str);
		MovePos(10, 14);
		PutString("       开  始          ");
		MovePos(10, 16);
		PutString("       返  回          ");
		Update();
	}

	/****************************************
	Function:  userDefineMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	自定义地图菜单主循环，若不退出将一直渲染下去
	*****************************************/
	void userDefineMenu() {
		//初始地图信息
		GameEngine::MapWidth = 8;
		GameEngine::MapHeight = 5;
		GameEngine::MineNum = 1;
		bool runFlag = true;
		choice = oldChoice = 6;	//初始化选择项
		renderMenu(3);	//渲染自定义地图菜单

		while (runFlag) {
			checkChoiceInUserDefineMenu();  // 检查输入
			if (choice != oldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearUserDefineMenu();
				oldChoice = choice;
			}
			randerChoiceBackground(choice); // 绘制选中项的背景


			// 如果选中菜单，执行操作
			if (isChosen) {
				switch (choice) {
				//开始游戏
				case 6:
					GameEngine::PlayAgain = HexagonGameEngine::PlayAgain = 1;	//是否重新开始或再来一局
					if (1 == fourOrSix) {
						while (GameEngine::PlayAgain) {	//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic((int)GameEngine::MapWidth * 4 + 2 + 30, (int)GameEngine::MapHeight * 2 + 5); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							GameEngine::InitGame();
							GameEngine::Play();
							GameEngine::DestroyGame();
						}
					}
					else {
						HexagonGameEngine::MapWidth = GameEngine::MapWidth;
						HexagonGameEngine::MapHeight = GameEngine::MapHeight;
						HexagonGameEngine::MineNum = GameEngine::MineNum;
						while (HexagonGameEngine::PlayAgain) {	//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic((int)GameEngine::MapWidth * 5 + 2 + 30, (int)GameEngine::MapHeight * 2 + 5); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							HexagonGameEngine::InitGame();
							HexagonGameEngine::Play();
							HexagonGameEngine::DestroyGame();
						}
					}

					InitGraphic(44, 25); // 初始化菜单的窗口大小
					InitController();    // 初始化控制器
					renderMenu(3);
					break;

				// 返回
				case 7:
					runFlag = false;// 将Flag置否以退出循环
					break;

				//增加地图宽度
				case 101:
					if (GameEngine::MapWidth < 40) GameEngine::MapWidth++;	//最大宽度40
					else  GameEngine::MapWidth = 8;
					MovePos(11, 6);
					sprintf_s(str, "Width %2d  X  %2d Height", (int)GameEngine::MapWidth, (int)GameEngine::MapHeight);
					PutString(str);
					break;
				//增加地图高度
				case 102:
					if (GameEngine::MapHeight < 20) GameEngine::MapHeight++;	//最大高度20
					else  GameEngine::MapHeight = 5;
					MovePos(11, 6);
					sprintf_s(str, "Width %2d  X  %2d Height", (int)GameEngine::MapWidth, (int)GameEngine::MapHeight);
					PutString(str);
					break;
				//减小地图宽度
				case 103:
					if (GameEngine::MapWidth > 8) GameEngine::MapWidth--;	//最小宽度8
					else  GameEngine::MapWidth = 40;
					MovePos(11, 6);
					sprintf_s(str, "Width %2d  X  %2d Height", (int)GameEngine::MapWidth, (int)GameEngine::MapHeight);
					PutString(str);
					break;
				//减小地图高度
				case 104:
					if (GameEngine::MapHeight > 5) GameEngine::MapHeight--;	//最小高度5
					else  GameEngine::MapHeight = 20;
					MovePos(11, 6);
					sprintf_s(str, "Width %2d  X  %2d Height", (int)GameEngine::MapWidth, (int)GameEngine::MapHeight);
					PutString(str);
					break;

				//增加地雷数
				case 105:
					if (GameEngine::MineNum < GameEngine::MapHeight * GameEngine::MapWidth - 9) GameEngine::MineNum++;
					else GameEngine::MineNum = int(GameEngine::MapHeight * GameEngine::MapWidth - 9);//最大地雷数
					MovePos(14, 10);
					sprintf_s(str, "地雷数 %4d ▲▼", GameEngine::MineNum);
					PutString(str);
					break;
				//减小地雷数
				case 106:
					if (GameEngine::MineNum > 1) GameEngine::MineNum--;
					else GameEngine::MineNum = int(GameEngine::MapHeight * GameEngine::MapWidth - 9);
					MovePos(14, 10);
					sprintf_s(str, "地雷数 %4d ▲▼", GameEngine::MineNum);
					PutString(str);
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}

	/****************************************
	Function:  levelChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	难度选择菜单主循环，若不退出将一直渲染下去
	*****************************************/
	void levelChoice() {
		renderMenu(2);	//渲染选择难度菜单
		bool runFlag = true;
		choice = oldChoice = 5;	//初始化选择项

		while (runFlag) {
			checkChoiceInSelection();  // 检查输入
			if (choice != oldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearChoiceBackground(oldChoice);
				oldChoice = choice;
			}
			randerChoiceBackground(choice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (isChosen) {
				switch (choice) {
					// 初级
				case 3:
					//初始化地图信息
					GameEngine::MapWidth = HexagonGameEngine::MapWidth = 10;
					GameEngine::MapHeight = HexagonGameEngine::MapHeight = 10;
					GameEngine::MineNum = HexagonGameEngine::MineNum = 10;
					GameEngine::PlayAgain = HexagonGameEngine::PlayAgain = 1;	//是否重新开始或再来一局

					//进入四边形模式游戏
					if (1 == fourOrSix) {
						while (GameEngine::PlayAgain) {//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic((int)GameEngine::MapWidth * 4 + 2 + 30, (int)GameEngine::MapHeight * 2 + 5); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							GameEngine::InitGame();
							GameEngine::Play();
							GameEngine::DestroyGame();
						}
					}
					//进入六边形模式游戏
					else {
						while (HexagonGameEngine::PlayAgain) {	//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic((int)GameEngine::MapWidth * 5 + 2 + 30, (int)GameEngine::MapHeight * 2 + 5); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							HexagonGameEngine::InitGame();
							HexagonGameEngine::Play();
							HexagonGameEngine::DestroyGame();
						}
					}
					InitGraphic(44, 25); // 初始化菜单的窗口大小
					InitController();    // 初始化控制器
					renderMenu(2);
					break;

					// 中级
				case 4:
					//初始化地图信息
					GameEngine::MapWidth = HexagonGameEngine::MapWidth = 16;
					GameEngine::MapHeight = HexagonGameEngine::MapHeight = 16;
					GameEngine::MineNum = HexagonGameEngine::MineNum = 30;
					GameEngine::PlayAgain = HexagonGameEngine::PlayAgain = 1;	//是否重新开始或再来一局

					//进入四边形模式游戏
					if (1 == fourOrSix) {
						while (GameEngine::PlayAgain) {//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic(int(GameEngine::MapWidth * 4 + 2 + 30), int(GameEngine::MapHeight * 2 + 5)); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							GameEngine::InitGame();
							GameEngine::Play();
							GameEngine::DestroyGame();
						}
					}
					else {
						while (HexagonGameEngine::PlayAgain) {	//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic(int(HexagonGameEngine::MapWidth * 5 + 2 + 30), int(HexagonGameEngine::MapHeight * 2 + 5)); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							HexagonGameEngine::InitGame();
							HexagonGameEngine::Play();
							HexagonGameEngine::DestroyGame();
						}
					}
					InitGraphic(44, 25); // 初始化菜单的窗口大小
					InitController();    // 初始化控制器
					renderMenu(2);
					break;

					// 高级
				case 5:
					//初始化地图信息
					GameEngine::MapWidth = HexagonGameEngine::MapWidth = 30;
					GameEngine::MapHeight = HexagonGameEngine::MapHeight = 16;
					GameEngine::MineNum = HexagonGameEngine::MineNum = 99;
					GameEngine::PlayAgain = HexagonGameEngine::PlayAgain = 1;

					//进入四边形模式游戏
					if (1 == fourOrSix) {
						while (GameEngine::PlayAgain) {//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic((int)GameEngine::MapWidth * 4 + 2 + 30, (int)GameEngine::MapHeight * 2 + 5); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							GameEngine::InitGame();
							GameEngine::Play();
							GameEngine::DestroyGame();
						}
					}
					//进入六边形模式游戏
					else {
						while (HexagonGameEngine::PlayAgain) {	//如果选择重新开始或再来一局，则循环继续执行
							InitGraphic((int)GameEngine::MapWidth * 5 + 2 + 30, (int)GameEngine::MapHeight * 2 + 5); // 初始化新的窗口大小
							InitController();    // 初始化控制器
							HexagonGameEngine::InitGame();
							HexagonGameEngine::Play();
							HexagonGameEngine::DestroyGame();
						}
					}
					InitGraphic(44, 25); // 初始化菜单的窗口大小
					InitController();    // 初始化控制器
					renderMenu(2);
					break;

					// 自定义
				case 6:
					userDefineMenu();	 //进入自定义地图菜单
					InitGraphic(44, 25); // 初始化菜单的窗口大小
					InitController();    // 初始化控制器
					renderMenu(2);
					break;

					//返回
				case 7:
					runFlag = false;	// 将Flag置否以退出循环
					break;

					//选择经典四边形模式
				case 201:
					fourOrSix = 1;
					//重新渲染文字
					MovePos(4, 4);
					PutStringWithColor("* 经典四边形 *", 178, 34, 34, 0, 0, 0); //4-18, 20-34
					PutStringWithColor("  * 扭曲六边形 *", 255, 255, 255, 0, 0, 0);
					break;
					//选择扭曲六边形模式
				case 202:
					fourOrSix = 0;
					//重新渲染文字
					MovePos(4, 4);
					PutStringWithColor("* 经典四边形 *", 255, 255, 255, 0, 0, 0); //4-18, 20-34
					PutStringWithColor("  * 扭曲六边形 *", 178, 34, 34, 0, 0, 0);
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}

	/****************************************
	Function:  WaitChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	主循环，若不退出将一直渲染下去
	*****************************************/
	void WaitChoice() {
		bool runFlag = true;
		choice = oldChoice = 1;	//初始化选择项

		while (runFlag) {
			checkChoice();  // 检查输入
			if (choice != oldChoice) {
				// 若新的选择与旧选择不一致，将旧选择的背景清空
				clearChoiceBackground(oldChoice);
				oldChoice = choice;
			}
			randerChoiceBackground(choice); // 绘制选中项的背景

			// 如果选中菜单，执行操作
			if (isChosen) {
				switch (choice) {
				case 1:
					// 开始游戏,选择难度
					levelChoice();
					renderMenu(1);
					break;
				case 2:
					// 绘制说明
					renderIntro();
					renderMenu(1);
					break;
				case 3:
					// 将Flag置否以退出循环
					runFlag = false;
					break;
				}
			}

			// 处理每帧的事务
			frame++;  // 渲染帧数自增
			clock_t elapsed = 25 - (clock() - tic); // 检查上一帧渲染时间，并计算与25的差值
			Sleep(elapsed > 0 ? elapsed : 0);       // 若差值大于零，则休眠该差值的毫秒数，以确保每帧渲染不超过50帧
			tic = clock();							// 更新上一次记录的时间
		}
	}
}