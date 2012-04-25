#ifndef DIRECTX_10_CLASS_H
#define DIRECTX_10_CLASS_H

#include "d3dUtil.h"
#include "Multisample.h"
#include "GameTimer.h"
#include <string>			// Necessary for using wostringstream
#include <sstream>
#include "Direct3DSceneState.h"


namespace MyD3D10Code
{
	class Direct3D10Class
	{
	public:
		Direct3D10Class(HWND TheWindow, unsigned int WindowWidth, unsigned int WindowHeight);
		virtual ~Direct3D10Class();

		DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc();												// Set up the swap chain by creating a description of it which will be passed to CreateDeviceAndSwapChain()
		void CreateTheSwapChainAndDirect3DDevice(DXGI_SWAP_CHAIN_DESC SwapChainDescription);	// Set up the swap chain and the Direct 3D device, to set up the swap chain use the description provided
		void CreateRenderTargetView();															// Create the render target view
		D3D10_TEXTURE2D_DESC CreateDepthStencilDescription();									// Create a depth/stencil buffer description
		void CreateDepthStencilView(D3D10_TEXTURE2D_DESC DepthStencilDescription);				// Create the depth/stencil view using the depth/stencil buffer description passed
		void SetRenderTargetsToOutputMergerStage();												// Set render target view and the depth/stencil view to the output merger stage
		D3D10_VIEWPORT CreateViewPort();														// Creates a viewport
		void SetViewportToDirect3DDevice(D3D10_VIEWPORT Viewport);								// Sets the viewport to the Direct 3D device
		D3DX10_FONT_DESC CreateFontDescription();												// Creates a font description
		void CreateFontObject(D3DX10_FONT_DESC FontDescription);								// Create the font object used to draw text to the screen
		void SetWindowWidth(unsigned int PassedWindowWidth);
		void SetWindowHeight(unsigned int PassedWindowHeight);
		void ResizeDirectXWindow();																// When the window is resized re-initialise all appropriate elements of Direct 3D that are needed to render to the window
		virtual void UpdateScene(float DeltaTime);
		virtual void DrawScene();
		void SetSceneStateToPaused();
		void SetSceneStateToRunning();
		void SetSceneToMinimized();
		void SetSceneToMaximized();
		void RestoreScene();
		void WindowBeingResized();
		void WindowFinishedBeingResized();
		bool ScenePaused();
		bool SceneMinimized();
		bool SceneMaximized();
		bool SceneBeingResized();
		void StartTimer();
		void TickTimer();
		float GetDeltaTime();
	protected:
		HWND m_DirectX10ClassWindowHandle;	// Handle to the window 

		// Used in creating swap chain
		unsigned int m_WindowWidth;		// Window width
		unsigned int m_WindowHeight;	// Window height

		// Used as part of creating the swap chain and direct 3D device
		D3D10_DRIVER_TYPE m_Direct3DDriverType;		// Stores the device driver to use
		ID3D10Device* m_Direct3DDevice;				// Pointer to the Direct3D device
		IDXGISwapChain* m_SwapChain;				// Pointer to the swap chain
		MultiSample m_Direct3DInstanceMultiSample;	// A struct containing elements necessary for using multisampling

		// Used as part of creating the render target view
		ID3D10RenderTargetView* m_RenderTargetView;

		// Used to create the depth/stencil buffer
		ID3D10Texture2D* m_DepthStencilBuffer;
		ID3D10DepthStencilView* m_DepthStencilView;

		// Used to create the font object
		ID3DX10Font* m_Font;

		// Used in UpdateScene
		GameTimer m_Timer;
		std::wstring m_FrameStats;

		// Used in DrawScene()
		D3DXCOLOR m_ClearColor;

		//
		Direct3DSceneStates m_SceneStates;
	};
}
#endif