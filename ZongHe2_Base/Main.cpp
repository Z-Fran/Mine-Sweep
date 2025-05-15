/****************************************
Main.cpp
Author: Root
Date: 2019/12/09
Description:
Main program entry point, includes Menu header file, initializes menu and enters wait-for-choice loop
*****************************************/
#include "GameMenu.h"

int main()
{
	GameMenu::InitMenu();
	GameMenu::WaitChoice();
}