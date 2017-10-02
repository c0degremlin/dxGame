#pragma once

#include "graphics.h"



///////////////////////////////////////////////////////////////////////////////
// D3DLIGHT9 wrapper
// Bugs:
//		None
//	Todo:
//		Finish spot and directional light creation function
//		Create a stack of * 20 ??? * lights with only 8 on at a time
///////////////////////////////////////////////////////////////////////////////
class phatLight
{
private:
	D3DCOLOR mColor;
	D3DLIGHT9 mLight;
	D3DMATERIAL9 mMaterial;
public:
	static int mIndex;
	phatLight();
	~phatLight();

	void createPoint();
	void createDir();
	void createSpot();

	D3DLIGHT9 getLight() { return mLight; }
	D3DMATERIAL9 getMat() { return mMaterial; }

	void setColor(D3DCOLOR cColor) { mColor = cColor; }
	void setColor(int iA, int iR, int iG, int iB) { D3DCOLOR tmp = D3DCOLOR_ARGB(iA, iR, iG, iB); mColor = tmp; }
};
///////////////////////////////////////////////////////////////////////////////
