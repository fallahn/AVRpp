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

#include <KeyPad.hpp>
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 4000000
#endif
#include <util/delay.h>

namespace
{
	//maps the scan code (ie the position in the keypad matrix) to the value of the key
	char charMap[] = {'#', '9', '6', '3', '0', '8', '5', '2', '*', '7', '4', '1'};
	uint8_t intMap[] = {'#', 9, 6, 3, 0, 8, 5, 2, '*', 7, 4, 1};

	//input port and output port connected to keypad
	volatile uint8_t* columnPort = &PIND;
	volatile uint8_t* rowPort = &PORTB;

	//number of rows / columns on keypad
	const uint8_t columnCount = 3u;
	const uint8_t rowCount = 4u;

	//number of bits from the beginning of connected port
	const uint8_t columnOffset = 5u;
	const uint8_t rowOffset = 1u;

	const uint8_t rowMask = (1 << rowOffset) | (1 << (rowOffset + 1)) | (1 << (rowOffset + 2)) | (1 << (rowOffset + 3));

	const uint8_t queueSize = 12u;
}

uint16_t KeyPad::m_mask = 0u;
uint16_t KeyPad::m_prevMask = 0u;
KeyPad::Event KeyPad::m_events[] = {Event(), Event(), Event(), Event(), Event(), Event(), Event(), Event(), Event(), Event(), Event(), Event()};
uint8_t KeyPad::m_queuePosition = 0u;

void KeyPad::Update()
{
	for(auto row = 0; row < rowCount; ++row)
	{
		//we need to preserve other outputs on the row port
		uint8_t temp = *rowPort | rowMask;
		//before pulling row bit low
		temp &= ~(1 << (row + rowOffset));
		*rowPort = temp;
		for(auto col = 0u; col < columnCount; ++col)
		{
			auto id = (1 << (row + (col * rowCount)));
			if(*columnPort & (1 << (col + columnOffset)))
			{
				m_mask &= ~id;
			}
			else
			{
				m_mask |= id;
			}
		}
		_delay_ms(45);
	}

	//update event queue
	for(auto i = 0u, queueIndex = 0u; i < queueSize; ++i)
	{
		//only raise event if mask has changed
		if((m_mask & (1 << i)) != (m_prevMask & (1 << i)))
		{
			m_events[queueIndex].type = (m_mask & (1 << i)) ? Event::Type::KEY_PRESSED : Event::Type::KEY_RELEASED;
			m_events[queueIndex].scancode = i;
			m_events[queueIndex].valueAsChar = charMap[i];
			m_events[queueIndex].valueAsInt = intMap[i];
			queueIndex++;
		}
	}
	m_prevMask = m_mask;
}

bool KeyPad::KeyPressed(Key key)
{
	return (m_mask & (1 << key));
}

bool KeyPad::PollEvent(Event& e)
{
	if(m_queuePosition == queueSize
		|| m_events[m_queuePosition].type == Event::Type::NONE)
	{
		m_queuePosition = 0u;
		return false;
	}
	e.type = m_events[m_queuePosition].type;
	e.scancode = m_events[m_queuePosition].scancode;
	e.valueAsChar = m_events[m_queuePosition].valueAsChar;
	e.valueAsInt = m_events[m_queuePosition].valueAsInt;
	m_events[m_queuePosition++].type = Event::Type::NONE;

	return true;
}