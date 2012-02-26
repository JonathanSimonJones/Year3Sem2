//=======================================================================================
// by Jonathan Jones 2012, based on Box.h by Frannk Luna
//=======================================================================================

#ifndef FLOOR_H
#define FLOOR_H

#include "d3dUtil.h"

class Floor
{
public:
	Floor(ID3D10Device* Direct3DDevice);
	~Floor();

	void Update(float DeltaTime);
	void Draw(ID3D10Device* Direct3DDevice);
	D3DXMATRIX ReturnWorldMatrix();
	void SetPosition(float x, float y, float z);
	void ScaleXZ(float x, float z);
private:
	int m_NumberOfVertices;
	DWORD m_NumberOfFaces;
	ID3D10Buffer* m_VertexBuffer;
	ID3D10Buffer* m_IndexBuffer;
	D3DXMATRIX m_World;
};

#endif 