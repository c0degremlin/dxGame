#include "sprite.h"

extern LARGE_INTEGER timeEnd;
extern LARGE_INTEGER timeStart;
extern LARGE_INTEGER timerFreq;

phatSprite::phatSprite()
{
	mX = mY = 0;
	mFilename.clear();
	bIsLoaded = bIsHidden = bIsMoving = false;
	mRows = mColumns = mCurFrame = mTotalFrames = 0;
	mStep = step;
	mScaling = 1.0f; // no scaling
}

phatSprite::~phatSprite()
{
}

////////////////////////////////n///////////////////////////////////////////////
// Loads a sprite sheet based on width and height
// Get rows, columns and frames mathematically
// Todo:
//		Finish
// Bugs:
//		Use frame vectors and stuff them properly
///////////////////////////////////////////////////////////////////////////////
void phatSprite::loadSheet( IDirect3DDevice9 *device, string sFilename, float iWidth, float iHeight, bool png, D3DCOLOR colorkey )
{
	D3DXIMAGE_INFO SrcInfo;

	float iColm;
	float iRow;

	mFilename = sFilename;
	mWidth = iWidth;
	mHeight = iHeight;
	mColorkey = colorkey;

	if(!png)
	{
		// Load image from file w/ colorkey
		gResult = D3DXCreateTextureFromFileEx(device, mFilename.c_str(), NULL, NULL, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
			colorkey, &SrcInfo, NULL, &p_mTexture);
		checkError();
	}
	else
	{
		// Load image from file w/o colorkey
		gResult = D3DXCreateTextureFromFileEx(device, mFilename.c_str(), NULL, NULL, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
			NULL, &SrcInfo, NULL, &p_mTexture);
		checkError();
	}
	mColumns = ( SrcInfo.Width / mWidth );
	mRows = ( SrcInfo.Height / mHeight );
	if(mRows > 0 && mColumns > 0)
		mTotalFrames = mRows * mColumns;
	else
		mTotalFrames = mRows + mColumns;

	iColm = ( SrcInfo.Width / iWidth );
	iRow = ( SrcInfo.Height / iHeight );

	mOffsetW = ( mWidth / (float)SrcInfo.Width );
	mOffsetH = ( mHeight / (float)SrcInfo.Height );

	for( int i = ( iRow + iColm ); i > 0; i--) // TODO: i don't work, im lazy
	{
		float tmp;
		tmp = (float)iRow / (float)i;
		mXframes.push_back( tmp-1.0f );
	}
}

////////////////////////////////n///////////////////////////////////////////////
// Draws correct frame with smooth timing.	Get offset from extern input
// Todo:
//		Finish
// Bugs:
//		Drawing a square dont work right i.e. programmaticly
//
//		Not really a bug but mCurFrame starts at 0
///////////////////////////////////////////////////////////////////////////////
// TODO: Rewrite me, im all fucked up
void phatSprite::draw( IDirect3DDevice9 *device, IDirect3DVertexBuffer9 *buffer, float iOffset)
{
	float iRate = 0.0f; // smooth animation
	VOID* pVoid = NULL; // a void pointer

	float fU;
	float fV;

	int colms;
	int rows;

	// End time - start time / last time called
	iRate = ( (float)timeEnd.QuadPart - (float)timeStart.QuadPart ) / timerFreq.QuadPart;

	// fU and fV are for calcs. they default to mOffset

	if( mCurFrame>=mTotalFrames )
		mCurFrame = 0;

	if( mRows == 1 ) // Rectangle
	{
		// Save width and height
		fV = mWidth;
		fU = mHeight;

		// scaling is better outside declarations
		mWidth *= mScaling;
		mHeight *= mScaling;

		iOffset = mOffsetW;

		//// Quad (1-2-3-4)
		//{ mX, mY, mZ, mColorTint, mnX, mnY, mnZ, iOffset*mCurFrame, 0.0f }; // Top Left 0u,0v
		//{ mX+mWidth, mY, mZ, mColorTint, mnX, mnY, mnZ, iOffset+(iOffset*mCurFrame), 0.0f }, // Top Right 1u,0v
		//{ mX, mY+mHeight, mZ, mColorTint, mnX, mnY, mnZ, iOffset*mCurFrame, 1.0f }, // Bottom left 0u,1v
		//{ mX+mWidth, mY+mHeight, mZ, mColorTint, mnX, mnY, mnZ, iOffset+(iOffset*mCurFrame), 1.0f } // Bottom right 1u,1v

		// Stuff that bad chicken manually
		// Xs and Ys
		mQuad[0].x = mX; mQuad[0].y = mY;		mQuad[1].x = mX+mWidth; mQuad[1].y = mY; // top left and right
		mQuad[2].x = mX; mQuad[2].y = mY+mHeight;		mQuad[3].x = mX+mWidth; mQuad[3].y = mY+mHeight; // bottom left and right

		// Us and Vs
		mQuad[0].u = mQuad[2].u = iOffset*mCurFrame; // u = 0.0f
		mQuad[1].u = mQuad[3].u = iOffset+(iOffset*mCurFrame); // u = 1.0f
		mQuad[0].v = mQuad[1].v = 0.0f;	// top
		mQuad[2].v = mQuad[3].v = 1.0f; // bottom

		// Constants
		mQuad[0].z = mQuad[1].z = mQuad[2].z = mQuad[3].z = mZ; // All same
		mQuad[0].diffuse = mQuad[1].diffuse = mQuad[2].diffuse = mQuad[3].diffuse = mColorTint; // All the same
		mQuad[0].nX = mQuad[1].nX = mQuad[2].nX = mQuad[3].nX = mnX;	// Normals
		mQuad[0].nY = mQuad[1].nY = mQuad[2].nY = mQuad[3].nY = mnY;	// ^^
		mQuad[0].nZ = mQuad[1].nZ = mQuad[2].nZ = mQuad[3].nZ = mnZ;	// ^^

		if(mCurFrame==0)
		{
			mQuad[0].u = mQuad[2].u = 0.0f; // start at front of texture
			mQuad[1].u = mQuad[3].u = iOffset;
		}

		if(buffer)
		{	// lock vertex buffer and stuff it
			buffer->Lock(0, 0, (void**)&pVoid, D3DLOCK_DISCARD);
			memcpy(pVoid, mQuad, sizeof(mQuad));
			buffer->Unlock();
		}
		else
		{
			gString = mFilename + " - No buffer to stuff??";
			throwError(gString);
		}
		//Restore our width and height
		mWidth = fV;
		mHeight = fU;
	}
	else // Square <-- more than one row, not needed to be square
	{
		for( rows = 0; rows < mRows; ++rows )
		{
			for( colms = 0; colms < mColumns; colms++)
			{
				// Save width and height
				fV = mWidth;
				fU = mHeight;

				// scaling is better outside declarations
				mWidth *= mScaling;
				mHeight *= mScaling;

				iOffset = mOffsetW;

				//// Quad (1-2-3-4)
				//{ mX, mY, mZ, mColorTint, mnX, mnY, mnZ, iOffset*mCurFrame, 0.0f }; // Top Left 0u,0v
				//{ mX+mWidth, mY, mZ, mColorTint, mnX, mnY, mnZ, iOffset+(iOffset*mCurFrame), 0.0f }, // Top Right 1u,0v
				//{ mX, mY+mHeight, mZ, mColorTint, mnX, mnY, mnZ, iOffset*mCurFrame, 1.0f }, // Bottom left 0u,1v
				//{ mX+mWidth, mY+mHeight, mZ, mColorTint, mnX, mnY, mnZ, iOffset+(iOffset*mCurFrame), 1.0f } // Bottom right 1u,1v

				// Stuff that bad chicken manually
				// Xs and Ys
				mQuad[0].x = mX; mQuad[0].y = mY;		mQuad[1].x = mX+mWidth; mQuad[1].y = mY; // top left and right
				mQuad[2].x = mX; mQuad[2].y = mY+mHeight;		mQuad[3].x = mX+mWidth; mQuad[3].y = mY+mHeight; // bottom left and right

				// Us and Vs
				mQuad[0].u = mQuad[2].u = iOffset*mCurFrame; // u = 0.0f
				mQuad[1].u = mQuad[3].u = iOffset+(iOffset*mCurFrame); // u = 1.0f
				mQuad[0].v = mQuad[1].v = 0.0f;	// top
				mQuad[2].v = mQuad[3].v = mOffsetH; // bottom <-- maybe mOffsetH??

				// Constants
				mQuad[0].z = mQuad[1].z = mQuad[2].z = mQuad[3].z = mZ; // All same
				mQuad[0].diffuse = mQuad[1].diffuse = mQuad[2].diffuse = mQuad[3].diffuse = mColorTint; // All the same
				mQuad[0].nX = mQuad[1].nX = mQuad[2].nX = mQuad[3].nX = mnX;	// Normals
				mQuad[0].nY = mQuad[1].nY = mQuad[2].nY = mQuad[3].nY = mnY;	// ^^
				mQuad[0].nZ = mQuad[1].nZ = mQuad[2].nZ = mQuad[3].nZ = mnZ;	// ^^

				if(colms==0)
				{
					mQuad[0].u = mQuad[2].u = 0.0f; // start at front of texture
					mQuad[1].u = mQuad[3].u = iOffset;
				}

				if(mRows) // on to the next one ...
				{
					// I should be working programmatically
				}

				//Restore our width and height
				mWidth = fV;
				mHeight = fU;
			} // end colm

			// HACK: i am a hack
			if( mCurFrame == mColumns && mCurFrame < (mColumns + rows) ) // beginning of new row
			{
				mQuad[0].u = mQuad[2].u = 0.0f; // start at front of texture
				mQuad[0].v = mQuad[1].v = mOffsetH;	// top
				mQuad[1].u = mQuad[3].u = mOffsetW; // u = 1.0f
				mQuad[2].v = mQuad[3].v = mOffsetH*mCurFrame; // bottom <-- maybe mOffsetH??
			}
			else if (mCurFrame > mRows) // i dont work
			{
				mQuad[0].u = mQuad[2].u = mOffsetW;
				mQuad[0].v = mQuad[1].v = mOffsetH;	// top
				mQuad[1].u = mQuad[3].u = mOffsetW*mCurFrame;
				mQuad[2].v = mQuad[3].v = mOffsetH*mCurFrame; // bottom <-- maybe mOffsetH??
			}

			if(buffer)
			{	// lock vertex buffer and stuff it
				gResult = buffer->Lock(0, 0, (void**)&pVoid, D3DLOCK_DISCARD);
				memcpy(pVoid, mQuad, sizeof(mQuad));
				buffer->Unlock();
			}
			else
			{
				gString = mFilename + " - No buffer to stuff??";
				throwError(gString);
			}
		} // end rows
	}

	device->SetVertexShader(NULL);
	device->SetFVF(UNTRANS_FVF);
	device->SetStreamSource(0, buffer, 0, sizeof(s3DVertex));

	// Set texture
	if( p_mTexture != NULL )
		gResult = device->SetTexture (0, p_mTexture);
	else
	{
		gString = mFilename;
		gString.append(" is not loaded. It is null.");
		throwError(gString);
	}
	// Draw image
	gResult = device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );
	checkError();

	device->SetTexture( 0, NULL ); // Free up the texture so we don't repeat them
}