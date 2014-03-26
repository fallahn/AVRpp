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

#ifndef CLOCK_H_
#define CLOCK_H_

#ifndef F_CPU
#define F_CPU 4000000
#endif //F_CPU
#include <util/delay.h>

namespace avr
{
class Clock final
{

public:
	static void Init(){Clock clock;}
	static void Tick(){m_count++;}

	class Timer final
	{
	public:
		Timer(): m_timer (&m_count){Restart();};
		//returns time elapsed since last restart in seconds
		float Elapsed();
		//returns time since last restart in seconds and resets timer
		float Restart();
	private:
		unsigned long m_startTime;
		volatile const unsigned long* m_timer;
	};

private:
	Clock();
	volatile static unsigned long m_count;
};
}


#endif /* CLOCK_H_ */