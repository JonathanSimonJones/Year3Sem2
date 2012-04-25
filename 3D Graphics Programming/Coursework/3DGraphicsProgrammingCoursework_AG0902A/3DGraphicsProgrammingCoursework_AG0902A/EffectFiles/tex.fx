// Tex.fx

#include "lighthelper.fx"

cbuffer cbPerFrame
{
	Light gLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gBoxWorld;
	float4x4 gWVP;
	float4x4 gTexMat;
};

// Cannot add non-numeric values to constant buffers
Texture2D gDiffuseMap;
Texture2D gSpecMap;		// Used in lighting

SamplerState gSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState WireFrame
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

struct VS_IN
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_OUT
{
	float4 PosH : SV_POSITION;		// System value position
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	// Apply world matrix to vert position
	vOut.PosW		= mul(float4(vIn.PosL, 1.0f), gBoxWorld );

	// Apply world matrix to vert noraml
	vOut.NormalW	= mul(float4(vIn.NormalL, 0.0f), gBoxWorld ); 

	// Transform to homogenous clip space
	vOut.PosH = mul(float4(vIn.PosL, 1.0f), gWVP);

	// Apply tex matrix
	vOut.Tex = mul(float4(vIn.Tex, 0.0f, 1.0f), gTexMat);

	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target  // SV_Target indicates the returned value should be in render target format
{
	// Get materials from texture map
	float4 diffuse	= gDiffuseMap.Sample(gSampler, pIn.Tex);	// Apply colour from texture to vert
	float4 spec		= gSpecMap.Sample( gSampler, pIn.Tex);		// Apply specular value to vert

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalise it
	float3 normalW = normalize(pIn.NormalW);

	// Compute the lit colour for this pixel
	SurfaceInfo v = {pIn.PosW, normalW, diffuse, spec};
	float3 litColour = ParallelLight(v, gLight, gEyePosW);

	// Return pixel fragment colour
	return float4(litColour, diffuse.a);
}

technique10 ColorTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );

		//SetRasterizerState(WireFrame);
	}
}