#pragma once

#include "graphics.h"

///////////////////////////////////////////////////////////////////////////////
// cTexQuad3d sprite wrapper
// Bugs:
//		None
//	Todo:
//		finish sheet loading
//		fix frame-based drawing
///////////////////////////////////////////////////////////////////////////////
class phatSprite : public cTexQuad3d
{
private:
	int mCurFrame;
	float mRows;
	float mColumns;
	int mTotalFrames;
	float mStep;

	float mScaling; // set to 1.0f for no scaling
	float mOffsetW; // for sheet looping i.e. width/height of device
	float mOffsetH;

	vector<float> mXframes; // Used in loadSheet
	vector<float> mYframes;

	bool bIsLoaded;
	bool bIsHidden;
	bool bIsMoving;
public:
	phatSprite();
	~phatSprite();
	void draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, float mOffset = 0.0800f);
	void loadSheet(IDirect3DDevice9* device, string sFilename, float iWidth, float iHeight, bool png = true, D3DCOLOR colorkey = D3D_PINK);

	// Accessors
	int const getFrame() { return mCurFrame; }
	int const getFrameCount() { return mTotalFrames; }
	float const getRows() { return mRows; }
	float const getColumns() { return mColumns; }
	float const getStep() { return mStep; }
	float const getScale() { return mScaling; }

	bool const isLoaded() { return bIsLoaded; }
	bool const isHidden() { return bIsHidden; }
	bool const isMoving() { return bIsMoving; }

	void setStep(float iNum) { mStep = iNum; }
	void setScale(float fNum = 1.0f) { mScaling = fNum; }
	inline void incFrame() { gInt = mCurFrame + 1; if (gInt <= mTotalFrames) mCurFrame = gInt; else mCurFrame = 0; }
	inline void decFrame() { gInt = mCurFrame - 1; if (gInt >= 0) mCurFrame = gInt; else mCurFrame = mTotalFrames; }
};
///////////////////////////////////////////////////////////////////////////////

