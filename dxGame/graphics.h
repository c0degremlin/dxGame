///////////////////////////////////////////////////////////////////////////////
// Filename: graphics.h
// Purpose: Provide a framework for d3d9
// Created by: Shane Laffond
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		Lighting don't work right
// Todo:
//		get 2D drawing via phatSprite to work properly
//		Get lighting to work, probably bad normals
//		Add checks to if paused before drawing
///////////////////////////////////////////////////////////////////////////////
#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include "global.h"

#include <vector>
using std::vector;

#include "log.h"

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.
// This makes D3D objects work well in the debugger watch window, but slows down performance slightly
#if defined (_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

// Thank you Andre LaMothe
#define ReleaseCOM(x) { if(x) { x->Release(); x = 0; } }

#define ExtractAlpha(x) ((x>>24)&255)
#define ExtractRed(x) ((x>>16)&255)
#define ExtractGreen(x) ((x>>12)&255)
#define ExtractBlue(x) (x&255)

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete [] (x); (x) = NULL; }
#define MSG_RETURN(x,s) if (!(x)) { MessageBox( 0, s, 0, 0 ); return FALSE; }
#define RETURN(x) if (!(x)) { return FALSE; }


const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const float step = 4; // Default step for phatSprite

extern HWND gWindow; // for setVsync()

static int gLightIndex = 0; // for light index


///////////////////////////////////////////////////////////////////////////////
//	D3DCOLOR defines
///////////////////////////////////////////////////////////////////////////////
const D3DCOLOR D3D_RED = D3DCOLOR_XRGB( 255, 0, 0 );
const D3DCOLOR D3D_GREEN = D3DCOLOR_XRGB( 0, 255, 0 );
const D3DCOLOR D3D_BLUE = D3DCOLOR_XRGB( 0, 0, 255 );
const D3DCOLOR D3D_PINK = D3DCOLOR_XRGB( 255, 0, 255 );
const D3DCOLOR D3D_GREY = D3DCOLOR_XRGB( 100, 100, 100 );
const D3DCOLOR D3D_BLACK = D3DCOLOR_XRGB( 0, 0, 0 );
const D3DCOLOR D3D_WHITE = D3DCOLOR_XRGB( 255, 255, 255 );

const D3DCOLOR D3D_APPBLUE = D3DCOLOR_XRGB( 0, 0, 212 ); // dark blue found in all samples

///////////////////////////////////////////////////////////////////////////////
//	End D3DCOLOR defines
///////////////////////////////////////////////////////////////////////////////

//// Quad (1-2-3-4)
//{ mX, mY, mZ, mColorTint, mnX, mnY, mnZ, iOffset*mCurFrame, 0.0f }; // Top Left 0u,0v
//{ mX+mWidth, mY, mZ, mColorTint, mnX, mnY, mnZ, iOffset+(iOffset*mCurFrame), 0.0f }, // Top Right 1u,0v
//{ mX, mY+mHeight, mZ, mColorTint, mnX, mnY, mnZ, iOffset*mCurFrame, 1.0f }, // Bottom left 0u,1v
//{ mX+mWidth, mY+mHeight, mZ, mColorTint, mnX, mnY, mnZ, iOffset+(iOffset*mCurFrame), 1.0f } // Bottom right 1u,1v

typedef struct
{
	float x, y, z, rhw; // 2D
	D3DCOLOR diffuse; // color
} sPrimitive;// For primitives
typedef struct
{
	float x, y, z, rhw; // 2D
	D3DCOLOR diffuse; // color
	float u, v;
} sVertex;// For 2D
typedef struct
{
	float x, y, z;
	D3DCOLOR diffuse; // color
	float nX, nY, nZ; // Normals
	float u, v;
} s3DVertex;// for 3D

const DWORD PRIM_FVF = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE ); // 2D with color
const DWORD TRANS_FVF = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );// 2D with color and texture coords
const DWORD UNTRANS_FVF = ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1 );// 3D with normals, color and texture coords

// Uses 2D call between sprite drawing calls
void drawText( IDirect3DDevice9 *device, ID3DXFont *font, string sText, int x, int y,
D3DCOLOR dColor = D3D_WHITE, int height = 18, string sFont = "Arial", DWORD dFlags = DT_NOCLIP | DT_LEFT);

IDirect3DTexture9 *loadTexture(IDirect3DDevice9* pDevice, string sFilename, int iWidth, int iHeight,
					D3DCOLOR colorkey = D3D_PINK);

void drawBox( IDirect3DDevice9 *pDevice, IDirect3DVertexBuffer9 *pBuffer, int iLeft, int iTop, int iRight, int iBottom,
					D3DCOLOR dColor = D3DCOLOR_XRGB( 255, 255, 255));
void drawBox( IDirect3DDevice9 *pDevice, IDirect3DVertexBuffer9 *pBuffer, RECT rRect, D3DCOLOR dColor = D3D_WHITE);
void drawLine( IDirect3DDevice9 *pDevice, IDirect3DVertexBuffer9 *pBuffer, int iX1, int iY1, int iX2, int iY2, D3DCOLOR dColor = D3D_WHITE);

///////////////////////////////////////////////////////////////////////////////
// DirectX 9 2d and 3d
// Bugs:
//		DrawText reports a false error???
//	Todo:
//		Add lighting
//		Stuff the D3DXSprite stuff in here ???
///////////////////////////////////////////////////////////////////////////////
class cPhatDX
{
private:
	D3DPRESENT_PARAMETERS md3dpp;
	D3DDISPLAYMODE md3dmode;
	D3DXFONT_DESC mDefaultFontDesc;

	D3DVIEWPORT9 mViewport;

	bool bWindowed;
	bool bVsync;
	bool bZbuffer;
	bool bLighting;
	bool bHAL; // hardware accelerated
	bool b2D; // used for drawing text w/ or w/o sprite handler

	int mWidth;
	int mHeight;
public:
	IDirect3D9 *p_mObject;
	IDirect3DDevice9 *p_mDevice;
	IDirect3DVertexBuffer9 *p_mVB;
	IDirect3DVertexBuffer9 *p_mVB3d;
	IDirect3DVertexBuffer9 *p_mVBprim;
	ID3DXSprite *p_mSpriteHandler;    // the pointer to our Direct3D Sprite interface
	ID3DXFont* p_mFont; // font Times New Roman 24 pt

	cPhatDX();
	~cPhatDX();

	// Accessors
	bool isWindowed() const { return bWindowed; }
	bool isVsync() const { return bVsync; }
	bool isZbuffer() const { return bZbuffer; }
	bool isLit() const { return bLighting; }
	bool isHAL() const { return bHAL; }
	bool is2D() const { return b2D; }

	void setVsync(bool tmp);

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

	void createD3D(HWND hWindow, bool windowed = true, bool vysnc = false , bool zbuffer = true); // Create Direct3D for 3 and 2D

	// default to dark blue
	void clear(int r = 0, int g = 0, int b = 212);

	// for 3D
	void beginScene();
	void presentScene();

	// for 2D
	void begin2D();
	void end2D();

	// Alpha blending
	void enableAlphaBlending( DWORD src = D3DBLEND_SRCALPHA, DWORD dest = D3DBLEND_INVSRCALPHA );
	void enableAlphaTesting();
	void disableAlphaBlending();
	void disableAlphaTesting();

	// Lighting funcs
	void enableLighting();
	void enableLight(D3DLIGHT9 &dLight);
	void disableLighting();
	void disableLight(D3DLIGHT9 &dLight);
	void setAmbient( D3DCOLOR ambient = D3D_WHITE ); // Default to turn light off 
													 // (black light doesn't emit right?)
	void setAmbient( int iA, int iR, int iG, int iB );
	D3DCOLOR getAmbient() const { D3DCOLOR tmp; p_mDevice->GetRenderState( D3DRS_AMBIENT, &tmp ); return tmp;}

	// Z buffer
	void enableZ();
	void disableZ();
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Textured quad wrapper
// Bugs:
//		None
//	Todo:
//		None
///////////////////////////////////////////////////////////////////////////////
class cTexQuad
{
private:
	sVertex mQuad[4]; // Only used here
protected:
	IDirect3DTexture9 *p_mTexture;
	D3DCOLOR mColorkey;
	D3DCOLOR mColorTint; // possible make lighting?
	string mFilename;

	float mWidth;
	float mHeight;

	float mX;
	float mY;
	bool bLoaded;
public:
	cTexQuad();
	~cTexQuad();

	void load( IDirect3DDevice9* pDevice, string sFilename, float iWidth, float iHeight, D3DCOLOR colorkey = D3D_PINK);
	void draw( IDirect3DDevice9* pDevice, IDirect3DVertexBuffer9* pVB);

	// Explicit
	void setX( float fNum ) { mX = fNum; }
	void setY( float fNum ) { mY = fNum; }
	// W/ bounds checking
	void moveX( float fNum = step)
	{
		float tmp = fNum + mX;
		if( tmp <= ( WINDOW_WIDTH - mWidth ) && tmp >= 0 )
			mX = tmp;
	}
	void moveY( float fNum = step )
	{
		float tmp = fNum + mY;
		if( tmp <= ( WINDOW_HEIGHT - mHeight ) && tmp >= 0 )
			mY = tmp;
	}
	void moveTo( float fx = step, float fy = step )
	{
		float tmp = fx + mX;
		if( tmp <= ( WINDOW_WIDTH - mWidth ) && tmp >= 0 )
			mX = tmp;

		tmp = fy + mY;
		if( tmp <= ( WINDOW_HEIGHT - mHeight ) && tmp >= 0 )
			mY = tmp;
	}

	void setColor( D3DCOLOR cColor) { mColorTint = cColor; }
	void setColor( int iA, int iR, int iG, int iB ) { D3DCOLOR tmp = D3DCOLOR_ARGB( iA, iR, iG, iB ); mColorTint = tmp; }

	string const getFilename() { return mFilename; }
	IDirect3DTexture9 *getTexture() const { return p_mTexture; }

	float getX() const { return mX; }
	float getY() const { return mY; }
	float getWidth() const { return mWidth; }
	float getHeight() const { return mHeight; }
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 3d lit textured quad wrapper
// Bugs:
//		lights don't work
//	Todo:
//		Normals aren't right <-- i think this is a lie // its not a lie normals should be pointed up
//		Lights don't work
///////////////////////////////////////////////////////////////////////////////
class cTexQuad3d : public cTexQuad
{
protected:
	float mZ;
	float mnX;
	float mnY;
	float mnZ;

	D3DMATERIAL9 mMaterial;
	s3DVertex mQuad[4];
public:
	cTexQuad3d();
	~cTexQuad3d();

	void setMaterial( D3DCOLORVALUE diffuse, D3DCOLORVALUE ambient, D3DCOLORVALUE specular, D3DCOLORVALUE emissive, float power );
	void draw( IDirect3DDevice9* pDevice, IDirect3DVertexBuffer9* pVB );
	D3DXVECTOR3 getTriangleNormal( D3DXVECTOR3 *vertex1, D3DXVECTOR3 *vertex2, D3DXVECTOR3 *vertex3 );
	void setupNormals();
};
///////////////////////////////////////////////////////////////////////////////

