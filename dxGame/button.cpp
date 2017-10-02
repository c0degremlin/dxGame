#include "button.h"

phatButton::phatButton()
{ 
	mText.clear(); mFilename.clear();
	mX = mY = 0;
	mWidth = mHeight = 0;
	mTextColor = D3D_BLACK; mColorTint = D3D_BLACK;
	SetRect(&mRect, 0, 0, 0, 0);
}

phatButton::~phatButton()
{
}

void phatButton::init(IDirect3DDevice9 *device, string sFilename, string sString, int iX, int iY, int iW, int iH)
{
	mX = iX;
	mY = iY;
	mText = sString;
	mFilename = sFilename;

	mWidth = iW;
	mHeight = iH;

	// set the outline option rect
	SetRect(&mRect, mX - outline_offset, mY - outline_offset, mWidth + outline_offset, mHeight + outline_offset);

	cTexQuad::load( device, sFilename, mWidth, mHeight );
}

void phatButton::init(string sString, int iX, int iY, int iW, int iH )
{
	mX = iX;
	mY = iY;
	mHeight = iH;
	mText = sString;

	// Calculate width if needed
	gInt = mText.size();
	mWidth = gInt * 11; // Not sure if this is an absolute value
	if( mWidth < 11 )
		mWidth = iW; // use input value
}

void phatButton::draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, ID3DXFont *font, bool outline)
{
	if( outline ) // offset default to 3px

		SetRect(&mRect, mX - outline_offset, mY - outline_offset, mWidth + outline_offset, mHeight + outline_offset);
		drawBox( device, buffer, mRect, mColorTint);

	if( !mText.empty() )
		drawText( device, font, mText, mX, mY, mTextColor );
}


// TODO: I don't work im broken
void phatButton::draw(IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, bool outline)
{
	SetRect( &mRect, mX-outline_offset, mY-outline_offset, mWidth+outline_offset, mHeight+outline_offset );

	//if( outline ) // offset default to 3px
		drawBox( device, buffer, mRect, mColorTint);

	cTexQuad::draw( device, buffer );
}

bool phatButton::getClicks( int mouseX, int mouseY )
{
	mMx = mouseX;
	mMy = mouseY;

	if( mMx >= mX && mMx <= mX+mWidth )
	{
		if (mMy >= mY && mMy <= mY + mHeight)
		{
				return true;
		}
	}

	return false;
}

void phatButton::setTexture(IDirect3DDevice9 *device, string sFilename, int width, int height, D3DCOLOR keycolor)
{
	if( p_mTexture )
		ReleaseCOM (p_mTexture);

	p_mTexture = loadTexture( device, sFilename, width, height, keycolor );
}