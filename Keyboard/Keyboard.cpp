#include "Keyboard.h"

namespace dx::engine
{
	Event::Event() noexcept
		: type{ Type::Invalid }, code{ 0 } {}

	Event::Event(Type type, ui8 code) noexcept
		: type{ type }, code{ code } {}

	bool Event::isPressed() const noexcept
	{
		return type == Type::Pressed;
	}

	bool Event::isReleased() const noexcept
	{
		return type == Type::Released;
	}

	bool Event::isValid() const noexcept
	{
		return type != Type::Invalid;
	}

	ui8 Event::GetCode() const noexcept
	{
		return code;
	}

	bool Keyboard::CharIsEmpty() noexcept
	{
		return charBuffer.empty();
	}

	void Keyboard::ClearChar() noexcept
	{
		charBuffer = std::queue<char>();
	}

	void Keyboard::Clear() noexcept
	{
		ClearChar();
		ClearKey();
	}

	void Keyboard::EnableAutorepeat() noexcept
	{
		autorepeatEnabled = true;
	}

	void Keyboard::DisableAutorepeat() noexcept
	{
		autorepeatEnabled = false;
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

	void Keyboard::ClearKey() noexcept
	{
		keyBuffer = std::queue<Event>();
	}

	char Keyboard::ReadChar() noexcept
	{
		if (charBuffer.size() > 0u)
		{
			ui8 code = charBuffer.front();
			charBuffer.pop();
			return code;
		}
		else
		{
			return 0;
		}
	}

	bool Keyboard::KeyIsEmpty() const noexcept
	{
		return keyBuffer.empty();
	}

	bool Keyboard::AutorepeatIsEnabled() const noexcept
	{
		return autorepeatEnabled;
	}

	void Keyboard::onKeyPressed(ui8 keycode) noexcept
	{
		keyStates[keycode] = true;
		keyBuffer.push(Event(Event::Type::Pressed, keycode));
		TrimBuffer(keyBuffer);
	}

	void Keyboard::onKeyReleased(ui8 keycode) noexcept
	{
		keyStates[keycode] = false;
		keyBuffer.push(Event(Event::Type::Released, keycode));
		TrimBuffer(keyBuffer);
	}

	void Keyboard::onChar(char ch) noexcept
	{
		charBuffer.push(ch);
		TrimBuffer(charBuffer);
	}
	void Keyboard::ClearState() noexcept
	{
		keyStates.reset();
	}
}
