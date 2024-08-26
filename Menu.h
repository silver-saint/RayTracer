#pragma once
#include <iostream>
#include <print>
class Menu
{
public:
	Menu() : input {0} {}
	void Init();
	int GetInput() const { return input; }
	void verifyInput();
	void PrintMessage();
private:
	int input;
};

extern Menu menu;
