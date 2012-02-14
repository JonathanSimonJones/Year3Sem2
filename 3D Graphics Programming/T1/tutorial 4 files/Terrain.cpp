// Terrain.cpp

#include "Terrain.hpp"
#include "Line.hpp"
#include <cstdlib>		// rand(), srand()
#include <time.h>		// time()

Terrain::Terrain():
mNumVertices(3),
mNumFaces(1),
scale(1)
{
}
	
Terrain::~Terrain()
{
}

void Terrain::Initialise(ID3D10Device* Direct3DDevice, int n)
{
	// Make sure n is not less than one as that value cannot be calculated using the current formula's
	if(n < 1)
	{
		n = 1;
	}
	
	// Calculate variables 
	int NumberOfBlocks = (n * n);
	mNumVertices = (n+1) * (n+1);
	mNumFaces = (NumberOfBlocks * 2);
	int mNumIndices = (6 * NumberOfBlocks);

	// Create vertices
	CreateTerrainVertices( n );

    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(vertices[0]) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(Direct3DDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	// Create indices 
	std::vector<DWORD> indices;
	CreateIndices( &indices, n);
	
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

void Terrain::Generate(ID3D10Device* Direct3DDevice)
{
	srand ( time(NULL) );
	
	for(int i = 0; i < 5; i++)
	{
		float gradient = rand() % 20;
		float yIntercept = rand() % 19;
		
		Line TempLine(gradient, yIntercept);

		for( int i = 0; i < vertices.size(); i++)
		{
			if( (TempLine.ReturnZ(vertices[i].pos.x)) > (vertices[i].pos.z) )
			{
				vertices[i].pos.y += 0.2f;
			}
		}
	}
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
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(vertices[0]) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(Direct3DDevice->CreateBuffer(&vbd, &vinitData, &mVB));

}

void Terrain::Draw(ID3D10Device* Direct3DDevice)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Direct3DDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	Direct3DDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	Direct3DDevice->DrawIndexed(mNumFaces*3, 0, 0);
}

void Terrain::CreateTerrainVertices( int n_)
{
	// Create vertex list 
	for( float z = 0; z < (n_+1); ++z)
	{
		for( float x = 0; x < (n_+1); ++x)
		{
			vertices.push_back( Vertex(x, 0.0f, z));
		}
	}
}

void Terrain::CreateIndices( std::vector<DWORD>* indices_, int n_)
{
	int NumOfRowsComplete = 0;
	
	for( int RowNum = 0; RowNum < (n_); ++RowNum)
	{
		for( int ColNum = 0; ColNum < (n_); ++ColNum)
		{
			if( !( (RowNum + ColNum) % 2) )
			{
				indices_->push_back( ( RowNum * n_ ) + ColNum + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum + 1 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum + n_ + 1 + NumOfRowsComplete);

				indices_->push_back( ( RowNum * n_ ) + ColNum + n_ + 2 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum + n_ + 1 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum  + 1 + NumOfRowsComplete);
			}
			else
			{
				indices_->push_back( ( RowNum * n_ ) + ColNum + n_ + 2 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum + n_ + 1 + NumOfRowsComplete);

				indices_->push_back( ( RowNum * n_ ) + ColNum + NumOfRowsComplete);

				indices_->push_back( ( RowNum * n_ ) + ColNum + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum + 1 + NumOfRowsComplete);
			
				indices_->push_back( ( RowNum * n_ ) + ColNum + n_ + 2 + NumOfRowsComplete);
			}
		}
		NumOfRowsComplete++;
	}
}