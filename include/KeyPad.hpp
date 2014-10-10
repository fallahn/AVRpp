/*********************************************************************
Matt Marchant 2013 - 2014
http://trederia.blogspot.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint-gcc.h>

namespace avr
{

typedef uint8_t Scancode;
class KeyPad final
{
public:
	enum Key
	{
		One   = 11,
		Two   = 7,
		Three = 3,
		Four  = 10,
		Five  = 6,
		Six   = 2,
		Seven = 9,
		Eight = 5,
		Nine  = 1,
		Star  = 8,
		Zero  = 4,
		Hash  = 0
	};

	struct Event
	{
		enum class Type
		{
			KEY_PRESSED,
			KEY_RELEASED,
			NONE
		};
		//event type, scancode (matrix value), literal value as char, literal value as int
		Event()
		: type(Type::NONE), scancode(0u), valueAsChar('\0'), valueAsInt(0u){}

		Type type;
		Scancode scancode;
		char valueAsChar;
		uint8_t valueAsInt;
	};

	static void Update();
	static bool KeyPressed(Key key);
	static bool PollEvent(Event& e);

private:
	static uint16_t m_mask;
	static uint16_t m_prevMask;

	static Event m_events[12];
	static uint8_t m_queuePosition;
};

} //namespace avr

#endif /* KEYPAD_H_ */