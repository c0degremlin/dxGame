#include "graphics.h"

cTexQuad::cTexQuad()
{
	mWidth = mHeight = 0.0f;
	mX = mY = 0.0f;
	mColorkey = NULL;
	mColorTint = D3DCOLOR_XRGB( 255, 255, 255 ); // No tinting
	p_mTexture = NULL;
}

cTexQuad::~cTexQuad()
{
	ReleaseCOM( p_mTexture );
}

void cTexQuad::load(IDirect3DDevice9* pDevice,string sFilename, float iWidth, float iHeight, D3DCOLOR colorkey)
{
	mFilename = sFilename;
	mColorkey = colorkey;
	mWidth = iWidth;
	mHeight = iHeight;

	p_mTexture = loadTexture( pDevice, sFilename, iWidth, iHeight, colorkey);

	if( p_mTexture != NULL )bLoaded = true;	else bLoaded = false;
}

void cTexQuad::draw(IDirect3DDevice9* pDevice, IDirect3DVertexBuffer9* pVB)
{
	VOID* pVoid; // a void pointer

	// Stuff that bad chicken manually
	// Xs and Ys
	mQuad[0].x = mX; mQuad[0].y = mY;// mQuad[0].u = iOffset*mCurFrame;
	mQuad[1].x = mX+mWidth; mQuad[1].y = mY; //mQuad[1].u = iOffset+(iOffset*mCurFrame);
	mQuad[2].x = mX; mQuad[2].y = mY+mHeight;
	mQuad[3].x = mX+mWidth; mQuad[3].y = mY+mHeight;

	// Us and Vs
	mQuad[0].u = mQuad[2].u = 0.0f;		mQuad[1].u = mQuad[3].u = 1.0f;
	mQuad[0].v = mQuad[2].v = 0.0f;		mQuad[1].v = mQuad[3].v = 1.0f;

	// Constants
	mQuad[0].z = mQuad[1].z = mQuad[2].z = mQuad[3].z = 1.0f; // All same
	mQuad[0].diffuse = mQuad[1].diffuse = mQuad[2].diffuse = mQuad[3].diffuse = mColorTint; // All the same

	if(pVB)
	{	// lock vertex buffer and stuff it
		pVB->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, mQuad, sizeof(mQuad));
		pVB->Unlock();
	}
	else
	{
		gString = mFilename + " - No buffer to stuff??";
		throwError(gString);
	}
	pDevice->SetVertexShader(NULL);
	pDevice->SetFVF(TRANS_FVF);
	pDevice->SetStreamSource(0, pVB, 0, sizeof(sVertex));

	//Set texture
	if( p_mTexture != NULL )
		gResult = pDevice->SetTexture (0, p_mTexture);
	else
	{
		gString = mFilename + " is not loaded. It is null.";
		throwError(gString);
	}

	//Draw image
	gResult = pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );
	checkError();

	pDevice->SetTexture( 0, NULL ); // Free up the texture so we don't repeat them
}

cTexQuad3d::cTexQuad3d()
{
	mZ = 1.0f;
	mnX = 1.0f;
	mnY = 0.0f;
	mnZ = -1.0f;
}

cTexQuad3d::~cTexQuad3d()
{}

void cTexQuad3d::setMaterial( D3DCOLORVALUE diffuse, D3DCOLORVALUE ambient, D3DCOLORVALUE specular, D3DCOLORVALUE emissive, float power )
{
	ZeroMemory( &mMaterial, sizeof( D3DMATERIAL9 ) );	mMaterial.Emissive = emissive;
	mMaterial.Diffuse = diffuse;		mMaterial.Ambient = ambient;		mMaterial.Specular = specular;		mMaterial.Power = power;
}

void cTexQuad3d::draw(IDirect3DDevice9* pDevice, IDirect3DVertexBuffer9* pVB)
{
	// Stuff that bad chicken manually
	// Xs and Ys
	mQuad[0].x = mX; mQuad[0].y = mY;// mQuad[0].u = iOffset*mCurFrame;
	mQuad[1].x = mX+mWidth; mQuad[1].y = mY; //mQuad[1].u = iOffset+(iOffset*mCurFrame);
	mQuad[2].x = mX; mQuad[2].y = mY+mHeight;
	mQuad[3].x = mX+mWidth; mQuad[3].y = mY+mHeight;

	// Us and Vs
	mQuad[0].u = mQuad[2].u = 0.0f;		mQuad[1].u = mQuad[3].u = 1.0f;
	mQuad[0].v = mQuad[2].v = 0.0f;		mQuad[1].v = mQuad[3].v = 1.0f;

	// Constants
	mQuad[0].z = mQuad[1].z = mQuad[2].z = mQuad[3].z = mZ; // All same
	mQuad[0].diffuse = mQuad[1].diffuse = mQuad[2].diffuse = mQuad[3].diffuse = mColorTint; // All the same
	mQuad[0].nX = mQuad[1].nX = mQuad[2].nX = mQuad[3].nX = mnX;	// Normals
	mQuad[0].nY = mQuad[1].nY = mQuad[2].nY = mQuad[3].nY = mnY;	// ^^
	mQuad[0].nZ = mQuad[1].nZ = mQuad[2].nZ = mQuad[3].nZ = mnZ;	// ^^

	VOID* pVoid; // a void pointer

	if(pVB)
	{	// lock vertex buffer and stuff it
		pVB->Lock(0, 0, (void**)&pVoid, D3DLOCK_DISCARD);
		memcpy(pVoid, mQuad, sizeof(mQuad));
		pVB->Unlock();
	}
	else
	{
		gString = mFilename + " - No buffer to stuff??"; throwError(gString);
	}
	pDevice->SetVertexShader(NULL);
	pDevice->SetFVF(UNTRANS_FVF);
	pDevice->SetStreamSource(0, pVB, 0, sizeof(s3DVertex));

	//Set texture
	if( p_mTexture != NULL )
		gResult = pDevice->SetTexture (0, p_mTexture);
	else
	{
		gString = mFilename;
		gString.append(" is not loaded. It is null.");
		throwError(gString);
	}

	//Draw image
	gResult = pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );
	checkError();

	pDevice->SetTexture( 0, NULL ); // Free up the texture so we don't repeat them
}

void cTexQuad3d::setupNormals()
{
}

/*	This method calculates the normal of a triangle using the given three vertices.  N = (v2 - v1)X(v3 - v1)
Parameters:
[in] vertex1 - A vertex [in] vertex2 - A vertex [in] vertex3 - A vertex
Returns: The normal as a D3DVECTOR3 */
D3DXVECTOR3 cTexQuad3d::getTriangleNormal( D3DXVECTOR3 *vertex1, D3DXVECTOR3 *vertex2, D3DXVECTOR3 *vertex3 )
{
	D3DXVECTOR3 normal;
	D3DXVECTOR3 vector1, vector2;

	D3DXVec3Subtract( &vector1, vertex2, vertex1 );
	D3DXVec3Subtract( &vector2, vertex3, vertex1 );

	D3DXVec3Cross( &normal, &vector1, &vector2 );

	D3DXVec3Normalize( &normal, &normal );

	return normal;
}