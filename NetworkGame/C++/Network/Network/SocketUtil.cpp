#include "stdafx.h"
#include "TcpSocket.h"
#include "SocketUtil.h"

UsingNamespaceNetwork

std::map<int, std::list<std::string>> g_info_map;

void SocketUtil::AppendInfo( std::string info, InfoType type /*= Error*/)
{
	if (g_info_map.find((int)type) != g_info_map.end())
	{
		g_info_map[(int)type].push_back(info);
	}
	else
	{
		std::list<std::string> info_list;
		info_list.push_back(info);
		g_info_map[(int)type] = info_list;
	}
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

boost::shared_ptr<TcpSocket> SocketUtil::CreateTcpSocket()
{
	SOCKET tcpsocket = socket(AF_INET, SOCK_STREAM, 0);
	return 	boost::shared_ptr<TcpSocket>(new TcpSocket(tcpsocket));
}

#pragma region tcp select

fd_set* Network::SocketUtil::FillSetFromTcpsocketVec(fd_set& out_set, const std::vector<TcpSocketPtr>* inTcpSockets)
{
	if (inTcpSockets)
	{
		FD_ZERO(&out_set);
		for (const TcpSocketPtr& socket : *inTcpSockets)
		{
			FD_SET(socket->m_socket, &out_set);
		}
		return &out_set;
	}
	else
	{
		return nullptr;
	}
}

void SocketUtil::FillTcpsocketVecFromSet(std::vector<TcpSocketPtr>* outTcpSockets, const std::vector<TcpSocketPtr>* inTcpSockets, const fd_set& inSet)
{
	if (inTcpSockets && outTcpSockets)
	{
		outTcpSockets->clear();
		for (const TcpSocketPtr& socket : *inTcpSockets)
		{
			if (FD_ISSET(socket->m_socket, &inSet))
			{
				outTcpSockets->push_back(socket);
			}
		}
	}
}

int SocketUtil::Select(
	const std::vector<TcpSocketPtr>* inReadSet,
	std::vector<TcpSocketPtr>* outReadSet,
	const std::vector<TcpSocketPtr>* inWriteSet,
	std::vector<TcpSocketPtr>* outWriteSet,
	const std::vector<TcpSocketPtr>* inExceptSet,
	std::vector<TcpSocketPtr>* outExceptSet)
{
	fd_set read, write, except;

	fd_set* read_ptr = FillSetFromTcpsocketVec(read, inReadSet);
	fd_set* write_ptr = FillSetFromTcpsocketVec(read, inWriteSet);
	fd_set* except_ptr = FillSetFromTcpsocketVec(read, inExceptSet);

	//timeout 设置为不限制
	int sel_ret = select(0, read_ptr, write_ptr, except_ptr, nullptr);

	if (sel_ret > 0)// > 0 : 等待超时，没有可读写或错误的文件
	{
		FillTcpsocketVecFromSet(outReadSet, inReadSet, read);
		FillTcpsocketVecFromSet(outWriteSet, inWriteSet, write);
		FillTcpsocketVecFromSet(outExceptSet, inExceptSet, except);
	}

	return sel_ret;
}

#pragma endregion

