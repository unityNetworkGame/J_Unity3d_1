#pragma once

NamespaceNetworkHead

class SocketAddress;

class NETWORK_EXPORT TcpSocket
{
public:
	~TcpSocket();

	int		Connect(const SocketAddress& inAddr);
	int		Bind(const SocketAddress& inToAddr);
	int		Listen(int inBackLog = 32);
	boost::shared_ptr< TcpSocket > Accept(SocketAddress& inFromAddr);
	int		Send(const void* inData, int inLength);
	int		Receive(void* inBuffer, int inLength);	

private:
	friend class SocketUtil;
	TcpSocket(SOCKET in_socket) :m_socket(in_socket) {}

private:
	SOCKET m_socket;
};

typedef boost::shared_ptr<TcpSocket> TcpSocketPtr;

NamespaceNetworkTail
