// Terrain.h

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "d3dUtil.h"
#include <vector>
#include "Vertex.h"

class Terrain
{
public:
	Terrain();
	~Terrain();
	void Initialise(ID3D10Device* Direct3DDevice, int n);
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	void Generate(ID3D10Device* Direct3DDevice);
	void Draw(ID3D10Device* Direct3DDevice);
private:
	void CreateTerrainVertices(int n_);
	void CreateIndices(std::vector<DWORD>* indices, int n_);
private:
	int mNumVertices;
	DWORD  mNumFaces; // 2 per quad

	std::vector<Vertex> vertices;

	int scale;
};

#endif 