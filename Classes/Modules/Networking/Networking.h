#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#ifdef TARGET_OS_MAC
#include "./osx/Include/CNetworkManager.h"
#elif defined __linux__
#include "./linux/Include/CNetworkManager.h"
#elif defined _WIN32 | defined _WIN64
#include "./win/Include/CNetworkManager.h"
#else // android
#include "./android/Include/CNetworkManager.h"
#endif

#endif /* _NETWORKING_H_ */
