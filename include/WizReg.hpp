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

//socket registers of WIZnet W5100 - see W5100 data sheet for full details

#ifndef WIZREG_H_
#define WIZREG_H_

namespace wiz
{
//Addresses of Socket registers on W5100
//don't use these directly - use Util::GetSocketRegAddress()
enum class SockReg
{
	MR     = 0x0000, //mode register
	CR     = 0x0001, //command register
	IR     = 0x0002, //interrupt register
	SR     = 0x0003, //status register	
	PORT   = 0x0004, //source port 04-05	
	DHAR   = 0x0006, //destination hardware address 06-0B
	DIPR   = 0x000C, //destination IP address 0C-0F
	DPORT  = 0x0010, //destination port 10-11
	MSSR   = 0x0012, //max segment size 12-13
	PROTO  = 0x0014, //protocol in raw mode
	TOS    = 0x0015, //IP TOS
	TTL    = 0x0016, //IP TTL
	TX_FSR = 0x0020, //TX free size 20-21
	TX_RD  = 0x0022, //TX read pointer 22-23
	TX_WR  = 0x0024, //TX write pointer 24-25
	RX_RSR = 0x0026, //RX received size 26-27
	RX_RD  = 0x0028  //RX read pointer 28-29
};

struct Util final
{
	static uint16_t GetSocketRegAddress(SockReg reg, uint8_t socketId)
	{
		//register address upper byte starts at 0x04 and increases with Socket
		const uint8_t socket = 0x04 + socketId;
		return static_cast<uint16_t>(reg) | (socket << 8);
	}
};

//register values - common to each socket register
enum class SockMode //mode register
{
	Closed    = 0x00, //socket unused
	TCP       = 0x01, //TCP mode
	UDP       = 0x02, //UDP mode
	IpRaw     = 0x03, //IP layer raw
	MacRaw    = 0x04, //MAC layer raw
	PPPoE     = 0x05, //PPPoE
	NoDelay   = 0x20, //no delay ack in TCP mode
	Multicast = 0x80 //enable multicast
};

enum class SockCommand //command register
{
	Open       = 0x01, //open socket
	Listen     = 0x02, //listen for TCP connections
	Connect    = 0x04, //connect to TCP socket
	Disconnect = 0x08, //close a TCP connection0
	Close      = 0x10, //close the socket
	Send       = 0x20, //update TX pointer and send data
	SendMac    = 0x21, //send data with MAC, no ARP mode
	SendKeep   = 0x22, //send keep alive
	Recv       = 0x40  //update RX pointer and receive data
};

enum class SockStatus //status register
{
	Closed      = 0x00, //socket is closed
	Init        = 0x13, //TCP initiated, waiting for listen or connect
	Listen      = 0x14, //set to TCP listen
	Established = 0x17, //TCP connection established
	CloseWait   = 0x1C, //disconnect request received, waiting close
	UDP         = 0x22, //socket is UDP
	IPRaw       = 0x32, //socket is IP raw
	MacRaw      = 0x42, //socket is MAC raw
	PPPoE       = 0x5F, //socket is PPPoE
	SynSent     = 0x15, //connection command given at init status
	SynRecv     = 0x16, //connection request received
	FinWait     = 0x18,
	Closing     = 0x1A,
	TimeWait    = 0x1B,
	LastAck     = 0x1D,
	ARP0        = 0x11,
	ARP1        = 0x21,
	ARP2        = 0x31
};

}




#endif /* WIZREG_H_ */