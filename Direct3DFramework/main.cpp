///////////////////////////////////////
// Created by Jonathan Jones
//////////////////////////////////////

#include "WindowsClass.h"	// Necessary to create a window 
#include <crtdbg.h>			// Necessary to enable run time memory check for debug builds

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	MyWindowsCode::WindowsClass AWindow(hInstance, nShowCmd);

	return AWindow.Run();
}