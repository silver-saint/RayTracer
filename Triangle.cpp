#include "Triangle.h"

void engine::Triangle::run()
{
	while (Window.IsOpen())
	{
		glfwPollEvents();
	}
}
