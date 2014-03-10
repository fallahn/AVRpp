AVRpp
-----

Zlib license.

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

What is this?
-------------

AVRpp is a small library of classes written in C++ aimed at being reusable and easily integrated into new projects, designed to take care of common chores such as creating timers or reading the ADC. It's targetted at 8-bit AVR hardware such as the ATMega8515 or ATMega8 - although should be compatible with a fairly wide range of devices with minimal configuration.


How do I use it?
----------------

AVRpp requires the avr-g++ toolchain, installed with AtmelStudio by default on Windows, and also available for linux. Simply include the *.hpp and *.cpp file in your project for the required class. AVRpp uses C++11 features and requires -std=c++11 to be set when compiling.


Example:
--------

To use the clock class first initialise the timer at the beginning of main(). This is important, it sets up the chip's clock speeds and creates an interrupt used by the class. To configure the timer for a specific chip, see Clock constructor in Clock.cpp.


void main()
{
	Clock::Init();

	while(1)
	{
		//do stuff...
	}
}


You only have to initialse the clock once. Now a timer can be created anywhere in your code:

Clock::Timer myTimer;

and queried with:

float time = myTimer.Elapsed();

or

float time = myTimer.Restart();


