#include "Menu.h"


void Menu::run()
{
	PrintMessage();
	verifyInput();
}

void Menu::verifyInput()
{
	int in;
	while ((std::cin >> in) && (in != 1 && in != 2))
	{
		std::println("Please enter a valid input");
	}
	input = in;
}

void Menu::PrintMessage()
{
	std::println("What would you like to render?\n 1.TriForce\n 2.Other.");
}
