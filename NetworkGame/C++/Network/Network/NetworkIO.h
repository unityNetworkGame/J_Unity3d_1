#pragma once

#include "SocketInclude.h"

#ifdef _DEBUG
#pragma comment(lib, "NetworkD.dll")
#else
#pragma comment(lib, "Network.dll")
#endif

#include "SocketAddress.h"
#include "TcpSocket.h"
#include "SocketUtil.h"
