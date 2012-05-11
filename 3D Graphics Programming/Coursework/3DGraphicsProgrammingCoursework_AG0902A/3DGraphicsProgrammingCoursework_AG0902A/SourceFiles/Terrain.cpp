// Terrain.cpp

#include "..\\HeaderFiles\\Terrain.hpp"
#include "..\\HeaderFiles\\Line.hpp"
#include <cstdlib>		// rand(), srand()
#include <time.h>		// time()

Terrain::Terrain()
: mNumVertices(3)
, mNumFaces(1)
, scale(1)
, sizeOfGrid(0)
{
}
	
Terrain::~Terrain()
{
}

void Terrain::Initialise(ID3D10Device* Direct3DDevice, int gridSize)
{
	// Set size of grid
	sizeOfGrid = gridSize;

	// Make sure n is not less than one as that value cannot be calculated using the current formula's
	if(sizeOfGrid < 1)
	{
		sizeOfGrid = 1;
	}
	
	// Calculate variables 
	int NumberOfBlocks = (sizeOfGrid * sizeOfGrid);
	mNumVertices = (sizeOfGrid+1) * (sizeOfGrid+1);
	mNumFaces = (NumberOfBlocks * 2);
	int mNumIndices = (6 * NumberOfBlocks);

	// Create vertices
	CreateTerrainVertices();

    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(vertices[0]) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
    
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(Direct3DDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	// Create indices 
	std::vector<DWORD> indices;
	CreateIndices( &indices);
	
	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(indices[0]) * mNumFaces*3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = &indices[0];

	HR(Direct3DDevice->CreateBuffer(&ibd, &IndexInitData, &mIB));
}

void Terrain::Generate()
{
	/*
	srand ( time(NULL) );
	
	int test = 0;
	bool negative = false;

	for(int i = 0; i < 25; i++)
	{
		test = rand() % 100;
		
		if ( test < 50)
		{
			negative = true;
		}
		else 
		{
			negative = false;
		}
		
		float gradient = rand() % 20;
		float yIntercept = rand() % 19;

		if (negative == true)
		{
			gradient *= -1;
		}
		
		Line TempLine(gradient, yIntercept);

		for( int i = 0; i < vertices.size(); i++)
		{
			if( (TempLine.ReturnZ(vertices[i].pos.x)) > (vertices[i].pos.z) )
			{
				vertices[i].pos.y += 0.2f;
			}
		}
	}
	*/
	/*
	Line TempLine2(-2.0f, 25.0f);

	for( int i = 0; i < vertices.size(); i++)
	{
		if( (TempLine2.ReturnZ(vertices[i].pos.x)) > (vertices[i].pos.z) )
		{
			vertices[i].pos.y += 1.0f;
		}
	}
	*/

	// Seed rand
	srand ( time(NULL) );

	// Set a scale ratio for vert postition
	const float genScale = 1.0f * scale;

	// Generate 2 rand positions on the grid
	const float x1 = static_cast<float>(rand() % sizeOfGrid);
	const float y1 = static_cast<float>(rand() % sizeOfGrid);
	const float x2 = static_cast<float>(rand() % sizeOfGrid);
	const float y2 = static_cast<float>(rand() % sizeOfGrid);

	// If they are the same then exit
	if(x1 == x2 && y1 == y2)
	{
		return;
	}

	// 
	Vertex *vert(0);

	HRESULT result;
	result = mVB->Map(D3D10_MAP_WRITE_NO_OVERWRITE, 0, (void**)&vert);

	float j = 0.0f;

	for( int i = 0; i < vertices.size(); i++)
	{ 
		const float z = (x1 - x2) * (y1 - vertices[i].pos.z) - (x1 - vertices[i].pos.x) * (y1 - y2);

		vertices[i].pos.y += (z < 0 ? .1 : -.1) * genScale;
		vert[i].pos.y = vertices[i].pos.y;
	}

	mVB->Unmap();
}

void Terrain::Draw(ID3D10Device* Direct3DDevice)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3DDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	Direct3DDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	Direct3DDevice->DrawIndexed(mNumFaces*3, 0, 0);
}

void Terrain::CreateTerrainVertices()
{
	// Create vertex list 
	for( float z = 0; z < (sizeOfGrid+1); ++z)
	{
		for( float x = 0; x < (sizeOfGrid+1); ++x)
		{
			vertices.push_back( Vertex(x, 0.0f, z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
		}
	}
}

void Terrain::CreateIndices( std::vector<DWORD>* indices_)
{
	int NumOfRowsComplete = 0;
	
	for( int RowNum = 0; RowNum < (sizeOfGrid); ++RowNum)
	{
		for( int ColNum = 0; ColNum < (sizeOfGrid); ++ColNum)
		{
			if( !( (RowNum + ColNum) % 2) )
			{
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + 1 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + sizeOfGrid + 1 + NumOfRowsComplete);

				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + sizeOfGrid + 2 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + sizeOfGrid + 1 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum  + 1 + NumOfRowsComplete);
			}
			else
			{
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + sizeOfGrid + 2 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + sizeOfGrid + 1 + NumOfRowsComplete);

				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + NumOfRowsComplete);

				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + 1 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * sizeOfGrid ) + ColNum + sizeOfGrid + 2 + NumOfRowsComplete);
			}
		}
		NumOfRowsComplete++;
	}
}