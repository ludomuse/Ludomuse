#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include <iostream>

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <Windows.h>
#include <ShellAPI.h>
#endif 

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	bool isServer = false;
	std::string filename;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

	if (argCount >= 2)
	{
		std::wstring warg(szArgList[1]);
		std::string arg(warg.begin(), warg.end());
		std::cout << "arg : " << arg << std::endl;
		isServer = arg == std::string("client") ? false : true;
	}

	if (isServer)
	{
		std::cout << "launching LudoMuse as a server" << std::endl;
	}
	else
	{
		std::cout << "launching LudoMuse as a client" << std::endl;
	}

	if (argCount >= 3)
	{
		std::wstring warg(szArgList[2]);
		filename = std::string(warg.begin(), warg.end());
		std::cout << "using config file : " << filename << std::endl;
	}
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32


	// create the application instance
    AppDelegate app(isServer, filename);
    return cocos2d::Application::getInstance()->run();
}
