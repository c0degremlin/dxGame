#include "console.h"

extern HWND gWindow;

phatConsole::phatConsole()
{
	mString.clear();
	mHidden = true;
	mShift = false;
	bClearText = false;
	SetRect(&mRect, 1,1, WINDOW_WIDTH-1, 150);
	mColor = D3DCOLOR_ARGB(128, 210, 210, 0);
}

phatConsole::~phatConsole()
{
}

void phatConsole::init()
{
}

void phatConsole::draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, ID3DXFont *font)
{
	if(!mHidden) // supposed to pause
	{
		// show only the console
		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3D_APPBLUE, 1.0f, 0);
		
		// Should be pausing here
		drawBox( device, buffer, mRect, mColor );

		if( mString.empty() )
			mString.insert(0, ":> "); // Add the caret

		drawText( device, font, mString, 5, 130, D3DCOLOR_XRGB(0,255,0) );
		if(bClearText) // signal a return
		{
			mString.clear();	bClearText = false;
		}
	}
	else if( !mString.empty())
		mString.clear(); // can't have anything showing...
}

///////////////////////////////////////////////////////////////////////////////
//	Input and output text
//	Bugs:
//		None
//	Todo:
//		Flush to command buffer
///////////////////////////////////////////////////////////////////////////////
void phatConsole::processInput(WPARAM wparam)
{
	if (!mHidden)
	{
		switch(wparam)
		{
		case VK_A: mString+=wparam; break;			case VK_B: mString+=wparam; break;			case VK_C: mString+=wparam; break;
		case VK_D: mString+=wparam; break;			case VK_E: mString+=wparam; break;			case VK_F: mString+=wparam; break;
		case VK_G: mString+=wparam; break;			case VK_H: mString+=wparam; break;			case VK_I: mString+=wparam; break;
		case VK_J: mString+=wparam; break;			case VK_K: mString+=wparam; break;			case VK_L: mString+=wparam; break;
		case VK_M: mString+=wparam; break;			case VK_N: mString+=wparam; break;			case VK_O: mString+=wparam; break;
		case VK_P: mString+=wparam; break;			case VK_Q: mString+=wparam; break;			case VK_R: mString+=wparam; break;
		case VK_S: mString+=wparam; break;			case VK_T: mString+=wparam; break;			case VK_U: mString+=wparam; break;
		case VK_V: mString+=wparam; break;			case VK_W: mString+=wparam; break;			case VK_X: mString+=wparam; break;
		case VK_Y: mString+=wparam; break;			case VK_Z: mString+=wparam; break;			case VK_SPACE: mString += wparam; break;
		case VK_RETURN: bClearText = true; doInput(); break;
		case VK_BACK: if( !mString.empty() )mString.erase( mString.end()-1, mString.end() ); break;
		}
		for (int j=0; j<mString.length(); ++j)
			mString[j]=tolower(mString[j]);
		// supposed to flush input into some buffer here
	}
}

// must append to desired command ':> '
// only does hard coded command
void phatConsole::doInput()
{
	if(mString == ":> exit" )
		DestroyWindow(gWindow);

	mString.clear();
}