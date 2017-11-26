#pragma once

#define NamespaceNetworkHead namespace Network {
#define NamespaceNetworkTail }
#define UsingNamespaceNetwork using namespace Network;


#ifdef _WINDOWS_
	#ifdef NETWORK_EXPORTS
		#define NETWORK_EXPORT __declspec(dllexport)
	#else
		#define NETWORK_EXPORT __declspec(dllimport)
	#endif
#else
	#define NETWORK_EXPORT
#endif