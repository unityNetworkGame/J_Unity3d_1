#pragma once

NamespaceNetworkHead

class NETWORK_EXPORT SocketAddress
{
	friend class TcpSocket;

public:
	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		sockaddr_in* addr_in = GetAsSockaddrIn();
		addr_in->sin_family = AF_INET;
		addr_in->sin_port = htons(inPort);
		addr_in->sin_addr.S_un.S_addr = inAddress;
	}
	SocketAddress(uint8_t inAddr_1, uint8_t inAddr_2, uint8_t inAddr_3, uint8_t inAddr_4, uint16_t inPort)
	{
		sockaddr_in* addr_in = GetAsSockaddrIn();
		addr_in->sin_family = AF_INET;
		addr_in->sin_port = htons(inPort);
		addr_in->sin_addr.S_un.S_un_b.s_b1 = inAddr_1;
		addr_in->sin_addr.S_un.S_un_b.s_b2 = inAddr_2;
		addr_in->sin_addr.S_un.S_un_b.s_b3 = inAddr_3;
		addr_in->sin_addr.S_un.S_un_b.s_b4 = inAddr_4;
	}
	SocketAddress(const sockaddr& inSockAddr)
	{
		m_sockaddr = inSockAddr;
	}
	SocketAddress(sockaddr_in& inSocketAddr)
	{
		m_sockaddr = *reinterpret_cast<sockaddr*>(&inSocketAddr);
	}
	SocketAddress()
	{

	}

	size_t GetSize() const { return sizeof(sockaddr); }
private:
	sockaddr m_sockaddr;

	sockaddr_in* GetAsSockaddrIn() 
	{
		return reinterpret_cast<sockaddr_in*>(&m_sockaddr);
	}
};

typedef boost::shared_ptr<SocketAddress> SocketAddressPtr;

NamespaceNetworkTail
