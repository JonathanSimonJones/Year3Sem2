#include "..\\HeaderFiles\\Scene.h"
#include "..\\HeaderFiles\\Effects.h"
#include "..\\HeaderFiles\\InputLayouts.h"
#include "..\\HeaderFiles\\TextureMgr.h"
#include "..\\HeaderFiles\\orthowindowclass.h"
#include "..\\HeaderFiles\\textureshaderclass.h"
#include "..\\HeaderFiles\\horizontalblurshaderclass.h"

Scene::Scene(HWND TheWindow, unsigned int WindowWidth, unsigned int WindowHeight)
: MyD3D10Code::Direct3D10Class(TheWindow, WindowWidth, WindowHeight)
, m_FX(0)
, m_Tech(0)
, m_fxWVPVar(0)
, m_VertexLayout(0)
, m_Theta(0.0f)
, m_Phi(PI*0.25f)
, m_Box(m_Direct3DDevice)
// Delete 5 below variables later from init list
, m_FxEyePosVar(0)
, m_FxLightVar(0)
, m_SpecMapRV(0)
, m_FxSpecMapVar(0)
, m_FxTexMatVar(0)
, m_FxBoxWorldVar(0)
, m_EnvironmentMapRV(0)
, m_RenderTexture()
, m_FullScreenWindow(0)
, m_TextureShader(0)
, m_BlurActive(true)
{
	// Initialise world view projection matrix
	D3DXMatrixIdentity(&m_WorldViewProjection);

	// Set the position of the camera
	GetCamera().position() = D3DXVECTOR3(0.0f, 10.0f, -40.0f);

	// Set the camera up
	float aspect = (float)m_WindowWidth/m_WindowHeight;		// Used in D3DXMatrixPerspectiveFovLH()
	GetCamera().setLens(0.25f*PI, aspect, 0.5f, 1000.0f);

	// Create effects and vertex layouts 
	BuildFX();
	BuildVertexLayouts();
	fx::InitAll(m_Direct3DDevice);
	InputLayout::InitAll(m_Direct3DDevice);

	// Initialise texture manager
	GetTextureMgr().init(m_Direct3DDevice);

	// Create crate resource view
	HR( D3DX10CreateShaderResourceViewFromFile(m_Direct3DDevice,	// The device to create the texture with
											   L"Images\\GreyBricks.jpg",	// The name of the file
											   0,					// Optional image info, specify null to use source dimensions and to creat a full chain of mipmap levels
											   0,					// Used to spawn a new thread to load resources, pPump
											   &m_DiffuseMapRV,		// The pointer that is used to interface with the new resource
											   0) );				// Used with pPump

	// Create specular map resource view
	HR( D3DX10CreateShaderResourceViewFromFile(m_Direct3DDevice,	// The device to create the texture with
											   L"Images\\defaultspec.dds",	// The name of the file to use
											   0,					// Optional image info, specify null to use source dimensions and to creat a full chain of mipmap levels 
											   0,					// Used to spawn a new thread to load resources, pPump
											   &m_SpecMapRV,		// The pointer that is used to interface with the new resource
											   0) );				// Used with pPump

	// Set the sky texture up
	m_EnvironmentMapRV	= GetTextureMgr().createCubeTex(L"Images\\grassenvmap1024.dds");

	// Set up parallel light
	m_ParallelLight.dir		 = D3DXVECTOR3(0.57735f, -0.57735f, -0.25f);	// Reveresed
	m_ParallelLight.ambient	 = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	m_ParallelLight.diffuse	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ParallelLight.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// Set box position
	m_Box.SetPosition(0.0f, 1.0f, 0.0f);
	
	// Initialise sky
	m_Sky.init(m_Direct3DDevice, m_EnvironmentMapRV, 5000.0f);

	m_Terrain.Initialise(m_Direct3DDevice, 45);

	// Initialise render texture
	m_RenderTexture.Initialize(m_Direct3DDevice, WindowWidth, WindowHeight);

	// Create the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;

	// Initialize the full screen ortho window object.
	m_FullScreenWindow->Initialize(m_Direct3DDevice, WindowWidth, WindowHeight);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;

	// Initialize the texture shader object.
	m_TextureShader->Initialize(m_Direct3DDevice, TheWindow);

	// Create the horizontal blur shader object.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	
	bool result;
	// Initialize the horizontal blur shader object.
	result = m_HorizontalBlurShader->Initialize(m_Direct3DDevice, TheWindow);
	if(!result)
	{
		MessageBox(TheWindow, L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
	}

	// Create the horizontal blur render to texture object.
	m_HorizontalBlurTexture = new RenderTextureClass;

	// Initialize the horizontal blur render to texture object.
	result = m_HorizontalBlurTexture->Initialize(m_Direct3DDevice, WindowWidth, WindowHeight);
	if(!result)
	{
		MessageBox(TheWindow, L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
	}

}

Scene::~Scene()
{
	// If device still exist 
	if(m_Direct3DDevice)
	{
		// Clear the device
		m_Direct3DDevice->ClearState();
	}

	// Release resources
	ReleaseCOM(m_FX);
	ReleaseCOM(m_VertexLayout);
	ReleaseCOM(m_DiffuseMapRV);
	ReleaseCOM(m_SpecMapRV);
	fx::DestroyAll();
	InputLayout::DestroyAll();

	// Release the horizontal blur shader object.
	if(m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the horizontal blur render to texture object.
	if(m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}
}

void Scene::BuildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined (DEBUG) || (_DEBUG)
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif 

	ID3D10Blob* compilationErrors = 0;

	HRESULT hr = 0;
	hr =  D3DX10CreateEffectFromFile(L"EffectFiles\\tex.fx",			// The name of the effects file to compile
									 0,						// pDefines
									 0,						// pInlcude
									 "fx_4_0",				// The version of the shader we are using
									 shaderFlags,			// Specify how the shader code will be complied
									 0,						// FXflags
									 m_Direct3DDevice,		// A pointer to the direct device
									 0,						// pEffectPool
									 0,						// pPump
									 &m_FX,					// Returns a pointer to the created object
									 &compilationErrors,	// Returns a pointer to a string containing the comiplation errors
									 0						// pttResult, used with pPump
									 );

	if( FAILED(hr) )
	{
		if( compilationErrors )
		{
			// Print out any errors
			MessageBoxA( 0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	}

	// Set the technique to color tech
	m_Tech = m_FX->GetTechniqueByName("ColorTech");

	// Setup variable pointers to the designated name in effect files
	m_fxWVPVar = m_FX->GetVariableByName("gWVP")->AsMatrix();	// Set the effect file WVP matrix to 
	m_fxDiffuseMapVar = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_FxSpecMapVar = m_FX->GetVariableByName("gSpecMap")->AsShaderResource();
	m_FxEyePosVar = m_FX->GetVariableByName("gEyePosW");
	m_FxLightVar = m_FX->GetVariableByName("gLight");
	m_FxTexMatVar = m_FX->GetVariableByName("gTexMat")->AsMatrix();
	m_FxBoxWorldVar = m_FX->GetVariableByName("gBoxWorld")->AsMatrix();
}

void Scene::BuildVertexLayouts()
{
	// Create the vertex input layout 
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Calculates the number of elements
	int numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;												// Used to store a pass description
	m_Tech->GetPassByIndex(0)->GetDesc(&PassDesc);							// Get vertex shader signature to pass to input layout
	HR(m_Direct3DDevice->CreateInputLayout(vertexDesc,						// Name of Input element descritption
										   numElements,						// The number of elements in the array
										   PassDesc.pIAInputSignature,		// Pass in the vertex shader signature so that Direct3D can create mapping from the vertex structure to the shader inputs at creation time, instead of at draw time
										   PassDesc.IAInputSignatureSize,	// And pass in its size
										   &m_VertexLayout					// Returns a pointer to created input layout
										   ) );
}

void Scene::UpdateScene(float dt)
{
	// Default update 
	MyD3D10Code::Direct3D10Class::UpdateScene(dt);

	// Update the camera
	GetCamera().Update(dt);

	// Update the box 
	m_Box.Update(dt);

	if(GetAsyncKeyState('B') & 0x8000)	
	{
		m_BlurActive = true;
	}

	if(GetAsyncKeyState('N') & 0x8000)	
	{
		m_BlurActive = false;
	}

	m_Terrain.Generate();
}

void Scene::DrawScene()
{
	MyD3D10Code::Direct3D10Class::DrawScene();

	// Render the post process scene
	RenderUsingPostProcessing();

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	m_Direct3DDevice->RSSetState(0);										// Set the rasterization state to the default state
	m_Font->DrawText(0, m_FrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	
	// Took this out and put it in Coloured cube DrawScene
	m_SwapChain->Present(0, 0);
}

void Scene::RenderUsingPostProcessing()
{
	Scene::RenderToTexture();

	if(m_BlurActive)
	{
		Scene::RenderHorizontalBlurToTexture();
	}

	Scene::RenderToBackBuffer();
}

void Scene::RenderToTexture()
{
	// Set the render target to be the render to texture 
	m_RenderTexture.SetRenderTarget(m_Direct3DDevice);

	// Clear the render to texture
	m_RenderTexture.ClearRenderTarget(m_Direct3DDevice, 0.0f, 1.0f, 0.0f, 1.0f);

	// Render the scene
	// Restore Default states, input layout and primitive topology 
	// as m_Font->DrawText changes them. Note that we can restore
	// the default states by passing null
	m_Direct3DDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_Direct3DDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	m_Direct3DDevice->IASetInputLayout(m_VertexLayout);									// Bind input layout to the device
	m_Direct3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// Set primitive topology to triangle list

	D3DXMATRIX view = GetCamera().view();
	D3DXMATRIX proj = GetCamera().proj();

	/////////////////////////
	// Set constants 
	/////////////////////////
	m_fxDiffuseMapVar->SetResource(m_DiffuseMapRV);

	m_FxSpecMapVar->SetResource(m_SpecMapRV);
	m_FxEyePosVar->SetRawValue(&GetCamera().position(), 0, sizeof(D3DXVECTOR3) );
	m_FxLightVar->SetRawValue(&m_ParallelLight, 0, sizeof(Light) );
	
	D3DXMATRIX texMat;
	D3DXMatrixIdentity(&texMat);
	m_FxTexMatVar->SetMatrix((float*)&texMat);
	
	///////////////////////////
	// End constants 
	///////////////////////////

	D3D10_TECHNIQUE_DESC techDescription;
	m_Tech->GetDesc( &techDescription );

	for( UINT p = 0; p < techDescription.Passes; ++p)
	{
		//m_Tech->GetPassByIndex( p )->Apply(0);
		ID3D10EffectPass* pass = m_Tech->GetPassByIndex(p);
		
		m_fxDiffuseMapVar->SetResource(m_DiffuseMapRV);
		m_WorldViewProjection = m_Box.ReturnWorldMatrix()*view*proj;
		m_fxWVPVar->SetMatrix( (float*)&m_WorldViewProjection );		// Updates WVP matrix in the internal cache of the effect object
		m_FxBoxWorldVar->SetMatrix((float*)&m_Box.ReturnWorldMatrix() );
		pass->Apply(0);
		//m_Box.Draw(m_Direct3DDevice);

		m_Terrain.Draw(m_Direct3DDevice);
	}
	m_Sky.draw();	// Draw the sky
}

void Scene::RenderHorizontalBlurToTexture()
{
	float screenSizeX;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)m_HorizontalBlurTexture->GetTextureWidth();

	// Set the render target to be the render to texture.
	m_HorizontalBlurTexture->SetRenderTarget(m_Direct3DDevice);

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_Direct3DDevice, 0.0f, 0.0f, 0.0f, 1.0f);

	Direct3D10Class::TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3DDevice);

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	m_HorizontalBlurShader->Render(m_Direct3DDevice, 
									m_FullScreenWindow->GetIndexCount(), 							
									m_RenderTexture.GetShaderResourceView(), 
									screenSizeX);

	// Turn the z buffer back on
	Direct3D10Class::TurnZBufferOn();
}

void Scene::RenderToBackBuffer()
{
	D3DXMATRIX worldMatrix;

	// Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&worldMatrix);
	
	Direct3D10Class::ResetDefaultRenderTargets();

	Direct3D10Class::TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3DDevice);

	if(m_BlurActive)
	{
		// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
		m_TextureShader->Render(m_Direct3DDevice, 
								m_FullScreenWindow->GetIndexCount(), 
								m_HorizontalBlurTexture->GetShaderResourceView());
	}
	else
	{
		// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
		m_TextureShader->Render(m_Direct3DDevice, 
								m_FullScreenWindow->GetIndexCount(),
								m_RenderTexture.GetShaderResourceView());
	}

	// Turn the z buffer back on
	Direct3D10Class::TurnZBufferOn();
}
