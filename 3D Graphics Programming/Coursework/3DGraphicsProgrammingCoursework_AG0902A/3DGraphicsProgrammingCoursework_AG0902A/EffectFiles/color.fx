// Colour.fx

cbuffer cbPerObject
{
	float4x4 gWVP;
};

RasterizerState WireFrame
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

void VS(float3 iPosL : POSITION,
		float4 iColor : COLOR,
		out float4 oPosH : SV_POSITION,		// System value position
		out float4 oColor : COLOR)
{
	// Transform to homogenous clip space
	oPosH = mul(float4(iPosL, 1.0f), gWVP);

	// Just pass vertex colour into the pixel shader
	oColor = iColor;
}

float4 PS(float4 posH	: SV_POSITION,
		  float4 color	: COLOR) : SV_Target  // SV_Target indicates the returned value should be in render target format
{
	return float4(1.0f,0.0f,1.0f,1.0f);
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