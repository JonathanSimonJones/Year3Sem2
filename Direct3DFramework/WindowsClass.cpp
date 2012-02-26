#include "WindowsClass.h"

namespace MyWindowsCode
{
	WindowsClass::WindowsClass(HINSTANCE PassedHandleInstance, int PassedWindowState):
	m_HandleToWindow(0),
	m_WindowWidth(600),
	m_WindowHeight( (m_WindowWidth/5) * 4),
	m_WindowWidthIncludingBorder(0),
	m_WindowHeightIncludingBorder(0),
	m_OldMousePos()
	{
		// Create a WNDCLASS which describes the window to be created
		WNDCLASS TheWindowDescription = CreateWindowDescription(PassedHandleInstance);

		// Calculate the window dimensions
		CalculateWindowDimensions();

		// Pass the newly created WNDCLASS to the function which creates a Windows window
		CreateTheWindow( PassedHandleInstance );

		// Display the window 
		DisplayWindow(PassedWindowState);

		// Stores a pointer to the instance of the class so that we can create a member windows procedure
		SetWindowLongPtr(m_HandleToWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this) );

		// Initialise Direct 3D application
		m_Direct3DScene.reset(new Scene(m_HandleToWindow, m_WindowWidth, m_WindowHeight));
	}

	WindowsClass::~WindowsClass()
	{
	}

	WNDCLASS WindowsClass::CreateWindowDescription(HINSTANCE PassedHandleInstance)
	{
		// Create a WNDCLASS to describe what sort of window we want to create
		WNDCLASS WindowDescription;

		WindowDescription.style         = CS_HREDRAW | CS_VREDRAW;				// The style of the window 
		WindowDescription.lpfnWndProc   = GlobalWindowsProcedure;				// The windows procedure to call
		WindowDescription.cbClsExtra    = 0;									 
		WindowDescription.cbWndExtra    = 0;
		WindowDescription.hInstance     = PassedHandleInstance;					// The handle to the application instance
		WindowDescription.hIcon         = LoadIcon(0, IDI_SHIELD);				// The icon in the top right of the window
		WindowDescription.hCursor       = LoadCursor(0, IDC_IBEAM);				// the cursor that appears in the window
		WindowDescription.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);	// The background colour of the window
		WindowDescription.lpszMenuName  = 0;									// Specify the windows menu
		WindowDescription.lpszClassName = L"WindowsDescription";				// The name of the Windows Class structure created

		// Check to see if the window class was created correctly
		if(!RegisterClass(&WindowDescription))
		{
			// Display a message box if it was not
			MessageBox(0, L"Windows description FAILED", 0, 0);
		}

		// Return the newly created window
		return WindowDescription;
	}

	void WindowsClass::CalculateWindowDimensions()
	{
		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT WindowRectangle = { 0, 0, m_WindowWidth, m_WindowHeight };					// Set up a rect object to represent the window dimensions
		AdjustWindowRect(&WindowRectangle, WS_OVERLAPPEDWINDOW, false);					// Change the windows dimensions to include the border, title bar etc
		m_WindowWidthIncludingBorder  = WindowRectangle.right - WindowRectangle.left;	// Set variable to correct value
		m_WindowHeightIncludingBorder = WindowRectangle.bottom - WindowRectangle.top;	// Set variable to correct value
	}

	void WindowsClass::CreateTheWindow( HINSTANCE PassedHandleInstance)
	{
		// Set the window handle to a newly created window
		m_HandleToWindow = CreateWindow(
			L"WindowsDescription",				// Registered WNDCLASS instance to use.
			L"DirectXTemplate",					// window title
			WS_OVERLAPPEDWINDOW,				// style flags
			CW_USEDEFAULT,						// x-coordinate
			CW_USEDEFAULT,						// y-coordinate
			m_WindowWidthIncludingBorder,		// width
			m_WindowHeightIncludingBorder,		// height
			0,									// parent window
			0,									// menu handle
			PassedHandleInstance,				// app instance
			0);									// extra creation parameters

		// Check to see if the window was created correctly
		if(m_HandleToWindow == 0)
		{
			// Display a message box if it was not
			MessageBox(0, L"Create Window FAILED", 0, 0);
		}
	}

	void WindowsClass::DisplayWindow(int StateInWhichWindowsStarts)
	{
		// Show the newly created window
		ShowWindow(m_HandleToWindow, StateInWhichWindowsStarts);

		// Refresh the window
		UpdateWindow(m_HandleToWindow);
	}

	int WindowsClass::Run()
	{
		// Set up a windows message
		MSG msg = {0};

		// Start scene timer
		m_Direct3DScene->StartTimer();

		// Check to see if the user wants to quit
		while(msg.message != WM_QUIT)
		{
			// If there are Windows message then process them
			if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE) )
			{
				// Process the message
				TranslateMessage( &msg);
				DispatchMessage( &msg);
			}
			else
			{
				// Set the new frame rate
				m_Direct3DScene->TickTimer();

				// Check to see if the scene is paused
				if( m_Direct3DScene->ScenePaused() == false)
				{
					// Udates the scene
					m_Direct3DScene->UpdateScene( m_Direct3DScene->GetDeltaTime() );
				}
				else
				{
					// Wait for a few cycles
					Sleep(50);
				}
				// Draw the scene
				m_Direct3DScene->DrawScene();
			}
		}

		// When we want to quit exit the function and return 0
		return (int)msg.wParam;
	}


	LRESULT CALLBACK WindowsClass::WindowsClassProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// Variables used in store information about the mouse
		POINT mousePos;
		int dx = 0;
		int dy = 0;

		switch( msg )
		{
		/*
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
		
		case WM_ACTIVATE:
			if( LOWORD(wParam) == WA_INACTIVE )
			{
				m_Direct3DScene->SetSceneStateToPaused();
			}
			else
			{
				m_Direct3DScene->SetSceneStateToRunning();
			}
			return 0;

		// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			// Save the new client area dimensions.
			m_WindowWidth  = LOWORD(lParam);
			m_WindowHeight = HIWORD(lParam);

			///////////////////////////////////
			// SORT OUT!!!!!! Figure out which Window width and height parameter you want to pass back to DX
			///////////////////////////////////
			//CalculateWindowDimensions();
			m_Direct3DScene->SetWindowWidth(m_WindowWidth);
			m_Direct3DScene->SetWindowHeight(m_WindowHeight);
			
			if( m_Direct3DScene )
			{
				if( wParam == SIZE_MINIMIZED )
				{
					m_Direct3DScene->SetSceneToMinimized();
				}
				else if( wParam == SIZE_MAXIMIZED )
				{
					m_Direct3DScene->SetSceneToMaximized();
				}
				else if( wParam == SIZE_RESTORED )
				{
				
					// Restoring from minimized state?
					if( m_Direct3DScene->SceneMinimized() == true  )
					{
						m_Direct3DScene->RestoreScene();
					}

					// Restoring from maximized state?
					else if( m_Direct3DScene->SceneMaximized() == true )
					{
						m_Direct3DScene->RestoreScene();
					}
					else if( m_Direct3DScene->SceneBeingResized() == true)
					{
						// If user is dragging the resize bars, we do not resize 
						// the buffers here because as the user continuously 
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars.  So instead, we reset after the user is 
						// done resizing the window and releases the resize bars, which 
						// sends a WM_EXITSIZEMOVE message.
					}
					else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					{
						m_Direct3DScene->ResizeDirectXWindow();
					}
				}
			}
			return 0;
			
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		case WM_ENTERSIZEMOVE:
			m_Direct3DScene->WindowBeingResized();
			return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			m_Direct3DScene->WindowFinishedBeingResized();
			return 0;
		*/
		// WM_DESTROY is sent when the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		/*
		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
			return 0;
		*/
		case WM_LBUTTONDOWN:
			if( wParam & MK_LBUTTON )
			{
				SetCapture(m_HandleToWindow);

				m_OldMousePos.x = LOWORD(lParam);
				m_OldMousePos.y = HIWORD(lParam);
			}
			return 0;

		case WM_LBUTTONUP:
			ReleaseCapture();
			return 0;

		case WM_MOUSEMOVE:
			if( wParam & MK_LBUTTON )
			{
				mousePos.x = (int)LOWORD(lParam); 
				mousePos.y = (int)HIWORD(lParam); 

				dx = mousePos.x - m_OldMousePos.x;
				dy = mousePos.y - m_OldMousePos.y;

				GetCamera().pitch( dy * 0.0087266f );
				GetCamera().rotateY( dx * 0.0087266f );
			
				m_OldMousePos = mousePos;
			}
			return 0;
		}

		// Default windows procedure 
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	HWND WindowsClass::GetHandleToWindow()
	{
		return m_HandleToWindow;
	}

	unsigned int WindowsClass::GetWindowWidth()
	{
		return m_WindowWidth;
	}

	unsigned int WindowsClass::GetWindowHeight()
	{
		return m_WindowHeight;
	}
}

// The global windows procedure needed to pass the messages to the window Windows procedure
LRESULT CALLBACK GlobalWindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Create a return result
	LRESULT result = 0;

	// Set up the pointer which points to the window information
	LONG_PTR Window = GetWindowLongPtr(hWnd, GWLP_USERDATA);

	// Check to see if the pointer has been set properly
	if( Window )
	{
		// If is has call the windows class procedure
		result = ((MyWindowsCode::WindowsClass*)(Window))->WindowsClassProcedure(hWnd, msg, wParam, lParam);
	}
	else
	{
		// Call the default windows procedure
		result = DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
	return result;
}