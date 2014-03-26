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

#include "../include/Clock.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

using namespace avr;

//ctor
Clock::Clock()
{
	//these settings assume ATMega8515. Adjust to specific hardware as necessary
	//set up timer 1 to timeout at 1 millisecond
	TIMSK = (1 << OCIE1A);
	OCR1A = 500;
	TCCR1B = (1 << WGM12) | (1 << CS11); //1/8 PS gives us milliseconds
	//enable interrupts
	sei();
}

//public


//private
volatile unsigned long Clock::m_count = 0u;

//----timer class-----//
float Clock::Timer::Elapsed()
{
	return static_cast<float>(*m_timer - m_startTime) / 1000.f;
}

float Clock::Timer::Restart()
{
	float time = Elapsed();
	m_startTime = *m_timer;
	return time;
}

//ISR for clock overflow
ISR(TIMER1_COMPA_vect)
{
	Clock::Tick();
}