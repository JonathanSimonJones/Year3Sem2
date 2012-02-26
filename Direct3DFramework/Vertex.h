#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"

struct Vertex
{
	Vertex() {}
	Vertex( float x,
			float y,
			float z,
			float r,
			float g,
			float b,
			float u,
			float v )
			:
			pos(x,y,z),
			normal(r,g,b),
			textureCoords(u,v) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2	textureCoords;
};

#endif // VERTEX_H