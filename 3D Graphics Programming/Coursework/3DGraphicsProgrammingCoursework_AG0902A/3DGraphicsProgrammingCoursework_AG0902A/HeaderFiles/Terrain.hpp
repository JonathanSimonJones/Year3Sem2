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
	void Initialise(ID3D10Device* Direct3DDevice, int gridSize);
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	void Generate();
	void Draw(ID3D10Device* Direct3DDevice);
private:
	void CreateTerrainVertices();
	void CreateIndices(std::vector<DWORD>* indices);
private:
	int mNumVertices;
	DWORD  mNumFaces; // 2 per quad

	std::vector<Vertex> vertices;

	int scale;
	int sizeOfGrid;
};

#endif 