#include "stdafx.h"
#include "SocketAddress.h"
#include "TcpSocket.h"
#include "SocketUtil.h"


int Network::TcpSocket::Connect(const SocketAddress& inAddr)
{
	int error = connect(m_socket, &inAddr.m_sockaddr, inAddr.GetSize());
	if (error < 0)
	{
		std::string strInfo = "TCPScoket::Connect failed.";
		SocketUtil::AppendInfo(strInfo);
		return -SocketUtil::GetLastError();
	}

	return NOERROR;
}

int Network::TcpSocket::Bind(const SocketAddress& inToAddr)
{
	int error = bind(m_socket, &inToAddr.m_sockaddr, inToAddr.GetSize());

	if (error < 0)
	{
		std::string strInfo = "TCPScoket::Bind failed.";
		SocketUtil::AppendInfo(strInfo);
		return -SocketUtil::GetLastError();
	}

	return NOERROR;
}

int Network::TcpSocket::Listen(int inBackLog /*= 32*/)
{
	int error = listen(m_socket, inBackLog);
	if (error < 0)
	{
		std::string strInfo = "TCPScoket::Listen failed.";
		SocketUtil::AppendInfo(strInfo);
		return -SocketUtil::GetLastError();
	}

	return NOERROR;
}

boost::shared_ptr< Network::TcpSocket > Network::TcpSocket::Accept(SocketAddress& inFromAddr)
{
	int nLength = inFromAddr.GetSize();
	SOCKET accepted_socket = accept(m_socket, &inFromAddr.m_sockaddr, &nLength);

	if (accepted_socket == INVALID_SOCKET)
	{
		std::string info = "TcpSocket::Accept failed";
		SocketUtil::AppendInfo(info);
		return nullptr;
	}
	else
	{
		return TcpSocketPtr(new TcpSocket(accepted_socket));
	}
}


int Network::TcpSocket::Send(const void* inData, int inLength)
{
	int nBytesSent = send(m_socket, static_cast<const char*>(inData), inLength, 0);
	if (nBytesSent < 0)
	{
		std::string info = "TcpSocket::Send failed.";
		SocketUtil::AppendInfo(info);
		return -SocketUtil::GetLastError();
	}
	else
	{
		return nBytesSent;
	}
}

int Network::TcpSocket::Receive(void* inBuffer, int inLength)
{
	int nBytesReceived = recv(m_socket, static_cast<char *>(inBuffer), inLength, 0);
	if (nBytesReceived < 0)
	{
		std::string info = "TcpSocket::Receive failed.";
		SocketUtil::AppendInfo(info);
		return -SocketUtil::GetLastError();
	}
	else
	{
		return nBytesReceived;
	}
}

Network::TcpSocket::~TcpSocket()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
}




