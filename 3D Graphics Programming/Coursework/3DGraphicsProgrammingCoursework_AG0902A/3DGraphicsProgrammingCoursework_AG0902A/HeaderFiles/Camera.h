#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

// Simple camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be obtained.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
class Camera
{
public:
	Camera();
	~Camera();

	D3DXVECTOR3& position();

	D3DXMATRIX view()const;
	D3DXMATRIX proj()const;

	void setLens(float fovY, float aspect, float zn, float zf);

	void strafe(float d);
	void walk(float d);

	void Up(float d);
	void Down(float d);

	void pitch(float angle);
	void rotateY(float angle);

	void rebuildView();

	void Update(float dt);

	void SetLookAt(const float x, const float y, const float z);

private:
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	const float m_SpeedOfMovement;
};

Camera& GetCamera();

#endif // DEMO_CAMERA_H