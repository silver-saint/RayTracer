#pragma once
#include <queue>
#include <bitset>
#include "../types.h"

namespace dx::engine
{
	class Event
	{
	public:
		enum class Type
		{
			Invalid,
			Pressed,
			Released
		};
		Event() noexcept;
		Event(Type type, ui8 code) noexcept;
		bool isPressed() const noexcept;
		bool isReleased() const noexcept;
		bool isValid() const noexcept;
		ui8 GetCode() const noexcept;
	private:
		Type type;
		ui8 code;
	};
	class Keyboard
	{
		friend class Window;
		friend class Event;

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
	public:
		bool KeyIsPressed(ui8 code) const noexcept { return keyStates[code]; }
		Event ReadKey() noexcept;
		bool KeyIsEmpty() const noexcept;
		void ClearKey() noexcept;
		char ReadChar() noexcept;
		bool CharIsEmpty() noexcept;
		void ClearChar() noexcept;
		void Clear() noexcept;
		void EnableAutorepeat() noexcept;
		void DisableAutorepeat() noexcept;
		bool AutorepeatIsEnabled() const noexcept;
	private:
		void onKeyPressed(ui8 keycode) noexcept;
		void onKeyReleased(ui8 keycode) noexcept;
		void onChar(char ch) noexcept;
		void ClearState() noexcept;
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer) noexcept;
	private:
		static constexpr ui32 sKeys = 256u;
		static constexpr ui32 sBufferSize = 16u;
		std::bitset<sKeys> keyStates;
		std::queue<Event> keyBuffer;
		std::queue<char> charBuffer;
		bool autoRepeatEnabled = { false };
	};

	template<typename T>
	inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > sBufferSize)
		{
			buffer.pop();
		}
	}

}