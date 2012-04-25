//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "..\\HeaderFiles\\Floor.h"
#include "..\\HeaderFiles\\Vertex.h"

Floor::Floor(ID3D10Device* Direct3DDevice):
m_NumberOfVertices(4),
m_NumberOfFaces(2)
{
	// Create vertex buffer
    Vertex v[4];

	// Draw floor
 	v[0] = Vertex(-5.0f,  0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[1] = Vertex( 5.0f,  0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[2] = Vertex(-5.0f,  0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[3] = Vertex( 5.0f,  0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

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

	// Create the index buffer

	DWORD i[6];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 1; i[4] = 3; i[5] = 2;

	// Create index buffer description 
	D3D10_BUFFER_DESC IndexBufferDesc;													
	IndexBufferDesc.Usage = D3D10_USAGE_IMMUTABLE;					// How the index buffer will be used
	IndexBufferDesc.ByteWidth = sizeof(DWORD) * m_NumberOfFaces*3;	// The size in bytes of the buffer
	IndexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;			// The type of information the buffer is storing
	IndexBufferDesc.CPUAccessFlags = 0;								// How will the CPU access the buffer
	IndexBufferDesc.MiscFlags = 0;

	// Used in the creation of the index buffer
	D3D10_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = i;								// Takes the starting address of the elements in the buffer

	HR( Direct3DDevice->CreateBuffer(&IndexBufferDesc,				// The index buffer deoscription
									 &IndexInitData,				// The starting address of the indices
									 &m_IndexBuffer) );				// The interface pointer to the new buffer
	
	D3DXMatrixIdentity(&m_World);
}
 
Floor::~Floor()
{
}

void Floor::Update(float DeltaTime)
{
}

void Floor::Draw(ID3D10Device* Direct3DDevice)
{
	UINT stride = sizeof(Vertex);								// Passed into IASetVertexBuffers
	UINT offset = 0;											// Passed into IASetVertexBuffers
	Direct3DDevice->IASetVertexBuffers(0,						// The input slot that the vertex buffer will be assigned to
									   1,						// Number of buffers we are assigning
									   &m_VertexBuffer,			// Pointer to the first element of an array of vertex buffers
									   &stride,					// Pointer to the first element of an array of strides, a stride is the size, in bytes, of an element in the corresponding vertex buffer
									   &offset);				// Pointer to the first element of an array of offsets, one for each vertex buffer. This is an offset, in bytes, from the start of the vertex buffer to the position in the vertex buffer from which the input assembly should start reading data.  

	Direct3DDevice->IASetIndexBuffer(m_IndexBuffer,				// The index buffer to use for drawing 
									 DXGI_FORMAT_R32_UINT,		// Specify the format of the indices
									 0);						// Offset, measured in bytes, from the start of the index buffer to the point to start reading from 

	Direct3DDevice->DrawIndexed(m_NumberOfFaces*3,				// Index count
								0,								// Start index location
								0);								// Start vertex location 

}

D3DXMATRIX Floor::ReturnWorldMatrix()
{
	return m_World;
}

void Floor::SetPosition(float x, float y, float z)
{
	// Create tempoary translation matrix to concatinate with the world matrix
	D3DXMATRIX Translation;
	D3DXMatrixIdentity(&Translation);

	D3DXMatrixTranslation(&Translation, x, y, z);

	// Concatinate matrices
	m_World *= Translation;
}

void Floor::ScaleXZ(float x, float z)
{
	// Create tempoary translation matrix to concatinate with the world matrix
	D3DXMATRIX Scale;
	D3DXMatrixIdentity(&Scale);

	D3DXMatrixScaling(&Scale, x, 0.0f, z);

	// Concatinate matrices
	m_World *= Scale;
}