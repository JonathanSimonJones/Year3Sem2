#ifndef WINDOWS_CLASS_H
#define WINDOWS_CLASS_H

/////////////////////////////////////////////////////////////////////////
//	WindowsClass.h
//	Jonathan Jones
//
//	Creates a Windows window
/////////////////////////////////////////////////////////////////////////

#include <Windows.h>			// Necessary to initialise a Window's window
#include <memory>				// Necessary for unique_ptr
#include "DirectX10Class.h"		// Necessary to create a DirectX scene
#include "GameTimer.h"
#include "Scene.h"

namespace MyWindowsCode
{
	class WindowsClass
	{
	public:
		// Constructor and deconstructor
		WindowsClass(HINSTANCE PassedHandleInstance, int PassedWindowState);						
		~WindowsClass();

		// Functions need to create the window
		WNDCLASS CreateWindowDescription(HINSTANCE PassedHandleInstance);							// Set up the what type of window to create
		void CalculateWindowDimensions();															// Create a RECT object which describes the dimensions of the window to pass to CreateTheWindow()
		void CreateTheWindow( HINSTANCE PassedHandleInstance);										// Create the window using the window description
		void DisplayWindow(int StateInWhichWindowsStarts);											// Display the window
		int Run();																					// Start handling the windows messages and run the application

		// Message handler
		LRESULT CALLBACK WindowsClassProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// The Windows class windows procedure which handles message from windows

		// Getters and setters
		HWND GetHandleToWindow();
		unsigned int GetWindowWidth();
		unsigned int GetWindowHeight();
	private:
		HWND m_HandleToWindow;		// Handle to the window
	
		unsigned int m_WindowWidth;						// The width of the window
		unsigned int m_WindowHeight;					// The height of the window
		unsigned int m_WindowWidthIncludingBorder;		// The width of the whole window 
		unsigned int m_WindowHeightIncludingBorder;		// The the height of the whole window

		//std::unique_ptr<MyD3D10Code::Direct3D10Class> m_Direct3DScene;
		std::unique_ptr<Scene> m_Direct3DScene;

		POINT m_OldMousePos;
	};
}

LRESULT CALLBACK GlobalWindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);		// The global windows procedure function which calls the Windows Class window procedure

#endif