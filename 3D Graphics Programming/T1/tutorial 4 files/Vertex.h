#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"

struct Vertex {
	Vertex() {}
	Vertex(float x, float y, float z): pos(x,y,z), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	D3DXVECTOR3 pos;
	D3DXCOLOR   color;
};
 
#endif // VERTEX_H