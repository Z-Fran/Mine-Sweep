/****************************************
HexagonGameEngine.cpp
Author: Root
Date: 2019/12/26
Description:
������������ģʽ������Ϸģ�飬��HexagonGameEngine�����ռ��ֹͬ����Ⱦ��������Ҫ��Ҫ�޸ĵ��ļ�
*****************************************/
#include "HexagonGameEngine.h"

namespace HexagonGameEngine {
	// �������������ռ�
	using namespace Graphic;
	using namespace Controller;

	size_t mapWidth = 0, mapHeight = 0;				 // ��ͼ��Ⱥ͸߶�
	UCHAR* mapCanvas = nullptr;						 // ��ͼ����ָ�룬�����������벻Ҫֱ�ӳ��Բ������ָ��
	COORD pos = { 0 }, posOld = { 0 };				 // ����λ��
	COORD posChoice = { 0 }, posChoiceOld = { 0 };	 // ѡ�е�ͼ�����λ��
	char key = '\0';								 // ���̵İ���
	bool gameFlag = false;							 // ��Ϸ����״̬
	size_t frame = 0;								 // ����Ⱦ֡��
	clock_t tic = clock();							 // ��Ϸ��ʼʱ��
	int operation = 0;								 // ִ�еĲ���
	bool isFirst = true;							 // �Ƿ�Ϊ��һ��

	// ��������¼ӵı�������������·���Ԥ�ñ���������
	UCHAR* mapMine = nullptr;						 // ��������ָ��
	int MineNum;									 // ��������(ȫ�ֱ���)
	int mineNum;									 // ��������
	size_t MapWidth, MapHeight;				         // ��ͼ��Ⱥ͸߶�(ȫ�ֱ���)
	size_t mineFlag;									 // �����ұ�ǵĵ�������
	int numChecked;									 // �����������С
	int numClick;									 // �㿪�Ĵ���	
	int score;										 // �÷�
	clock_t startTime;								 // ��ʼʱ��
	bool gameOver = 0;								 // ��Ϸʧ�ܱ�־
	bool gameWin = 0;								 // ��Ϸʤ����־
	bool PlayAgain;									 // ���¿�ʼ������һ�ֱ�־
	bool start = false;								 // ��ʼ��Ϸ��־
	char str[50] = "";								 // ������ʾ�ַ���
	int biasX = 5;									 // UI��������ڵ�ͼλ�õ�ƫ����

	//���ڻ���ͼ��ÿ�����ֶ�Ӧ�����ַ���
	const char zero1[10] = "    "; const char zero2[10] = "    ";
	const char one1[10] = " �� "; const char one2[10] = " �� ";
	const char two1[10] = " �� "; const char two2[10] = " �� ";
	const char three1[10] = " �� "; const char three2[10] = " �� ";
	const char four1[10] = " �� "; const char four2[10] = " �� ";
	const char five1[10] = " �� "; const char five2[10] = " �� ";
	const char six1[10] = " �� "; const char six2[10] = " �� ";
	const char seven1[10] = "��-�� "; const char seven2[10] = "��-�� ";//��ʾδ����
	const char eight1[10] = "���"; const char eight2[10] = "���";	   //��ʾ����
	const char nine1[10] = "С��"; const char nine2[10] = "����";	   //��ʾ���

	/****************************************
	Function:  renderMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	���Ƶ�ǰ��ͼ
	*****************************************/
	void renderMap() {
		int foreColorR = 255, foreColorG = 255, foreColorB = 255;	//������ɫ
		int backColorR = 0, backColorG = 0, backColorB = 0;			//������ɫ

		//���Ƶ�ͼ
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
			//�߽ǵ�����
			if (j == mapWidth && mapWidth % 2 == 0) {
				MovePos((SHORT)j * 5 + 2, 2);
				PutStringWithColor(" ", 255, 255, 255, 0, 0, 0);
				MovePos((SHORT)j * 5 + 2, ((SHORT)mapHeight) * 2 + 2);
				PutStringWithColor("/", 255, 255, 255, 0, 0, 0);
			}
			Update(); // ����ͼ���µ���Ļ
		}

		//���Ƶ��׵�ͼ
		for (size_t y = 0; y < mapHeight; y++) {
			for (size_t x = 0; x < mapWidth; x++) {
				const size_t mapIndex = y * mapWidth + x;		// ȷ����ͼ������±�
				char numMap1[10] = "";							// ȷ�������ַ���
				char numMap2[10] = "";							// ȷ�������ַ���

				//���õ�ǰҪ��ӡ���ַ���������ɫ
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
				case 7:	//δ����
					strcpy_s(numMap1, seven1);
					strcpy_s(numMap2, seven2);
					foreColorR = 216;
					foreColorG = 191;
					foreColorB = 216;
					break;
				case 8:	//����
					strcpy_s(numMap1, eight1);
					strcpy_s(numMap2, eight2);
					foreColorR = 178;
					foreColorG = 34;
					foreColorB = 34;
					break;
				case 9:	//����
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
				}//ż���к����������겻ͬ

				//�����ַ���
				Update(); // ����ͼ���µ���Ļ
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_GRID_HORIZONTAL);	//�����ϻ���
				MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k);
				PutStringWithColor(numMap1, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
				Update(); // ����ͼ���µ���Ļ
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE);	//�����»���
				MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k + 1);
				PutStringWithColor(numMap2, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
				Update(); // ����ͼ���µ���Ļ
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// �ָ�ϵͳĬ��	
			}
		}
		Update(); // ����ͼ���µ���Ļ
	}

	/****************************************
	Function:  InitGame()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ʼ����Ϸ
	*****************************************/
	void InitGame() {
		ClearScreen();								 // ����

		//��ʼ����ͼ��Ϣ����Ϸ��Ϣ
		posChoiceOld = { 0 };		//��ֹ��һ����Ϸ�������Ϸ��Ӱ��
		mapWidth = MapWidth;
		mapHeight = MapHeight;
		mineFlag = mineNum = MineNum;
		numClick = numChecked = score = 0;
		gameOver = gameWin = 0;
		PlayAgain = 0;
		start = false;

		// ��ʼ����������
		mapCanvas = new UCHAR[mapWidth * mapHeight];
		UCHAR* p = mapCanvas; //��һά����ָ�����
		for (p = mapCanvas; p < mapCanvas + mapWidth * mapHeight; p++) {
			*p = 7;		//7��ʾδ����
		}
		renderMap();	// ���Ƶ�ͼ
	}

	/****************************************
	Function:  renderChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	����ѡ�еĵؿ�ı���
	*****************************************/
	void renderChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);        // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // ȷ����ͼ�±�
		int foreColorR = 255, foreColorG = 255, foreColorB = 255; //������ɫ	
		int backColorR = 0, backColorG = 0, backColorB = 0;		  //������ɫ
		char numMap1[10] = "";							// ȷ�������ַ���
		char numMap2[10] = "";							// ȷ�������ַ���

		//���õ�ǰҪ��ӡ���ַ���������ɫ
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
		case 7:	//δ����
			strcpy_s(numMap1, seven1);
			strcpy_s(numMap2, seven2);
			foreColorR = 218;
			foreColorG = 112;
			foreColorB = 214;
			break;
		case 8:	//����
			strcpy_s(numMap1, eight1);
			strcpy_s(numMap2, eight2);
			foreColorR = 178;
			foreColorG = 34;
			foreColorB = 34;
			break;
		case 9:	//����
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
		}//ż���к����������겻ͬ

		//�����ַ���
		Update(); // ����ͼ���µ���Ļ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_GRID_HORIZONTAL);	//�����ϻ���
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k);
		PutStringWithColor(numMap1, foreColorR, foreColorG, foreColorB, alpha, alpha, alpha);
		Update(); // ����ͼ���µ���Ļ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE);	//�����»���
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k + 1);
		PutStringWithColor(numMap2, foreColorR, foreColorG, foreColorB, alpha, alpha, alpha);
		Update(); // ����ͼ���µ���Ļ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// �ָ�ϵͳĬ��	
	}

	/****************************************
	Function:  clearChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	���choice��ָʾλ�õı���������ͬ�ϣ�����������
	*****************************************/
	void clearChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // ȷ����ͼ�±�
		int foreColorR = 255, foreColorG = 255, foreColorB = 255; //������ɫ	
		int backColorR = 0, backColorG = 0, backColorB = 0;		  //������ɫ
		char numMap1[10] = "";							// ȷ�������ַ���
		char numMap2[10] = "";							// ȷ�������ַ���

		//���õ�ǰҪ��ӡ���ַ���������ɫ
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
		case 7:	//δ����
			strcpy_s(numMap1, seven1);
			strcpy_s(numMap2, seven2);
			foreColorR = 216;
			foreColorG = 191;
			foreColorB = 216;
			break;
		case 8:	//����
			strcpy_s(numMap1, eight1);
			strcpy_s(numMap2, eight2);
			foreColorR = 178;
			foreColorG = 34;
			foreColorB = 34;
			break;
		case 9:	//����
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
		}//ż���к����������겻ͬ

		//�����ַ���
		Update(); // ����ͼ���µ���Ļ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_GRID_HORIZONTAL);	//�����ϻ���
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k);
		PutStringWithColor(numMap1, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
		Update(); // ����ͼ���µ���Ļ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE);	//�����»���
		MovePos((SHORT)x * 5 + 3, (SHORT)y * 2 + k + 1);
		PutStringWithColor(numMap2, foreColorR, foreColorG, foreColorB, backColorR, backColorG, backColorB);
		Update(); // ����ͼ���µ���Ļ
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	// �ָ�ϵͳĬ��	
	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	�������Ͳ�������������ر���
	*****************************************/
	void checkChoice() {
		FlushInput();							// ˢ�����뻺����
		pos = GetCursorPos();					// ��ȡ�������
		COORD hitLeftPos = GetCursorHitPos();	// ��ȡ�����������
		COORD hitRightPos = GetCursorHitPos(2);	// ��ȡ�Ҽ���������
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);
		key = GetKeyHit();						// ��ȡ��������
		operation = 0;							// ��ǰ���� (0�޲�����1Ϊ�ڿ��ؿ飬2Ϊ��ֵ���ģ������Լ�ָ��)

		// ����������
		switch (key) {
		case VK_ESCAPE:
			// ESC��������Ϸ����״̬�÷����˳���Ϸ
			gameFlag = false;
			break;

			// �������Ҽ��ƶ�ѡ�е�����
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

			// �س��Ϳո�ֱ��Ӧ����1��2
		case VK_RETURN:
			operation = 1;
			break;
		case VK_SPACE:
			operation = 2;
			break;
		}

		// ��������̨�������������ͼ����
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

		//���"����"
		if (!(hitLeftPos.Y != 1 || hitLeftPos.X < (MapWidth / 2 - 1) * 5 - 10 || hitLeftPos.X >(MapWidth / 2 - 1) * 5 - 2)) {
			gameFlag = false;
		}
		//���"���¿�ʼ������һ��"
		else if (!(hitLeftPos.Y != 1 || hitLeftPos.X < (MapWidth / 2 - 1) * 5 + 16 || hitLeftPos.X >(MapWidth / 2 - 1) * 5 + 24)) {
			operation = 3;
		}
		//�������
		else if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y && (size_t)pos.X >= 3 && (size_t)pos.X <= 5 * (mapWidth - 1) + 7 && (size_t)pos.Y >= 2
			&& (size_t)pos.Y <= 2 * (mapHeight - 1) + 4) {
			operation = 1;
		}
		//�һ�����
		else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y && (size_t)pos.X >= 3 && (size_t)pos.X <= 5 * (mapWidth - 1) + 7 && (size_t)pos.Y >= 2
			&& (size_t)pos.Y <= 2 * (mapHeight - 1) + 4) {
			operation = 2;
		}
	}

	/****************************************
	Function:  initMap(COORD choice)
	Parameter: COORD choice�����µ�һ����λ��
	Return:    None(void)
	Description:
	��ʼ����ͼ�͵���λ�ã���֤��һ���Ȳ�������
	*****************************************/
	void initMap(COORD choice) {
		startTime = clock();							 // ��¼��Ϸ��ʼʱ��
		const SHORT posX = choice.X, posY = choice.Y;

		// ��ʼ����������
		mapMine = new UCHAR[mapWidth * mapHeight]; 
		UCHAR* p = mapMine; //��һά����ָ�����
		for (p = mapMine; p < mapMine + mapWidth * mapHeight; p++) {
			*p = 0;
		}//0��ʾû�е���

		//���ʵ������λ�����ɵ���
		srand((unsigned)time(NULL));	//���ڲ��������
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
	Parameter: x(SHORT), ������
			   y(SHORT), ������
	Return:    int , 6�����ڵ�������
	Description:
	�����������ڵ�������
	*****************************************/
	int countMine(SHORT x, SHORT y) {
		int num = 0;
		size_t xNeibor[6] = { (size_t)0, (size_t)-1, (size_t)1, (size_t)-1, (size_t)0, (size_t)1 };	//���ڼ�������������
		size_t yNeibor[6] = { (size_t)-1, (size_t)0, (size_t)0, (size_t)1, (size_t)1, (size_t)1 };	//���ڼ�������������
		if (x % 2 == 0) {
			yNeibor[3] = yNeibor[5] = -1;
		}//ż���к�����������������겻ͬ

		//����
		for (int i = 0; i < 6; i++) {
			if (x + xNeibor[i] >= 0 && x + xNeibor[i] < mapWidth && y + yNeibor[i] >= 0 && y + yNeibor[i] < mapHeight) {
				num += mapMine[(y + yNeibor[i]) * mapWidth + x + xNeibor[i]];
			}
		}
		numChecked++;	//�ѷ�������������
		return num;
	}

	/****************************************
	Function:  mineCheck(SHORT x, SHORT y)
	Parameter: x(SHORT), ������
			   y(SHORT), ������
	Return:    None(void)
	Description:
	�������ڸ񣬷��������Ŀհף�ֱ���������ֵı߽�
	*****************************************/
	void mineCheck(SHORT x, SHORT y) {
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // ȷ����ͼ�����������е��±�

		if (7 == mapCanvas[y * mapWidth + x]) {	 //���û�з���
			mapCanvas[mapIndex] = countMine(x, y);//���µ�ͼ����
			if (0 == mapCanvas[mapIndex]) {	//�����Χû�е��ף���������������
				size_t xNeibor[6] = { (size_t)0, (size_t)-1, (size_t)1, (size_t)-1, (size_t)0, (size_t)1 };	//���ڼ�������������
				size_t yNeibor[6] = { (size_t)-1, (size_t)0, (size_t)0, (size_t)1, (size_t)1, (size_t)1 };	//���ڼ�������������
				if (x % 2 == 0) {
					yNeibor[3] = yNeibor[5] = -1;
				}//ż���к�����������������겻ͬ
				
				 //����Χ��չ
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
	Parameter: COORD choice �� �����λ��
	Return:    None(void)
	Description:
	�����ؿ飬�����������Ŀհף���ֱ�����ֵı߽�
	*****************************************/
	void digBlock(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		COORD loc;

		if (isFirst) {
			// ����ǵ�һ���ߣ����ȳ�ʼ����ͼ��ע�ⲻҪ���������һ������
			initMap(choice);
			isFirst = false; // ����һ������Ϊ��
		}

		if (7 == mapCanvas[y * mapWidth + x]) {//û�ȹ�
			numClick++;	//�����������
			if (1 == mapMine[y * mapWidth + x]) {//�ȵ���
				gameOver = 1;	//��־��Ϸʧ��
				for (int i = 0; i < mapWidth; i++) {
					for (int j = 0; j < mapHeight; j++) {
						//ʧ������ʾ�����е���
						if (mapMine[j * mapWidth + i] == 1) {
							mapCanvas[j * mapWidth + i] = 8;
						}
						else {
							mapCanvas[j * mapWidth + i] = countMine(i, j);
						}
					}
				}
			}
			else mineCheck(x, y);	//û�вȵ������������ڸ�

			//���û�з����ĵؿ���ڵ�����������Ϸʤ��
			if (numChecked == mapHeight * mapWidth - mineNum) {
				gameWin = 1;
			}

			//���µ�ͼ
			for (int i = 0; i < mapWidth; i++) {
				for (int j = 0; j < mapHeight; j++) {
					loc.X = i; loc.Y = j;
					clearChoiceBackground(loc);
					Update();
				}
			}

			//����÷�
			score += (int)(mapHeight * mapWidth - mineNum - numClick) * numChecked / 10;
		}
	}

	/****************************************
	Function:  flagBlock()
	Parameter: COORD choice �� �һ���λ��
	Return:    None(void)
	Description:
	���һ��ĵؿ���Ϊ����
	*****************************************/
	void flagBlock(COORD choice) {
		//�����ͼ����
		const SHORT x = choice.X, y = choice.Y;
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x;
		MovePos(2 + x * 4, y * 2 + 3);

		//�޸ĵؿ�
		switch (mapCanvas[mapIndex]) {
		case 7:
			if (mineFlag > 0) { //ʣ����ײ�Ϊ0���ܼ������ӱ��
				mapCanvas[mapIndex] = 9;	//�޸ı�Ǻ�ؿ����ʽ
				mineFlag--;
			}
			break;
		case 9:
			mapCanvas[mapIndex] = 7;	//�޸�ȡ����Ǻ�ؿ����ʽ
			mineFlag++;
			break;
		}
	}

	/****************************************
	Function:  uiDesign()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ϷUI���
	*****************************************/
	void uiDesign() {
		pos = GetCursorPos();
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);

		//���أ����¿�ʼ����Ϸʤ������Ϸʧ�ܵ����
		MovePos((SHORT)(MapWidth / 2 - 1) * 5 - 10, (SHORT)1);
		if (gameOver == 1)PutStringWithColor("��    ��  ʧ (�Τ�~������ ����һ��", 255, 255, 255, 0, 0, 0);
		else if (gameWin == 1)PutStringWithColor("��    ��  ʤ (*^��^*) ��  ����һ��", 255, 255, 255, 0, 0, 0);
		else if (start == false) {
			PutStringWithColor("��    ��   ( �ѣ�� )     �����ʼ", 255, 215, 100, 0, 0, 0);
			MovePos((SHORT)MapWidth * 5 + (SHORT)biasX, 5);
			sprintf_s(str, "��Ϸʱ�� %d s         ", 0);
			PutString(str);
		}
		else {
			PutStringWithColor("��    ��   ( �ѣ�� )     ���¿�ʼ", 255, 255, 255, 0, 0, 0);
			//��ʾ��Ϸʱ��
			MovePos((SHORT)MapWidth * 5 + biasX, (SHORT)4);
			sprintf_s(str, "                    ");
			PutString(str);
			MovePos((SHORT)MapWidth * 5 + biasX, (SHORT)5);
			if (isFirst)sprintf_s(str, "��Ϸʱ�� %d s         ", 0); else sprintf_s(str, "��Ϸʱ�� %d s", (clock() - startTime) / 1000 + 1);
			PutStringWithColor(str, 100 + (clock() - startTime) / 1000 % 10 * 15, 100 + (clock() - startTime) / 1000 % 10 * 15, 100 + (clock() - startTime) / 1000 % 10 * 15, 0, 0, 0);
		}
		if (!(pos.Y != 1 || pos.X < (MapWidth / 2 - 1) * 5 - 10 || pos.X >(MapWidth / 2 - 1) * 5 - 2)) {
			for (size_t x = (MapWidth / 2 - 1) * 5 - 10; x < (MapWidth / 2 - 1) * 5 - 2; x++) {
				MovePos((SHORT)x, 1); // �������Ƶ�Ŀ���
				ModColor(1, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
			}
		}
		//��Ⱦ����
		if (!(pos.Y != 1 || pos.X < (MapWidth / 2 - 1) * 5 + 16 || pos.X >(MapWidth / 2 - 1) * 5 + 24)) {
			for (size_t x = (MapWidth / 2 - 1) * 5 + 16; x < (MapWidth / 2 - 1) * 5 + 24; x++) {
				MovePos((SHORT)x, 1); // �������Ƶ�Ŀ���
				ModColor(1, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
			}
		}

		//��Ϸ��Ϣ�����
		MovePos((SHORT)MapWidth * 5 + biasX, 3);
		sprintf_s(str, "��ǰѡ��(%u, %u)        ", posChoice.X, posChoice.Y);
		PutStringWithColor(str, 120 + posChoice.X * 3, 120 + posChoice.Y * 3, 230, 0, 0, 0);
		//ʣ������
		MovePos((SHORT)MapWidth * 5 + biasX, 7);
		sprintf_s(str, "ʣ������ %zd         ", mineFlag);
		PutStringWithColor(str, 220, 20 + (mineFlag * 2) % 50, 60 + (mineFlag * 2) % 50, 0, 0, 0);
		//���߲���
		MovePos((SHORT)MapWidth * 5 + biasX, 9);
		sprintf_s(str, "���߲��� %d           ", numClick);
		PutStringWithColor(str, 255 - numClick % 50, 200 + numClick % 50, 0, 0, 0, 0);
		//ɨ������
		MovePos((SHORT)MapWidth * 5 + biasX, 11);
		sprintf_s(str, "ɨ������ %d        ", numChecked);
		PutStringWithColor(str, 127 + numChecked % 50, 255 - numChecked % 50, 0, 0, 0, 0);
		//�÷�
		MovePos((SHORT)MapWidth * 5 + biasX, 13);
		sprintf_s(str, "��    �� %d       ", score);
		PutStringWithColor(str, 205, 92 + score % 50, 92 + score % 50, 0, 0, 0);
	}

	/****************************************
	Function:  Play()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ʼ��Ϸ
	*****************************************/
	void Play() {
		gameFlag = true;
		isFirst = true;

		//�ȴ������ʼ
		while (false == start && gameFlag) {
			checkChoice(); // �������,����operation
			switch (operation) {
			case 3:
				start = true;
			}
			uiDesign();
			Update();
		}

		while (gameFlag) {
			checkChoice(); // �������,����operation
			// �鿴��ǰ�����Ƿ���Ҫ���±���
			//posChoiceΪ��ǰ�������ӳ�䵽��ͼ�е�����
			if (posChoice.X != posChoiceOld.X || posChoice.Y != posChoiceOld.Y) {
				clearChoiceBackground(posChoiceOld);
				posChoiceOld = posChoice;
			}
			renderChoiceBackground(posChoice); ///��˸

			//��û��ʤ����ʧ��
			if (0 == gameOver && 0 == gameWin) {
				uiDesign();
				//ִ����Ӧ����
				switch (operation) {
				case 1:
					// �����ؿ�
					digBlock(posChoice);
					MovePos((SHORT)(MapWidth / 2 - 1) * 5 - 10, 1);
					PutStringWithColor("��    ��   (�šѣ��)��   ���¿�ʼ", 255, 255, 255, 0, 0, 0);
					Update();
					Sleep(300);
					break;
				case 2:
					// ���Ϊ����
					flagBlock(posChoice);
					break;
				case 3:
					//���¿�ʼ
					gameFlag = 0;
					PlayAgain = 1;
				}
			}
			//��Ϸʧ��
			else if (1 == gameOver) {
				uiDesign();
				switch (operation) {
				case 3:
					//����һ��
					gameFlag = 0;
					PlayAgain = 1;
				}
			}
			//��Ϸʤ��
			else {
				uiDesign();
				switch (operation) {
				case 3:
					//����һ��
					gameFlag = 0;
					PlayAgain = 1;
				}
			}

			Update();    // ���²�������Ļ
			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);	    // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();						    // ������һ�μ�¼��ʱ��
		}
	}

	/****************************************
	Function:  DestroyGame()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������Ϸ�����ն�̬����ı���
	*****************************************/
	void DestroyGame() {
		delete[] mapCanvas;
		//delete[] mapMine;
	}


}