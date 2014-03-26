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

//sends and receives data to a WIZnet W5100 Ethernet processor via SPI//
//including updating of network settings and writing to eeprom//

#ifndef WIZNET5100_H_
#define WIZNET5100_H_

#include <avr/io.h>

namespace avr
{
	
class WizComm final
{
public:
	static void Init();
	//destination address, data to write to it
	static void SPIWrite(unsigned regAddr, uint8_t data);
	//returns data at address
	static uint8_t SPIRead(unsigned regAddr);
	//address of wiznet register to update, data to send, size of data
	static void SetRegisterData(uint16_t regAddr, const uint8_t* data, uint32_t size);
	//address of wiznet register to read, pointer to array to hold data, amount of data to read
	static void GetRegisterData(uint16_t regAddr, uint8_t* data, uint32_t size);
	//writes new IP address to eeprom and updates Wiznet
	static void SetIpAddress(const uint8_t* addr);
	//writes new hardware address to eeprom and updates wiznet
	static void SetMacAddress(const uint8_t* addr);
	//writes new gateway address to eeprom and updates wiznet
	static void SetGatewayAddress(const uint8_t* addr);
	//writes new subnet mask to eeprom and updates wiznet
	static void SetSubnetMask(const uint8_t* addr);
	
private:
	//waits for SPI ready flag when transmitting
	static void SPITransmit();
};
}
#endif /* WIZNET5100_H_ */