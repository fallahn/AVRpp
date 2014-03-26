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

#include <AnalogueIn.hpp>

using namespace avr;

Adc::Reference Adc::m_aref = Reference::Default;
bool Adc::m_initialised = false;

void Adc::Init()
{
	//pre-scales @ 32 providing 125khz @ 4mhz clock
	//TODO set this correctly based on F_CPU if defined
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0);
	m_initialised = true;
}

void Adc::SetReference(Reference reference)
{
	m_aref = reference;
}

uint16_t Adc::GetValue(uint8_t pin)
{
	if(!m_initialised)
		return 0;

	ADMUX = (m_aref << 6) | (pin & 0x07);
	if(m_aref == Internal)
		ADMUX |= (1 << ADLAR);

	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	if(m_aref == Internal)
		return ADCH;

	const uint8_t low = ADCL; //must read low byte first
	const uint8_t high = ADCH;

	return (high << 8) | low;
}