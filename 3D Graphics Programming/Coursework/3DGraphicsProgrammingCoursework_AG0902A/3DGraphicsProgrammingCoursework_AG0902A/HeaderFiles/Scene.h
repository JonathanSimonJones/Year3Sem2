#ifndef SCENE_H
#define SCENE_H

#include "DirectX10Class.h"
#include "d3dUtil.h"
#include "Box.h"
#include "Camera.h"
#include "Light.h"
#include "Sky.h"
#include "Terrain.hpp"
#include "Rendertextureclass.h"

class OrthoWindowClass;
class TextureShaderClass;
class HorizontalBlurShaderClass;

class Scene : public MyD3D10Code::Direct3D10Class
{
public:
	Scene(HWND TheWindow, unsigned int WindowWidth, unsigned int WindowHeight);
	~Scene();

	void UpdateScene(float dt);
	void DrawScene();
private:
	void RenderUsingPostProcessing();
	void RenderToTexture();
	void RenderToBackBuffer();
	void RenderHorizontalBlurToTexture();
private:
	D3DXMATRIX m_WorldViewProjection;

	void BuildFX();

	ID3D10Effect* m_FX;
	ID3D10EffectTechnique* m_Tech;
	ID3D10EffectMatrixVariable* m_fxWVPVar;

	void BuildVertexLayouts();

	ID3D10InputLayout* m_VertexLayout;

	float m_Theta;
	float m_Phi;

	Box m_Box;

	// Added during texturing
	ID3D10ShaderResourceView* m_DiffuseMapRV;				// The resource view for the crate texture
	ID3D10EffectShaderResourceVariable* m_fxDiffuseMapVar;	// Used to update Diffuse map in tex.fx 

	// Added during lighting
	ID3D10EffectVariable* m_FxEyePosVar;
	ID3D10EffectVariable* m_FxLightVar;
	ID3D10ShaderResourceView* m_SpecMapRV;
	ID3D10EffectShaderResourceVariable* m_FxSpecMapVar;
	ID3D10EffectMatrixVariable* m_FxTexMatVar;
	ID3D10EffectMatrixVariable* m_FxBoxWorldVar;

	Light m_ParallelLight;

	// Added during implementing skybox
	Sky m_Sky;

	ID3D10ShaderResourceView* m_EnvironmentMapRV;

	Terrain m_Terrain;

	RenderTextureClass m_RenderTexture;

	OrthoWindowClass *m_FullScreenWindow;

	TextureShaderClass* m_TextureShader;

	HorizontalBlurShaderClass *m_HorizontalBlurShader;

	RenderTextureClass *m_HorizontalBlurTexture;
};

#endif 