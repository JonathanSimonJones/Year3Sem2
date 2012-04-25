#include "..\\HeaderFiles\\DirectX10Class.h"

namespace MyD3D10Code
{
	Direct3D10Class::Direct3D10Class(HWND TheWindow, unsigned int WindowWidth, unsigned int WindowHeight):
	m_DirectX10ClassWindowHandle(TheWindow),
	m_WindowWidth(WindowWidth),
	m_WindowHeight(WindowHeight),
	m_Direct3DDriverType(D3D10_DRIVER_TYPE_HARDWARE),
	m_Direct3DDevice(0),
	m_SwapChain(0),
	m_Direct3DInstanceMultiSample(1,0),
	m_RenderTargetView(0),
	m_DepthStencilBuffer(0),
	m_DepthStencilView(0),
	m_Font(0),
	m_Timer(),
	m_FrameStats(L""),
	m_ClearColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) ),
	m_SceneStates()
	{
		// Create a swap chain description and pass this to the function which creates the swap chain and the device
		CreateTheSwapChainAndDirect3DDevice( CreateSwapChainDesc() );
	
		// Create the render target view
		CreateRenderTargetView();

		// Create the depth/stencil view by passing in a depth/stencil description
		CreateDepthStencilView( CreateDepthStencilDescription() );

		// Set render target view and the depth/stencil view to the output merger stage
		SetRenderTargetsToOutputMergerStage();

		// Setup the viewport
		SetViewportToDirect3DDevice( CreateViewPort() );

		// Create the font object for text output
		CreateFontObject( CreateFontDescription() );
	}

	Direct3D10Class::~Direct3D10Class()
	{
		// Release all the ?resources?
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_SwapChain);
		ReleaseCOM(m_DepthStencilBuffer);
		ReleaseCOM(m_Direct3DDevice);
		ReleaseCOM(m_Font);

		if( m_Direct3DDevice )
		{
			m_Direct3DDevice->ClearState();
		}
	}

	DXGI_SWAP_CHAIN_DESC Direct3D10Class::CreateSwapChainDesc()
	{
		// Create swap chain descritption
		DXGI_SWAP_CHAIN_DESC SwapChainDescription;

		SwapChainDescription.BufferDesc.Width					= m_WindowWidth;						// Set buffer width
		SwapChainDescription.BufferDesc.Height					= m_WindowHeight;						// Set buffer height
		SwapChainDescription.BufferDesc.RefreshRate.Numerator	= 60;									// Display mode refresh rate numerator in hertz
		SwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;									// Display mode refresh rate denominator in hertz
		SwapChainDescription.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			// Format of each pixel elements
		SwapChainDescription.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// ?The scanline order on how to draw to the buffer?
		SwapChainDescription.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		// ?How scaling is handled and weather it is centered on the screen?

		// Set up multisampling, currently no multisampling
		SwapChainDescription.SampleDesc.Count	= m_Direct3DInstanceMultiSample.GetCount();		// The number of multi sample to take per pixel
		SwapChainDescription.SampleDesc.Quality = m_Direct3DInstanceMultiSample.GetQuality();	// ?The image quality level?

		SwapChainDescription.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// Use the back buffer to render
		SwapChainDescription.BufferCount	= 1;								// Specifies the number of back buffers in swap chain
		SwapChainDescription.OutputWindow	= m_DirectX10ClassWindowHandle;		// Handle to the window we are rendering to
		SwapChainDescription.Windowed		= true;								// Handles if we are in fullscreen or not, true = not fullscreen
		SwapChainDescription.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;			// Lets the display driver decide select the most effiecient presentation method
		SwapChainDescription.Flags			= 0;								// Additional flags, e.g set a flag so that when thge window is set to full screen DirectX will choose a display mode that best matches the new window

		return SwapChainDescription;	// Return the swap chain description
	}

	void Direct3D10Class::CreateTheSwapChainAndDirect3DDevice(DXGI_SWAP_CHAIN_DESC SwapChainDescription)
	{
		// Initialise dubug device flag
		UINT CreateDebugDeviceFlags = 0;

		// If we are debugging then set device flag to enable the debug layer
		#if defined(DEBUG) || defined(_DEBUG)  
			CreateDebugDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
		#endif

		// Create the device and swap chain
		HR( D3D10CreateDeviceAndSwapChain(
				0,						// Default display adapter
				m_Direct3DDriverType,	// Specify if we want hardware acceleration (D3D10_DRIVER_TYPE_HARDWARE) or if we want to create a software implementation for testign purposes (D3D10_DRIVER_TYPE_REFERENCE)
				0,						// Specify if we are going to use a software rasteriser
				CreateDebugDeviceFlags, // Flag specifying if we are debugging or not
				D3D10_SDK_VERSION,		// The version of the SDK we are using
				&SwapChainDescription,	// Pointer to the swap chain description
				&m_SwapChain,			// Returns the created swap chain
				&m_Direct3DDevice		// Returns the created device
				) );
	}

	void Direct3D10Class::CreateRenderTargetView()
	{
		// May not be necessary to include below function
		HR(m_SwapChain->ResizeBuffers(1, m_WindowWidth, m_WindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		// Create render target view
		ID3D10Texture2D* BackBuffer;																		// Create the back buffer which will be drawn to
		HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&BackBuffer)));	// Get a pointer to the swap chain back buffer and set it to the back buffer texture
		HR(m_Direct3DDevice->CreateRenderTargetView(BackBuffer, 0, &m_RenderTargetView));					// Create the render target view using the back buffer as a resource to render to
		ReleaseCOM(BackBuffer);																				// Release the COM reference count to the back buffer as we have increased it by using GetBuffer()
	}

	D3D10_TEXTURE2D_DESC Direct3D10Class::CreateDepthStencilDescription()
	{
		// Create Depth/Stencil description
		D3D10_TEXTURE2D_DESC DepthStencilDescription;

		DepthStencilDescription.Width				= m_WindowWidth;								// Height of window
		DepthStencilDescription.Height				= m_WindowHeight;								// Width of window
		DepthStencilDescription.MipLevels			= 1;											// The number of mipmap levels
		DepthStencilDescription.ArraySize			= 1;											// The number of textures in the texture array
		DepthStencilDescription.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;				// The format of the texels
		DepthStencilDescription.SampleDesc.Count	= m_Direct3DInstanceMultiSample.GetCount();		// The number of multisamples to use
		DepthStencilDescription.SampleDesc.Quality	= m_Direct3DInstanceMultiSample.GetQuality();	// The quality level
		DepthStencilDescription.Usage				= D3D10_USAGE_DEFAULT;							// How the texture will be used, will the gpu or cpu be allowed to read or write, cureently the gpu can read and write
		DepthStencilDescription.BindFlags			= D3D10_BIND_DEPTH_STENCIL;						// Where the resource will be bound to the pipeline, currently bout to the output merger stage
		DepthStencilDescription.CPUAccessFlags		= 0;											// Specifies what access the CPU is allowed
		DepthStencilDescription.MiscFlags			= 0;											// Not used for depth/stencil buffer

		return DepthStencilDescription;	// Return Dpeth/Stencil description
	}

	void Direct3D10Class::CreateDepthStencilView(D3D10_TEXTURE2D_DESC DepthStencilDescription)
	{
		// Create a depth/stencil buffer using the depth/stencil description
		HR(m_Direct3DDevice->CreateTexture2D(&DepthStencilDescription, 0, &m_DepthStencilBuffer));

		// Create the depth/stencil view
		HR(m_Direct3DDevice->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));
	}

	void Direct3D10Class::SetRenderTargetsToOutputMergerStage()
	{
		m_Direct3DDevice->OMSetRenderTargets(1,						// Number of render targets to bind
											 &m_RenderTargetView,	// The render target view
											 m_DepthStencilView);	// The Depth/Stencil view
	}

	D3D10_VIEWPORT Direct3D10Class::CreateViewPort()
	{
		// Create Viewport description
		D3D10_VIEWPORT ViewPort;

		ViewPort.TopLeftX = 0;				// Top left x coord of viewport
		ViewPort.TopLeftY = 0;				// Top left y coord of viewport
		ViewPort.Width	  = m_WindowWidth;	// Viewport width
		ViewPort.Height   = m_WindowHeight;	// Viewport height
		ViewPort.MinDepth = 0.0f;			// Minimum depth buffer value
		ViewPort.MaxDepth = 1.0f;			// Maximum depth buffer value

		return ViewPort;
	}

	void Direct3D10Class::SetViewportToDirect3DDevice(D3D10_VIEWPORT Viewport)
	{
		m_Direct3DDevice->RSSetViewports(1,				// Number of viewports 
										 &Viewport);	// The viewport
	}

	D3DX10_FONT_DESC Direct3D10Class::CreateFontDescription()
	{
		D3DX10_FONT_DESC FontDescription;
		FontDescription.Height          = 24;							// Height of font character
		FontDescription.Width           = 0;							// Width of font character
		FontDescription.Weight          = 0;							// Weight of font, 0 - 1000
		FontDescription.MipLevels       = 1;							// Mipmap level, currently set so that the texture space is exactly mapped to the screen space
		FontDescription.Italic          = false;						// Do you want italic text
		FontDescription.CharSet         = DEFAULT_CHARSET;				// ?Character set/
		FontDescription.OutputPrecision = OUT_DEFAULT_PRECIS;			// How closely the output must match the requested parameters
		FontDescription.Quality         = DEFAULT_QUALITY;				// Output quality
		FontDescription.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;	// ???
		wcscpy(FontDescription.FaceName, L"Times New Roman");			// ?The type of of font to use, used to identify which font to use?

		return FontDescription;
	}

	void Direct3D10Class::CreateFontObject(D3DX10_FONT_DESC FontDescription)
	{

		// Create the font object
		D3DX10CreateFontIndirect(m_Direct3DDevice, &FontDescription, &m_Font);
	}

	void Direct3D10Class::SetWindowWidth(unsigned int PassedWindowWidth)
	{
		m_WindowWidth = PassedWindowWidth;
	}
	
	void Direct3D10Class::SetWindowHeight(unsigned int PassedWindowHeight)
	{
		m_WindowHeight = PassedWindowHeight;
	}


	void Direct3D10Class::ResizeDirectXWindow()
	{
		
		// Release the old views, as they hold references to the buffers we
		// will be destroying.  Also release the old depth/stencil buffer.
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);
		ReleaseCOM(m_Font);

		// Resize the swap chain so that it fits with the new dimensions of the window
		HR(m_SwapChain->ResizeBuffers(1, m_WindowWidth, m_WindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		// Create a swap chain description and pass this to the function whcih creates the swap chain and the device
		CreateTheSwapChainAndDirect3DDevice( CreateSwapChainDesc() );
	
		// Create the render target view
		CreateRenderTargetView();

		// Create the depth/stencil view by passing in a depth/stencil description
		CreateDepthStencilView( CreateDepthStencilDescription() );

		// Set render target view and the depth/stencil view to the output merger stage
		SetRenderTargetsToOutputMergerStage();

		// Setup the viewport
		SetViewportToDirect3DDevice( CreateViewPort() );

		CreateFontObject( CreateFontDescription() );
	}

	void Direct3D10Class::UpdateScene(float DeltaTime)
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.

		static int frameCnt = 0;
		static float t_base = 0.0f;

		frameCnt++;

		// Compute averages over one second period.
		if( (m_Timer.getGameTime() - t_base) >= 1.0f )
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;

			std::wostringstream outs;   
			outs.precision(6);
			outs << L"FPS: " << fps << L"\n" 
					<< "Milliseconds: Per Frame: " << mspf;
			m_FrameStats = outs.str();
		
			// Reset for next average.
			frameCnt = 0;
			t_base  += 1.0f;
		}
	}

	void Direct3D10Class::DrawScene()
	{
		m_Direct3DDevice->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
		m_Direct3DDevice->ClearDepthStencilView(m_DepthStencilView,						// The view to clear 
												D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL,	// Which buffer/buffers to clear
												1.0f,									// The float value set to set each pixel to in the depth buffer
												0);										// The interger value to set each pixel to in the stencil buffer

		// We specify DT_NOCLIP, so we do not care about width/height of the rect.
		//RECT R = {5, 5, 0, 0};
		//m_Font->DrawText(0, m_FrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);

		// Took this out and put it in Coloured cube DrawScene
		//m_SwapChain->Present(0, 0);
	}

	void Direct3D10Class::SetSceneStateToPaused()
	{
		m_SceneStates.SetStateToPaused();
		m_Timer.stop();
	}

	void Direct3D10Class::SetSceneStateToRunning()
	{
		m_SceneStates.SetStateToUnPaused();
		m_Timer.start();
	}

	void Direct3D10Class::SetSceneToMinimized()
	{
		m_SceneStates.SetStateToPaused();
		m_SceneStates.SetStateToMinimized();
		m_SceneStates.SetStateToUnMaximized();
	}

	void Direct3D10Class::SetSceneToMaximized()
	{
		m_SceneStates.SetStateToUnPaused();
		m_SceneStates.SetStateToUnMinimized();
		m_SceneStates.SetStateToMaximized();
		ResizeDirectXWindow();
	}

	// Could potentially cause problems
	void Direct3D10Class::RestoreScene()
	{
		if(m_SceneStates.Minimized() == true)
		{
			m_SceneStates.SetStateToUnPaused();
			m_SceneStates.SetStateToUnMinimized();
			ResizeDirectXWindow();
		}
		else if(m_SceneStates.Maximized() == true)
		{
			m_SceneStates.SetStateToUnPaused();
			m_SceneStates.SetStateToUnMaximized();
			ResizeDirectXWindow();
		}
	}

	void Direct3D10Class::WindowBeingResized()
	{
		m_SceneStates.SetStateToPaused();
		m_SceneStates.SetStateToResized();
		m_Timer.stop();
	}

	void Direct3D10Class::WindowFinishedBeingResized()
	{
		m_SceneStates.SetStateToUnPaused();
		m_SceneStates.SetStateToResizeComplete();
		m_Timer.start();
		ResizeDirectXWindow();
	}

	bool Direct3D10Class::ScenePaused()
	{
		return m_SceneStates.Paused();
	}

	bool Direct3D10Class::SceneMinimized()
	{
		return m_SceneStates.Minimized();
	}

	bool Direct3D10Class::SceneMaximized()
	{
		return m_SceneStates.Maximized();
	}

	bool Direct3D10Class::SceneBeingResized()
	{
		return m_SceneStates.Resizing();
	}

	void Direct3D10Class::StartTimer()
	{
		m_Timer.reset();
	}

	void Direct3D10Class::TickTimer()
	{
		m_Timer.tick();
	}

	float Direct3D10Class::GetDeltaTime()
	{
		return m_Timer.getDeltaTime();
	}
}