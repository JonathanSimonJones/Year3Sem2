#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "VertexList.h"

class Triangle
{
public:
	Triangle();
	~Triangle();
private:
	ID3D10Effect* m_FX;
	ID3D10EffectTechnique*  m_Technique;
	ID3D10InputLayout* m_VertexLayout;
};

#endif