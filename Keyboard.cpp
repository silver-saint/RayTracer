#include "Keyboard.h"

namespace dx::engine
{
	Event::Event() noexcept
		: type{ Type::Invalid }, code{ 0 } {}

	Event::Event(Type type, ui8 code) noexcept
		: type{ type }, code{ code } {}

	bool Keyboard::CharIsEmpty() noexcept
	{
		return charBuffer.empty();
	}

	Event Keyboard::ReadKey() noexcept
	{
		if (keyBuffer.size() > 0)
		{
			Event e = keyBuffer.front();
			keyBuffer.pop();
			return e;
		}
		else
		{
			return Event();
		}
	}

	void Keyboard::onKeyPressed(ui8 keycode) noexcept
	{
	}

	void Keyboard::onKeyReleased(ui8 keycode) noexcept
	{
	}

	void Keyboard::onChar(char ch) noexcept
	{
	}
}
