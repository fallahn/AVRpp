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

//registers used are ATMega8 and compatible///

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Serial.hpp> 

#ifndef F_CPU
#define F_CPU 7372800
#endif //F_CPU

namespace
{
	const int baudRate = 19200;
	const int baudPrescale = (((F_CPU / (baudRate * 16UL))) - 1);
}

void Serial::Init(bool useInterrupt)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	
	UBRRH = (baudPrescale >> 8);
	UBRRL = baudPrescale;
	
	if(useInterrupt)
	{
		UCSRB |= (1 << RXCIE);
		sei();
	}
}

void Serial::Print(const char* s)
{
	int i = 0;
	while(s[i] != '\0')
		PutChar(s[i++]);
}

void Serial::Flush()
{
	while(UCSRA & (1 << RXC))
		uint8_t c = UDR;
}


//private
void Serial::PutChar(char c)
{
	if(c == '\n') PutChar('\r');
	while(!(UCSRA & (1 << UDRE)));
	UDR = c;
}

char Serial::GetChar()
{
	while(!(UCSRA & (1 << RXC)));
	return UDR;
}

//ISR for receiving data
ISR(USART_RXC_vect)
{
	//echoes received byte. UDR MUST be read in order
	//to clear interrupt flag, else no more interrupts
	//will be received.
	char rx = UDR;
	Serial::PutChar(&rx);
}