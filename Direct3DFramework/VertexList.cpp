#include "VertexList.h"

namespace VertexList
{
	const D3D10_INPUT_ELEMENT_DESC *ReturnInputElementDescription(int VertexNum)
	{
		if(VertexNum == 1)
		{
			return Vertex_Pos_Colour_Description;
		}
		if(VertexNum == 2)
		{
			return Vertex_Pos_Colour_Texture_Description;
		}
	}
}