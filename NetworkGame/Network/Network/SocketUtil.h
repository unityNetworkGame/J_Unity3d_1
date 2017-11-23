#ifndef _socket_util_h_
#define _socket_util_h_
#pragma once
#include <vector>

NamespaceNetworkHead

class TcpSocket;

class NETWORK_EXPORT SocketUtil
{
public:
	enum InfoType
	{
		Error,
		Info,
	};
public:
	//ÃÌº”»’÷æ
	static void	AppendInfo(std::string info, InfoType type = Error);
	static int	GetLastError();
	static boost::shared_ptr<TcpSocket> CreateTcpSocket();

#pragma region tcp select

	static fd_set* FillSetFromTcpsocketVec(
		fd_set& out_set, 
		const std::vector<TcpSocketPtr>* inTcpSockets);

	static void FillTcpsocketVecFromSet(
		std::vector<TcpSocketPtr>* outTcpSockets,
		const std::vector<TcpSocketPtr>* inTcpSockets,
		const fd_set& inSet);

	static int Select(
		const std::vector<TcpSocketPtr>* inReadSet,
		std::vector<TcpSocketPtr>* outReadSet,
		const std::vector<TcpSocketPtr>* inWriteSet,
		std::vector<TcpSocketPtr>* outWriteSet,
		const std::vector<TcpSocketPtr>* inExceptSet,
		std::vector<TcpSocketPtr>* outExceptSet);

#pragma endregion
};

NamespaceNetworkTail

#endif

