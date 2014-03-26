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

#include <avr/interrupt.h>

#include <Serial.hpp> 

namespace
{
	const int baudRate = 19200;
	const int baudPrescale = (((F_CPU / (baudRate * 16UL))) - 1);
	
	FILE uartStream;
}

using namespace avr;
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
	
#ifdef HOOK_IO
	uartStream.put = PutChar;	
	uartStream.get = GetChar;
	uartStream.flags = _FDEV_SETUP_RW;
	
	stdout = stdin = &uartStream;
#endif //HOOK_IO
}

uint8_t Serial::Receive()
{
	while(!(UCSRA & (1 << RXC)));
	uint8_t b = UDR; //we must at least read it to clear the buffer
	return b;
}

void Serial::Flush()
{
	while(UCSRA & (1 << RXC))
		uint8_t c = UDR;
}

//private
int Serial::PutChar(char c, FILE* stream)
{
	if(c == '\n') PutChar('\r', stream);
	while(!(UCSRA & (1 << UDRE)));
	UDR = c;
	return 0;
}

int Serial::GetChar(FILE* stream)
{
	while(!(UCSRA & (1 << RXC)));
	uint8_t c = UDR;
	
	//PutChar(c, stream);
	return c;
}

//ISR for receiving data
ISR(USART_RXC_vect)
{
	//UDR MUST be read in order
	//to clear interrupt flag, else no more interrupts
	//will be received.
	char rx = UDR;
	//TODO anything you like with rx
}