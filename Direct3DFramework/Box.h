//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
// Added to by Jonathan Jones 2012
//=======================================================================================

#ifndef BOX_H
#define BOX_H

#include "d3dUtil.h"

class Box
{
public:
	Box(ID3D10Device* Direct3DDevice);
	~Box();

	void Update(float DeltaTime);
	void Draw(ID3D10Device* Direct3DDevice);
	D3DXMATRIX ReturnWorldMatrix();
	void SetPosition(float x, float y, float z);
private:
	int m_NumberOfVertices;
	DWORD m_NumberOfFaces;
	ID3D10Buffer* m_VertexBuffer;
	ID3D10Buffer* m_IndexBuffer;
	D3DXMATRIX m_World;
};


#endif // BOX_H
