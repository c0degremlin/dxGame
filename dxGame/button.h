///////////////////////////////////////////////////////////////////////////////
// Filename: button.h
// Purpose: Create virtual buttons with or without a texture
// Created by: Shane Laffond
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		Textured button loads but wont display
//		automatic text bounding box calculation improper, may have fixed it. do math when using functions not just init
// Todo:
//		Finish creating classes
///////////////////////////////////////////////////////////////////////////////

#pragma  once
#include "input.h"
#include "graphics.h"

const int outline_offset = 3;

class phatButton : public cTexQuad
{
private:
	string mText;
	D3DCOLOR mTextColor;
	int mMx; // Mouse x and y
	int mMy;
	RECT mRect; // for outline box
public:
	phatButton();
	~phatButton();

	void init(string sString, int iX, int iY, int iW, int iH);
	void init(IDirect3DDevice9 *device, string sFilename, string sString, int iX, int iY, int iW, int iH = 20);

	void draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, ID3DXFont *font, bool outline);
	void draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, bool outline);

	bool getClicks( int mouseX, int mouseY );

	// Accessors
	string getText() const { return mText; }

	void setText(string sText) { mText = sText; };
	void setTextColor( D3DCOLOR dColor ) { mTextColor = dColor; }
	void setTexture(IDirect3DDevice9 *device, string sFilename, int width, int height, D3DCOLOR keycolor);
};
