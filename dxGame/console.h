#pragma once

#include "graphics.h"
#include "phatDX.h"

///////////////////////////////////////////////////////////////////////////////
// Creates a quake-style console.
// Bugs:
//		none
//	Todo:
//		Finish creation and input/output of commands and text
//		Add command history
///////////////////////////////////////////////////////////////////////////////
class phatConsole
{
private:
	string mString; // For output
	string mHistory[30]; // for command history up to 30
	bool mHidden;
	bool mShift; // for a virtual tilde ( ~ )
	bool bClearText;
	RECT mRect;
	D3DCOLOR mColor; // for custom colors
public:
	phatConsole();
	~phatConsole();

	inline void setShift(bool tmp) { mShift = tmp; }
	inline void setHidden(bool tmp) { mHidden = tmp; }
	const string getString() { return mString; }
	const bool getHidden() { return mHidden; }
	inline void setColor(D3DCOLOR dColor) { mColor = dColor; }
	inline void setColor(int iA, int iR, int iG, int iB) { D3DCOLOR tmp = D3DCOLOR_ARGB(iA, iR, iG, iB); mColor = tmp; }

	void init(); // Setup for drawing
	void draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, ID3DXFont *font);
	void processInput(WPARAM wparam);
	void doInput();
};
///////////////////////////////////////////////////////////////////////////////

