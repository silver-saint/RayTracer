#pragma once
#include <iostream>
#include <print>
class Menu
{
public:
	void Init();
	int GetInput() const { return input; }
	void verifyInput();
	void PrintMessage();
private:
	int input = 0;
};

