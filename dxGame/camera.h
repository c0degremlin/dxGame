///////////////////////////////////////////////////////////////////////////////
// Filename: camera.h
// Purpose: Provide a framework for d3d9 camera
// Created by: Shane Laffond stolen from the web somewhere (gamedev.net???)
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		None
// Todo:
//		None
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class OrthoCamera
{
private:
	D3DXMATRIX  p_matrixView;
	D3DXMATRIX  p_matrixProj;
	D3DXVECTOR2 p_position;
	double      p_zoom;

	double p_maxZoom;
	double p_minZoom;
	double p_rectWidth;
	double p_rectHeight;

public:
	OrthoCamera();
	~OrthoCamera();
	void Zoom( double zoom );
	void Update( IDirect3DDevice9 *pDevice );

	D3DXVECTOR2 getPosition()   { return p_position; }
	D3DXMATRIX  getView()      { return p_matrixView; }

	void setZoomRange(double min = 0.0, double max = 100.0) { p_maxZoom = max; p_minZoom = min; }

	double getMaxZoom() { return p_maxZoom; }
	double getMinZoom() { return p_minZoom; }

	double getZoom()   { return p_zoom;   }

	void setRect(double width, double height) { p_rectWidth = (float)width; p_rectHeight = (float)height; }

	void setPosition(double x, double y)   { p_position.x = (float)x; p_position.y = (float)y; }
	void setPosition(D3DXVECTOR2 position)   { p_position = position; }

	double getX() { return p_position.x; }
	double getY() { return p_position.y; }

	void setX(double value) { p_position.x = (float)value; }
	void setY(double value) { p_position.y = (float)value; }
};