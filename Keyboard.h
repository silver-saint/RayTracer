#pragma once
#include "dx/Window.h"
#include "stl.h";
#include <bitset>
namespace dx::engine
{
	class Event
	{
		enum class Type
		{
			Invalid,
			Pressed,
			Released
		};
	public:
		Event() noexcept;
		Event(Type type, ui8 code) noexcept;
	private:
		Type type;
		ui8 code;
	};
	class Keyboard
	{
		friend Window;
		friend Event;

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		bool KeyIsPressed(ui8 code) const noexcept { return keyStates[code]; }
		bool CharIsEmpty() noexcept;
		Event ReadKey() noexcept;

	private:
		void onKeyPressed(ui8 keycode) noexcept;
		void onKeyReleased(ui8 keycode) noexcept;
		void onChar(char ch) noexcept;
		static constexpr ui32 keys = 256u;
		static constexpr ui32 buffer = 16u;
		std::bitset<keys> keyStates;
		stl::Queue<Event> keyBuffer;
		stl::Queue<char> charBuffer;
	};

}