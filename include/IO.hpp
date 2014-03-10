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


#ifndef IO_H_
#define IO_H_

#include <avr/io.h>

namespace IO
{
	enum State
	{
		High,
		Low
	};

	namespace DDR
	{
		//sets a DDR bit on requested port / pin
		void Set(const uint8_t pin, volatile uint8_t* port, State state);
	}

	namespace Pin
	{
		//returns whether requested pin / port is high or low
		State Set(const uint8_t pin, volatile uint8_t* port);
	}

	namespace Port
	{
		//sets output pin high or low on requested port
		void Set(const uint8_t pin, volatile uint8_t* port, State state);
	}
}

namespace Bit
{
	bool IsSet(const uint8_t bit, uint8_t byte);
	void Set(const uint8_t bit, uint8_t& byte);
	void Unset(const uint8_t bit, uint8_t& byte);
}
#endif /* IO_H_ */