// NetworkDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include "../../Network/Network/NetworkIO.h"


UsingNamespaceNetwork
using namespace std;

#define IP_SERVER_S "127,0,0,1"
#define IP_SERVER 127,0,0,1
#define PROT_SERVER 9796
#define LISTENQ 10
#define BUFFER_SIZE 256

static TcpSocketPtr CreateServerSocket()
{
	/*sockaddr_in server_sock;
	server_sock.sin_family = AF_INET;
	inet_pton(AF_INET, IP_SERVER_S,  &server_sock.sin_addr);
	server_sock.sin_port = htons(PROT_SERVER);
	
	SocketAddress server_addr(server_sock);	*/

	SocketAddress server_addr(IP_SERVER, PROT_SERVER);

	cout << "create server socket:"<< endl << "IP_SERVER - " << IP_SERVER_S << endl << "PORT_SERVER - "<< to_string(PROT_SERVER) << endl;
	TcpSocketPtr server_socker_ptr = SocketUtil::CreateTcpSocket();
	if (server_socker_ptr == nullptr)
	{
		cout << "create server socket failed" << endl;
		return NULL;
	}
	else {
		cout << "create server socket success" << endl;
	}

	cout << "to bind server socket ..." << endl;
	int bind_ret = server_socker_ptr->Bind(server_addr);
	if (bind_ret < 0)
	{
		cout << "bind server socket failed. error code :" << to_string(bind_ret) << endl;
		return NULL;
	}
	cout << "bind server socket success." << endl;

	if (server_socker_ptr->Listen(LISTENQ) == -1)
	{
		cout << "server socket listen failed." << endl;
		return NULL;
	}

	cout << "server socket listen success" << endl;
	return server_socker_ptr;
}

static bool s_game_runing = true;
static void RunServerMainLoop(TcpSocketPtr server_socket_ptr)
{
	if (NULL == server_socket_ptr)
		return;

	std::vector<TcpSocketPtr> vecReadSockets;
	vecReadSockets.push_back(server_socket_ptr);

	std::vector<TcpSocketPtr> vecReadableSockets;

	while (s_game_runing)
	{
		if (SocketUtil::Select(&vecReadSockets, &vecReadableSockets, nullptr, nullptr, nullptr, nullptr))
		{
			for (const TcpSocketPtr& socket : vecReadableSockets)
			{
				if (socket == server_socket_ptr)
				{
					//如果是服务器监听socket，取出当前数据并将服务器socket重新放入ReadSockets中
					SocketAddress newSocketAddr;
					TcpSocketPtr new_srv_socket = server_socket_ptr->Accept(newSocketAddr);
					vecReadSockets.push_back(new_srv_socket);

					//处理接受到的数据
				}
				else
				{
					char buffer[BUFFER_SIZE];
					int recv = socket->Receive(buffer, BUFFER_SIZE);
					if (recv > 0)
					{
						//处理收到的客户端消息
						cout << "Receive from client" << buffer << endl;
					}
					else if (recv <= 0)
					{
						//表示数据发送完成可以关闭socket了
						//FD_CLR(socket,)
					}
				}
			}
		}
	}

}

class AutoWSA
{
public:
	AutoWSA()
	{
		WSADATA wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
	}
	~AutoWSA()
	{
		WSACleanup();
	}
};

int main()
{
	AutoWSA wsa;
	TcpSocketPtr server_socket_ptr =  CreateServerSocket();
	if (NULL == server_socket_ptr)
		return 1;

	RunServerMainLoop(server_socket_ptr);

	system("pause");
    return 0;
}

