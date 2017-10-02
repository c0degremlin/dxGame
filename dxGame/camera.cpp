#include "camera.h"
#include "graphics.h"

OrthoCamera::OrthoCamera()
{
	p_position = D3DXVECTOR2(0.0f,0.0f);
	p_zoom = 1.0;
	p_maxZoom = 0.0;
	p_minZoom = 100.0;
	p_rectWidth = WINDOW_WIDTH;
	p_rectHeight = WINDOW_HEIGHT;
}

OrthoCamera::~OrthoCamera()
{}

void OrthoCamera::Zoom(double zoom)
{
	double newZoom = p_zoom * zoom;
	if(newZoom > p_minZoom)
	{
		p_zoom = p_minZoom;
		return;
	}

	if(newZoom < p_maxZoom)
	{
		p_zoom = p_maxZoom;
		return;
	}

	p_zoom = newZoom;
}

void OrthoCamera::Update( IDirect3DDevice9 *pDevice )
{
	D3DXMatrixOrthoOffCenterLH( &p_matrixProj, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 1, 10 );
	pDevice->SetTransform(D3DTS_PROJECTION, &this->p_matrixProj);

	//set the camera's view matrix
	D3DXVECTOR3 eye(this->p_position.x, this->p_position.y, (float)-p_zoom);
	D3DXVECTOR3 at(eye.x, eye.y, (float)p_maxZoom);
	D3DXMatrixLookAtLH(&this->p_matrixView, &eye, &at, &D3DXVECTOR3(0.0f,1.0f,0.0f));
	pDevice->SetTransform(D3DTS_VIEW, &this->p_matrixView);
}