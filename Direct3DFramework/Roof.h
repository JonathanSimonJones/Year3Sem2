//=======================================================================================
// by Jonathan Jones 2012, based on Box.h by Frannk Luna
//=======================================================================================

#ifndef ROOF_H
#define ROOF_H

#include "d3dUtil.h"

class Roof
{
public:
	Roof(ID3D10Device* Direct3DDevice);
	~Roof();

	void Update(float DeltaTime);
	void Draw(ID3D10Device* Direct3DDevice);
	D3DXMATRIX ReturnWorldMatrix();
	void SetPosition(float x, float y, float z);
private:
	int m_NumberOfVertices;
	DWORD m_NumberOfFaces;
	ID3D10Buffer* m_VertexBuffer;
	D3DXMATRIX m_World;
};


#endif // ROOF_H
