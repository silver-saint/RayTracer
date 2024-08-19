#pragma once
#include <iostream>
#include <print>
class Menu
{
public:
	Menu() = default;
	int GetInput() const { return input; }
	void run();
	void verifyInput();
	void PrintMessage();
private:
	int input = 0;
};

