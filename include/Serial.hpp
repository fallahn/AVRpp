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

/********************************************************************
RS232 utility class.
--------------------

Static functions provide ability to stream arrays of data out of the uart
and parse received bytes. Bytes can be received via a leaner blocking
function or via non-blocking interrupt. This class also hooks stdio so
that using printf() in any program which includes the Serial class will
send the data out directly over RS232, providing the ability to send
debugging messages to an attached terminal. Send() will attempt to
serialise any data arrays passed to it and send it out in byte order.
For example:

uint32_t intData[] = {12347, 22, 2298};
Serial::Send(intData, 3u);

will send:

{00}{00}{30}{3B}{00}{00}{00}{16}{00}{00}{08}{FA}
*********************************************************************/


#ifndef SERIAL_H_
#define SERIAL_H_

#ifndef F_CPU
#undef F_CPU //this is important that uart clock frequency is correct
#define F_CPU 7372800
#endif //F_CPU

#define HOOK_IO //undefine this to prevent printf() sending out to uart

#include <stdio.h>
#include <avr/io.h>

namespace avr
{
	
class Serial final
{
public:	
	//if useInterrupt is true an interrupt is triggered each time a byte is received
	static void Init(bool useInterrupt = false);
	//send arbitrary array of data over uart
	template<typename T>
	static void Send(const T* data, uint32_t len)
	{
		auto size = sizeof(T);
		for(auto i = 0u; i < len; ++i)
		{
			auto current = data[i];
			for(auto j = 0u; j < size; ++j)
			{
				uint8_t byteToSend = (current >> (8 * ((size - 1) - j)));
				while(!(UCSRA & (1 << UDRE)));
				UDR = byteToSend;
			}
		}
	}
	//blocking function, waits until a byte is available before returning it
	//for non-blocking initialise with interrupts enabled and modify interrupt
	//vector in serial.cpp
	static uint8_t Receive();
	static void Flush();

private:
	//these are used internally so printf() will print to the uart
	static int PutChar(char c, FILE* stream);
	static int GetChar(FILE* stream);
};
}
#endif /* SERIAL_H_ */