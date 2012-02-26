#ifndef VERTEX_LIST_H
#define VERTEX_LIST_H

#include "d3dUtil.h"
#include <string>

namespace VertexList
{
	class Vertex_Pos_Col
	{
	private:
		D3DXVECTOR3 Position;
		D3DXCOLOR Colour;
	};

	class Vertex_Pos_Col_Tex
	{
	private:
		D3DXVECTOR3 Position;
		D3DXCOLOR Colour;
		D3DXVECTOR2 Texture;
	};

	const D3D10_INPUT_ELEMENT_DESC Vertex_Pos_Colour_Description[] = 
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	const D3D10_INPUT_ELEMENT_DESC Vertex_Pos_Colour_Texture_Description[] =
	{
		{"POSITION"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURECOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};


	const D3D10_INPUT_ELEMENT_DESC *ReturnInputElementDescription(int VertexNum);
}

#endif