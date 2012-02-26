//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Box.h"
#include "Vertex.h"

Box::Box(ID3D10Device* Direct3DDevice):
m_NumberOfVertices(24),
m_NumberOfFaces(12)
{
	/*
	Vertex vertices[] = 
	{
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), RED},
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f), BLUE},
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f), GREEN},
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f), RED},
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f), BLUE},
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f), GREEN},
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f), RED},
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f), BLUE},
	};
	*/
	// Create vertex buffer
    Vertex v[24];

	// Fill in the front face vertex data.
 	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[17] = Vertex(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[18] = Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[21] = Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[22] = Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[23] = Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

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
	
	/*
	// Create indices array for box
	DWORD indices[] = {
		// front face 
		0, 1 ,2,
		0, 2, 3,

		// back face 
		4, 6, 5,
		4, 7, 6,

		// left face 
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	*/
	// Create the index buffer

	DWORD i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

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
 
Box::~Box()
{
}

void Box::Update(float DeltaTime)
{
	// Create tempoary translation matrix to concatinate with the world matrix
	D3DXMATRIX Translation;
	D3DXMatrixIdentity(&Translation);

	// If user presses left arrow
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 ) 
	{
		// Move left
		D3DXMatrixTranslation(&Translation, -5.0f*DeltaTime, 0.0f, 0.0f);
	}
	// If user presses right arrow
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 ) 
	{
		// Move right
		D3DXMatrixTranslation(&Translation, 5.0f*DeltaTime, 0.0f, 0.0f);
	}
	// If user presses up arrow
	if( GetAsyncKeyState(VK_UP) & 0x8000 ) 
	{
		// Move up
		D3DXMatrixTranslation(&Translation, 0.0f, 5.0f*DeltaTime, 0.0f);
	}
	// If user presses down arrow
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 ) 
	{
		// Move down
		D3DXMatrixTranslation(&Translation, 0.0f, -5.0f*DeltaTime, 0.0f);
	}

	// Concatinate matrices
	m_World *= Translation;
}

void Box::Draw(ID3D10Device* Direct3DDevice)
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

D3DXMATRIX Box::ReturnWorldMatrix()
{
	return m_World;
}

void Box::SetPosition(float x, float y, float z)
{
	// Create tempoary translation matrix to concatinate with the world matrix
	D3DXMATRIX Translation;
	D3DXMatrixIdentity(&Translation);

	D3DXMatrixTranslation(&Translation, x, y, z);

	// Concatinate matrices
	m_World *= Translation;
}