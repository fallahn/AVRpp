/*********************************************************************
Matt Marchant 2013 - 2014
http://trederia.blogspot.com

The zlib license has been used to make this software fully compatible
with SFML. See http://www.sfml-dev.org/license.php

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

#include <IO.hpp>

namespace IO
{
	namespace DDR
	{
		//sets a DDR bit on requested port / pin
		void Set(const uint8_t pin, volatile uint8_t* port, State state)
		{
			//TODO check validity of pin / port vals
			volatile uint8_t *ddrPort = port - 1;
			if(state == High)
			{
				*ddrPort |= (1 << pin);
			}
			else
			{
				*ddrPort &= ~(1 << pin);
			}
		}
	}

	namespace Pin
	{
		//returns whether requested pin / port is high or low
		State Set(const uint8_t pin, volatile uint8_t* port)
		{
			//TODO check validity of values
			return (*port & (1 << pin)) ? High : Low;
		}
	}

	namespace Port
	{
		//sets output pin high or low on requested port
		void Set(const uint8_t pin, volatile uint8_t* port, State state)
		{
			//TODO check validity of pin / port vals
			if(state == High)
			{
				*port |= (1 << pin);
			}
			else
			{
				*port &= ~(1 << pin);
			}
		}
	}
}

namespace Bit
{
	bool IsSet(const uint8_t bit, uint8_t byte)
	{
		return (byte & (1 << bit));
	}

	void Set(const uint8_t bit, uint8_t& byte)
	{
		byte |= (1 << bit);
	}

	void Unset(const uint8_t bit, uint8_t& byte)
	{
		byte &= ~(1 << bit);
	}
}