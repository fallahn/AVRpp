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

#include <Wiznet5100.hpp>
#include <IO.hpp>
#include <Serial.hpp>

#include <util/delay.h>
#include <avr/eeprom.h>
#include <string.h>

namespace
{
	//----configuration vars----//
	//AVR set to ATMega32/pin compatible
	volatile uint8_t* SPI_Port = &PORTB;
	volatile uint8_t* SPI_DDR = SPI_Port - 1u;
	const uint8_t SPI_SCK = 7u;
	const uint8_t SPI_MOSI = 5u;
	const uint8_t SPI_SS = 4u;
	const uint8_t W_RESET = 1u; //connected to reset pin of W5100 - pull low to reset
	const uint8_t SPI_EN = 0u; //connected to the SPI enable pin of W5100
	
	//----wiznet settings----//
	const uint8_t WizReadOpcode		= 0x0F;
	const uint8_t WizWriteOpcode	= 0xF0;
	const uint16_t TxBufferAddress  = 0x4000;
	const uint16_t RxBufferAddress  = 0x6000;
	const uint16_t TxBufferMask     = 0x07FF; //for 2k buffers - adjust according to buffer size
	const uint16_t RxBufferMask     = 0x07FF;
	const uint8_t ChannelSize       = 0x55; //2kb = 4 channel, 8kb = 1 channel etc.
	const uint16_t MaxBufferSize    = 512; //max size of incoming data buffer in bytes
	
	
	//wiznet register addresses
	const uint16_t ModeRegister				= 0x0000; //Mode Register
	const uint16_t GatewayAddressRegister	= 0x0001; //0x0001 - 0x0004
	const uint16_t SubnetMaskRegister		= 0x0005; //0x0005 - 0x0008
	const uint16_t HardwareAddressRegister	= 0x0009; //MAC address reg 0x0009 - 0x000E
	const uint16_t IpAddressRegister		= 0x000F; //0x000F - 0x0012
	const uint16_t RxSizeRegister			= 0x001A; //receive memory size
	const uint16_t TxSizeRegister			= 0x001B; //transmit memory size
	
	//default network settings
	EEMEM uint8_t e_macAddress[]	 = {0x00, 0x23, 0xCD, 0xBA, 0x7D, 0xDD};
	EEMEM uint8_t e_ipAddress[]		 = {192u, 168u, 001u, 190u};
	EEMEM uint8_t e_subnetMask[]	 = {255u, 255u, 255u, 000u};
	EEMEM uint8_t e_gatewayAddress[] = {192u, 168u, 001u, 001u};
		
	uint8_t macAddress[6];
	uint8_t ipAddress[4];
	uint8_t subnetMask[4];
	uint8_t gatewayAddress[4];
	
	const uint8_t macAddressSize = 6u;
	const uint8_t ipAddressSize = 4u;
}

using namespace wiz;
void WizComm::Init()
{	
#if DEBUG
	printf("Initialising W5100...\n\n");
#endif //DEBUG
	
	//load eeprom vals
	eeprom_read_block(static_cast<void*>(macAddress), static_cast<void*>(e_macAddress), macAddressSize);
	eeprom_read_block(static_cast<void*>(ipAddress), static_cast<void*>(e_ipAddress), ipAddressSize);
	eeprom_read_block(static_cast<void*>(subnetMask), static_cast<void*>(e_subnetMask), ipAddressSize);
	eeprom_read_block(static_cast<void*>(gatewayAddress), static_cast<void*>(e_gatewayAddress), ipAddressSize);
	
	//MOSI, SCK, SS and SPI_EN as output
	*SPI_DDR = (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_SS) | (1 << SPI_EN) | (1 << W_RESET);
	IO::Port::Set(SPI_SS, SPI_Port, IO::High);
	IO::Port::Set(SPI_EN, SPI_Port, IO::Low);
	//momentary low to reset W5100
	IO::Port::Set(W_RESET, SPI_Port, IO::Low);
	_delay_ms(2);
	IO::Port::Set(W_RESET, SPI_Port, IO::High);
	_delay_ms(2);

	//enable SPI and set to master
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR |= (1 << SPI2X); //SPI clock speed
	
	//set wiznet mode register to 0b10000000
	SPIWrite(ModeRegister, 0x80);	
	SetRegisterData(HardwareAddressRegister, macAddress, macAddressSize);
	SetRegisterData(IpAddressRegister, ipAddress, ipAddressSize);
	SetRegisterData(SubnetMaskRegister, subnetMask, ipAddressSize);
	SetRegisterData(GatewayAddressRegister, gatewayAddress, ipAddressSize);

	//set tx / rx channel size 1ch/8kb - 4ch/2kb
	SPIWrite(TxSizeRegister, ChannelSize);
	SPIWrite(RxSizeRegister, ChannelSize);
	
	//read back some data and print it to make sure it's written
#if DEBUG
	printf("Mode register set to: %d\n\n", SPIRead(ModeRegister));

	GetRegisterData(HardwareAddressRegister, macAddress, macAddressSize);
	printf("Hardware Address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", SPIRead(HardwareAddressRegister), macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
	GetRegisterData(IpAddressRegister, ipAddress, 4u);
	printf("IP Address: %d.%d.%d.%d\n", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
	GetRegisterData(SubnetMaskRegister, subnetMask, 4u);
	printf("Subnet Mask: %d.%d.%d.%d\n", subnetMask[0], subnetMask[1], subnetMask[2], subnetMask[3]);
	GetRegisterData(GatewayAddressRegister, gatewayAddress, 4u);
	printf("Gateway: %d.%d.%d.%d\n", gatewayAddress[0], gatewayAddress[1], gatewayAddress[2],gatewayAddress[3]);
	printf("\nW5100 initialisation complete.\n----------------\n\n");
	
#endif //DEBUG
}

void WizComm::SPIWrite(unsigned addr, uint8_t data)
{
	//printf("Writing addr %d value %d\n", addr, data);
		
	//activates SPI SS pin, tells the W5100 to expect a write
	IO::Port::Set(SPI_SS, SPI_Port, IO::Low);
	IO::Port::Set(SPI_EN, SPI_Port, IO::High);
	
	SPDR = WizWriteOpcode;
	SPITransmit();
	
	//send address of W5100 reg to write to and send data
	SPDR = (addr & 0xFF00) >> 8;
	SPITransmit();
	
	SPDR = addr & 0x00FF;
	SPITransmit();
	
	SPDR = data;
	SPITransmit();
	
	//deactivate again
	IO::Port::Set(SPI_SS, SPI_Port, IO::High);
	IO::Port::Set(SPI_EN, SPI_Port, IO::Low);
	_delay_ms(100);
}

uint8_t WizComm::SPIRead(unsigned addr)
{
	IO::Port::Set(SPI_SS, SPI_Port, IO::Low);
	IO::Port::Set(SPI_EN, SPI_Port, IO::High);
	
	SPDR = WizReadOpcode;
	SPITransmit();
	
	SPDR = (addr & 0xFF00) >> 8;
	SPITransmit();
	
	SPDR = addr & 0x00FF;
	SPITransmit();
	
	SPDR = 0x00; //empty data to receive
	SPITransmit();
	
	IO::Port::Set(SPI_SS, SPI_Port, IO::High);
	IO::Port::Set(SPI_EN, SPI_Port, IO::Low);
	return SPDR;
}

void WizComm::SetRegisterData(uint16_t regAddr, const uint8_t* data, uint32_t size)
{
	for(auto i = 0u; i < size; ++i)
		SPIWrite(regAddr + i, data[i]);
}

void WizComm::GetRegisterData(uint16_t regAddr, uint8_t* data, uint32_t size)
{
	for(auto i = 0u; i < size; ++i)
		data[i] = SPIRead(regAddr + i);
}

void WizComm::SetMacAddress(const uint8_t* addr)
{
	memcpy(macAddress, addr, macAddressSize);
	
	SetRegisterData(HardwareAddressRegister, macAddress, macAddressSize);
	eeprom_update_block(static_cast<const void*>(macAddress), static_cast<void*>(e_macAddress), macAddressSize);	
}

void WizComm::SetIpAddress(const uint8_t* addr)
{
	memcpy(ipAddress, addr, ipAddressSize);
		
	SetRegisterData(IpAddressRegister, ipAddress, ipAddressSize);
	eeprom_update_block(static_cast<const void*>(ipAddress), static_cast<void*>(e_ipAddress), ipAddressSize);
}

void WizComm::SetSubnetMask(const uint8_t* addr)
{
	memcpy(subnetMask, addr, ipAddressSize);
		
	SetRegisterData(SubnetMaskRegister, subnetMask, ipAddressSize);
	eeprom_update_block(static_cast<const void*>(subnetMask), static_cast<void*>(e_subnetMask), ipAddressSize);
}

void WizComm::SetGatewayAddress(const uint8_t* addr)
{
	memcpy(gatewayAddress, addr, ipAddressSize);
		
	SetRegisterData(GatewayAddressRegister, gatewayAddress, ipAddressSize);
	eeprom_update_block(static_cast<const void*>(gatewayAddress), static_cast<void*>(e_gatewayAddress), ipAddressSize);
}

bool WizComm::OpenSocket(uint8_t socket, uint8_t protocol, uint16_t port)
{
	uint16_t statusRegister = SockReg::GetSocketRegAddress(SockReg::SR, socket);
	//make sure socket is closed before trying to open it
	if(SPIRead(statusRegister) == SockStatus::Closed)
		SendCommand(socket, SockCommand::Close);
	
	//set socket protocol and port	
	SPIWrite(SockReg::GetSocketRegAddress(SockReg::MR, socket), protocol);
	uint16_t portRegister = SockReg::GetSocketRegAddress(SockReg::PORT, socket);
	SPIWrite(portRegister, ((port & 0xff00) >> 8));
	SPIWrite(portRegister + 1, (port & 0x00ff));
	
	//open socket
	SendCommand(socket, SockCommand::Open);
	
	//check status to see if we were successful
	if(SPIRead(statusRegister) == SockStatus::Init)
	{
		return true;
	}
	else
	{
		SendCommand(socket, SockCommand::Close);
	}
	return false;
}

bool WizComm::Connect(uint8_t socket, const uint8_t* addr, uint16_t port)
{
	//make sure we're trying to make a TCP connection
	if(SPIRead(SockReg::GetSocketRegAddress(SockReg::MR, socket)) != SockMode::TCP)
	{
#if DEBUG
		printf("Socket not in TCP mode, aborting connection attempt\n");
#endif
		SendCommand(socket, SockCommand::Close);
		return false;
	}
	
	//try to connect
	uint16_t statusRegister = SockReg::GetSocketRegAddress(SockReg::SR, socket);
	if(SPIRead(statusRegister) == SockStatus::Init)
	{
		/*TODO finish this*/
		
		return true;
	}	
	
	
	return false;
}

bool WizComm::Listen(uint8_t socket)
{
	//check socket open and initialised
	uint16_t statusRegister = SockReg::GetSocketRegAddress(SockReg::SR, socket);
	if(SPIRead(statusRegister) == SockStatus::Init)
	{
		//send request for listen status and wait for result
		SendCommand(socket, SockCommand::Listen);
		//check for success
		if(SPIRead(statusRegister) == SockStatus::Listen)
		{
			return true;
		}
		else
		{
			SendCommand(socket, SockCommand::Close);
		}
	}
	return false;
}

void WizComm::SendCommand(uint8_t socket, SockCommand::Command command)
{
	uint16_t controlRegister = SockReg::GetSocketRegAddress(SockReg::CR, socket);
	SPIWrite(controlRegister, command);
	while(SPIRead(controlRegister));
}

bool WizComm::SendData(uint8_t socket, const uint8_t* data, uint16_t size)
{
	if(!size) return false;
#if DEBUG
	printf("Sending %d bytes\n", size);
#endif //DEBUG	
	
	//check transmit size register is available
	uint16_t freeSizeReg = SockReg::GetSocketRegAddress(SockReg::TX_FSR, socket);
	uint16_t txsize = SPIRead(freeSizeReg);
	txsize = ((txsize & 0x00ff) << 8u) + SPIRead(freeSizeReg + 1u);
	
#if DEBUG
	printf("TX buffer %d free bytes\n", txsize);
#endif

	//wait for enough bytes to become available for data length
	uint16_t timeout = 0u;
	while(txsize < size)
	{
		_delay_ms(1);
		txsize = SPIRead(freeSizeReg);
		txsize = ((txsize & 0x00ff) << 8u) + SPIRead(freeSizeReg + 1u);
		
		if(timeout++ > 1000u)
		{
			//quit
#if DEBUG
			printf("TX free size timed out\n");
#endif
			SendCommand(socket, SockCommand::Disconnect);
			return false;
		}
	}
	
	//get pointer to W5100 tx buffer and fill with our data
	uint16_t writePointerRegister = SockReg::GetSocketRegAddress(SockReg::TX_WR, socket);
	uint16_t pointer = SPIRead(writePointerRegister);
	uint16_t offsetAddress = ((pointer & 0x00ff) << 8) + SPIRead(writePointerRegister + 1u);
#if DEBUG
	printf("TX buffer found at: 0x%x\n", offsetAddress);
#endif
	while(size--)
	{
		//calc physical buffer address on W5100
		uint16_t physicalAddress = (TxBufferAddress + (2048 * socket/*offset for socketId by channel width*/)) + (offsetAddress & TxBufferMask);
		//write next data byte
		SPIWrite(physicalAddress, *data);
		offsetAddress++;
		data++;
	}
	
	//increment pointer register so it points to next empty space
	SPIWrite(writePointerRegister, (offsetAddress & 0xff00) >> 8);
	SPIWrite(writePointerRegister + 1u, (offsetAddress & 0x00ff));
	//and send!
	SendCommand(socket, SockCommand::Send);
	
	return true;
}

bool WizComm::RecvData(uint8_t socket, uint8_t* data, uint16_t size)
{
	if(!size) return false;
	if(size > MaxBufferSize) size = MaxBufferSize - 2u;
	
	//get pointer to rx buffer on W5100
	uint16_t readPointerRegister = SockReg::GetSocketRegAddress(SockReg::RX_RD, socket);
	uint16_t pointer = SPIRead(readPointerRegister);
	uint16_t offsetAddress = (((pointer & 0x00ff) << 8) + SPIRead(readPointerRegister + 1u));
	
#if DEBUG
	printf("RX buffer address: 0x%x\n", offsetAddress);
#endif
	//read data into buffer
	while(size--)
	{
		uint16_t physicalAddress = (RxBufferAddress + (2048 * socket/*see tx func*/)) + (offsetAddress & RxBufferMask);
		*data = SPIRead(physicalAddress);
		offsetAddress++;
		data++;
	}
	//null terminate data
	*data = '\0';
	
	//increase buffer pointer to point to the next receive buffer
	SPIWrite(readPointerRegister, (offsetAddress & 0xff00) >> 8);
	SPIWrite(readPointerRegister + 1u, (offsetAddress & 0x00ff));
	
	//issue send command and wait
	uint16_t commandRegister = SockReg::GetSocketRegAddress(SockReg::CR, socket);
	SPIWrite(commandRegister, SockCommand::Recv);
	_delay_us(6);
	
	return true;
}

uint16_t WizComm::RecvSize(uint8_t socket)
{
	uint16_t receivedSizeRegister = SockReg::GetSocketRegAddress(SockReg::RX_RSR, socket);
	return ((SPIRead(receivedSizeRegister) & 0x00ff) << 8) + SPIRead(receivedSizeRegister + 1u);
}

//private
void WizComm::SPITransmit()
{
	while(!(SPSR & (1 << SPIF)));
}