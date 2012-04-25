//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "..\\HeaderFiles\\Roof.h"
#include "..\\HeaderFiles\\Vertex.h"

Roof::Roof(ID3D10Device* Direct3DDevice):
m_NumberOfVertices(12),
m_NumberOfFaces(4)
{
	// Create vertex buffer
    Vertex v[12];

	// Fill in the front face vertex data.
 	v[0] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex( 0.0f,  2.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);
	v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face 
	v[3] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[4] = Vertex( 0.0f,  2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f);
	v[5] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	// Fill in the right face
	v[6] = Vertex( 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[7] = Vertex( 0.0f, 2.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f);
	v[8] = Vertex( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the left face
	v[9]  = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[10] = Vertex( 0.0f, 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.5f, 0.0f);
	v[11] = Vertex(-1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);


	// Create the vertex buffer descritpion
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;						// Specify how the vertex buffer will be used, currently it sets to never change
	vbd.ByteWidth = sizeof(Vertex) * m_NumberOfVertices;	// The size in bytes of the vertex buffer we are going to create
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;				// Specify the type of information the buffer is storing
	vbd.CPUAccessFlags = 0;									// Describe how the CPU will access the vertex buffer description
	vbd.MiscFlags = 0;										
	
	// Used in the creation of the vertex buffer
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;							// Takes the starting address of the elements in the buffer
	
	// Create the vertex buffer
	HR(Direct3DDevice->CreateBuffer(&vbd,				// The buffer description
									&vinitData,			// ?
									&m_VertexBuffer));	// The pointer which stores the path to the new buffer
	
	D3DXMatrixIdentity(&m_World);
}
 
Roof::~Roof()
{
}

void Roof::Update(float DeltaTime)
{
}

void Roof::Draw(ID3D10Device* Direct3DDevice)
{
	UINT stride = sizeof(Vertex);								// Passed into IASetVertexBuffers
	UINT offset = 0;											// Passed into IASetVertexBuffers
	Direct3DDevice->IASetVertexBuffers(0,						// The input slot that the vertex buffer will be assigned to
									   1,						// Number of buffers we are assigning
									   &m_VertexBuffer,			// Pointer to the first element of an array of vertex buffers
									   &stride,					// Pointer to the first element of an array of strides, a stride is the size, in bytes, of an element in the corresponding vertex buffer
									   &offset);				// Pointer to the first element of an array of offsets, one for each vertex buffer. This is an offset, in bytes, from the start of the vertex buffer to the position in the vertex buffer from which the input assembly should start reading data.  

	Direct3DDevice->Draw(m_NumberOfVertices,0);

}

D3DXMATRIX Roof::ReturnWorldMatrix()
{
	return m_World;
}

void Roof::SetPosition(float x, float y, float z)
{
	// Create tempoary translation matrix to concatinate with the world matrix
	D3DXMATRIX Translation;
	D3DXMatrixIdentity(&Translation);

	D3DXMatrixTranslation(&Translation, x, y, z);

	// Concatinate matrices
	m_World *= Translation;
}