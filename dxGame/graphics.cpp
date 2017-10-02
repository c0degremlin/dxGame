#include "graphics.h"

void cPhatDX::createD3D(HWND hWindow, bool windowed, bool vsync, bool zbuffer)
{
	p_mObject = Direct3DCreate9(D3D_SDK_VERSION);
	if(p_mObject == NULL)
	{
		MB("Couldn't create D3D. Please make sure that you have DirectX 9 runtime installed.");
		exit(1);
	}

	ZeroMemory(&md3dmode, sizeof(md3dmode));
	ZeroMemory(&md3dpp, sizeof(md3dpp));
	ZeroMemory(&mDefaultFontDesc, sizeof(mDefaultFontDesc));

	bWindowed = windowed;
	bVsync = vsync;
	bZbuffer = zbuffer;

	mWidth = WINDOW_WIDTH;
	mHeight = WINDOW_HEIGHT;

	md3dpp.hDeviceWindow = hWindow;

	md3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	md3dpp.BackBufferCount = 1;
	md3dpp.BackBufferWidth = WINDOW_WIDTH;
	md3dpp.BackBufferHeight = WINDOW_HEIGHT;
	md3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	md3dpp.MultiSampleQuality = 0;

	md3dpp.EnableAutoDepthStencil = true;
	md3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	if(bVsync)
		md3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	else
		md3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(bWindowed)// Windowed with a backbuffer flip
	{
		p_mObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &md3dmode);

		bHAL = true;
		// check for 32 bit HAL
		if(FAILED(p_mObject->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, md3dmode.Format, D3DFMT_A8R8G8B8, bWindowed)))
		{
			bHAL = false;
			// check for 32 bit REF
			if(FAILED(p_mObject->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF, md3dmode.Format, D3DFMT_A8R8G8B8, bWindowed)))
			{
				MB("Please update your video card. It is too old.");
				return;
			}
		}
		p_mObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &md3dmode);

		// Setup and create window
		md3dpp.Windowed = bWindowed;
		md3dpp.BackBufferFormat = md3dmode.Format;

		// Windowed with a backbuffer flip
		md3dpp.BackBufferFormat = md3dmode.Format;

		if(bVsync)
			md3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		else
			md3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if( bHAL )
		{
			gResult = p_mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING, &md3dpp, &p_mDevice); //  hardware vertex processing
			checkError();
		}
		else
		{
			gResult = p_mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, hWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &md3dpp, &p_mDevice); // software vertex processing
			if( gResult != S_OK )
			{
				gResult = p_mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, &md3dpp, &p_mDevice); // no software unsupported hardware
				{
					MB("Couldn't create main device");
					exit(1000);
				}
			}
		}
	} // End windowed
	else
	{
		// Setup and create fullscreen
		md3dpp.Windowed = false;
		md3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

		if( bHAL)
		{
			gResult = p_mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING, &md3dpp, &p_mDevice); // HW vertex processing
			checkError();
		}
		else
		{
			gResult = p_mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, hWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &md3dpp, &p_mDevice); // SW vertex processing
			if(  gResult != S_OK )
			{
				MB("Couldn't create main device");
				exit(1000);
			}
		}
	} // End fullscreen

	// Vertex buffer for textured quads
	gResult = p_mDevice->CreateVertexBuffer( sizeof(sVertex) * 4, D3DUSAGE_DYNAMIC, TRANS_FVF, D3DPOOL_DEFAULT, &p_mVB, NULL );
	checkError();
	// Vertex buffer for 3D textured quads
	gResult = p_mDevice->CreateVertexBuffer( sizeof(s3DVertex) * 4, D3DUSAGE_DYNAMIC, UNTRANS_FVF, D3DPOOL_DEFAULT, &p_mVB3d, NULL );
	checkError();
	// Vertex buffer for primitives
	gResult = p_mDevice->CreateVertexBuffer( sizeof(sPrimitive) * 4, D3DUSAGE_DYNAMIC, PRIM_FVF, D3DPOOL_DEFAULT, &p_mVBprim, NULL );
	checkError();

	gResult = D3DXCreateSprite(p_mDevice, &p_mSpriteHandler);
	checkError();

	// Setup the viewport
	mViewport.X = 0;
	mViewport.Y = 0;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
	mViewport.MinZ = 0.05f;
	mViewport.MaxZ = 1.0f;
	gResult = p_mDevice->SetViewport(&mViewport);
	checkError();

	// create default font with 18 px
	mDefaultFontDesc.Height          = -18;
	mDefaultFontDesc.Width           = 0;
	mDefaultFontDesc.Weight          = 0;
	mDefaultFontDesc.MipLevels       = 1;
	mDefaultFontDesc.Italic          = false;
	mDefaultFontDesc.CharSet         = DEFAULT_CHARSET;
	mDefaultFontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	mDefaultFontDesc.Quality         = DEFAULT_QUALITY;
	mDefaultFontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(mDefaultFontDesc.FaceName, "Lucida Console");
	gResult = D3DXCreateFontIndirect(p_mDevice, &mDefaultFontDesc, &p_mFont);
	checkError();

	//////////////////////////////////////////////////////////////////////////////
	// Texture and Sampler States Here
	//////////////////////////////////////////////////////////////////////////////

	// Turn culling counter clockwise
	p_mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if(bZbuffer)
		p_mDevice->SetRenderState(D3DRS_ZENABLE, true);
	else
		p_mDevice->SetRenderState(D3DRS_ZENABLE, false);

	// Min and mag filters
	p_mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	p_mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	//////////////////////////////////////////////////////////////////////////////
	// End Texture and Sampler States Here
	//////////////////////////////////////////////////////////////////////////////
}

cPhatDX::cPhatDX()
{
	p_mObject = NULL;
	p_mDevice = NULL;
	p_mVB = p_mVB3d = p_mVBprim = NULL;
	p_mSpriteHandler = NULL;
	p_mFont = NULL;

	b2D = false;
}

cPhatDX::~cPhatDX()
{
	ReleaseCOM(p_mFont);
	ReleaseCOM(p_mSpriteHandler);

	if(p_mDevice)
		p_mDevice->SetStreamSource( 0, NULL, 0, 0 );

	ReleaseCOM(p_mVBprim);
	ReleaseCOM(p_mVB);
	ReleaseCOM(p_mVB3d);

	ReleaseCOM(p_mDevice);

	// Always last
	ReleaseCOM(p_mObject);
}

void cPhatDX::clear(int r, int g, int b)
{
	if(p_mDevice)
	{
		if(bZbuffer)
		{
			p_mDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(r,g,b), 1.0f, 0);
		}
		else
			p_mDevice->Clear(0,NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b), 1.0f, 0);
	}
}

void cPhatDX::beginScene()
{
	gResult = p_mDevice->BeginScene();
	checkError();
}

void cPhatDX::presentScene()
{
	gResult = p_mDevice->EndScene();
	checkError();

	gResult = p_mDevice->Present(NULL, NULL, NULL, NULL);
	checkError();
}

void cPhatDX::begin2D()
{
	if(bZbuffer)
		gResult = p_mSpriteHandler->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT );
	else
		gResult = p_mSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	checkError();
	b2D = true;
}

void cPhatDX::end2D()
{
	gResult = p_mSpriteHandler->End();
	checkError();
	b2D = false;
}

// setVsync()
// Purpose: If vsync value differs from internal value, release all and recreate with proper values
// Bugs: None
void cPhatDX::setVsync(bool tmp)
{
	if( bVsync != tmp ) // different values
	{
		bVsync = tmp;

	ReleaseCOM(p_mFont);
	ReleaseCOM(p_mSpriteHandler);

	if(p_mDevice)
		p_mDevice->SetStreamSource( 0, NULL, 0, 0 );

	ReleaseCOM(p_mVBprim);
	ReleaseCOM(p_mVB);
	ReleaseCOM(p_mVB3d);

	ReleaseCOM(p_mDevice);

	// Always last
	ReleaseCOM(p_mObject);

	createD3D(gWindow, bWindowed, bVsync, bZbuffer);


	enableAlphaBlending();
	enableLighting();
	setAmbient( D3D_WHITE );


	}
}


///////////////////////////////////////////////////////////////////////////////
// All render state flags
///////////////////////////////////////////////////////////////////////////////
void cPhatDX::enableAlphaBlending( DWORD src, DWORD dest )
{
	p_mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	p_mDevice->SetRenderState(D3DRS_SRCBLEND, src);
	p_mDevice->SetRenderState(D3DRS_DESTBLEND, dest);
	p_mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

void cPhatDX::enableAlphaTesting()
{
	p_mDevice->SetRenderState( D3DRS_ALPHATESTENABLE, true );
	p_mDevice->SetRenderState( D3DRS_ALPHAREF, true );
	p_mDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
}

void cPhatDX::disableAlphaBlending()
{
	p_mDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
}

void cPhatDX::disableAlphaTesting()
{
	p_mDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false );
}

void cPhatDX::enableLighting()
{
	p_mDevice->SetRenderState( D3DRS_LIGHTING, true );
	bLighting = true;
}

void cPhatDX::disableLighting()
{
	p_mDevice->SetRenderState( D3DRS_LIGHTING, false );
	bLighting = false;
}

void cPhatDX::enableZ()
{
	p_mDevice->SetRenderState( D3DRS_ZENABLE, true );
	bZbuffer = true;
}

void cPhatDX::disableZ()
{
	p_mDevice->SetRenderState( D3DRS_ZENABLE, false);
	bZbuffer = false;
}
///////////////////////////////////////////////////////////////////////////////
// End render flags
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Lighting
///////////////////////////////////////////////////////////////////////////////

void cPhatDX::enableLight(D3DLIGHT9 &dLight)
{
	p_mDevice->SetLight( gLightIndex, &dLight );
	p_mDevice->LightEnable(gLightIndex, true);
}
void cPhatDX::disableLight(D3DLIGHT9 &dLight)
{
	p_mDevice->SetLight(gLightIndex, &dLight );
	p_mDevice->LightEnable(gLightIndex, false);
}

// Ambient color defaults to off
void cPhatDX::setAmbient( D3DCOLOR ambient )
{
	p_mDevice->SetRenderState( D3DRS_AMBIENT, ambient);
}

void cPhatDX::setAmbient( int iA, int iR, int iG, int iB )
{
	D3DCOLOR tmp = D3DCOLOR_ARGB( iA, iR, iG, iB );
	p_mDevice->SetRenderState( D3DRS_AMBIENT, tmp );
}

///////////////////////////////////////////////////////////////////////////////
// End lighting
///////////////////////////////////////////////////////////////////////////////


// Default to white with 18 pt, Arial left centered w/o clipping
void drawText( IDirect3DDevice9 *device, ID3DXFont *mFont, string sText, int x, int y, D3DCOLOR dColor, int height, string sFont, DWORD dFlags)
{
	if( !sText.empty() ) // dont waste time
	{
		RECT R = {x, y, 0, 0};
		if(height==18)
			gInt = mFont->DrawText( NULL, sText.c_str(), sText.length(), &R, dFlags, dColor);
		
		else // make the requested font.
		{
		//---------------------------
		// make a new font with custom height
		//---------------------------
		ID3DXFont *mTempFont;
		D3DXFONT_DESC mTempDesc;

		mTempDesc.Height          = -height; // Negative height gives approx right pixel height
		mTempDesc.Width           = 0;
		mTempDesc.Weight          = 0;
		mTempDesc.MipLevels       = 1;
		mTempDesc.Italic          = false;
		mTempDesc.CharSet         = DEFAULT_CHARSET;
		mTempDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		mTempDesc.Quality         = DEFAULT_QUALITY;
		mTempDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(mTempDesc.FaceName, sFont.c_str());

		D3DXCreateFontIndirect(device, &mTempDesc, &mTempFont);

		gInt = mTempFont->DrawText( NULL, sText.c_str(), sText.length(), &R, dFlags, dColor);

		ReleaseCOM(mTempFont);
		}
		
		if( gInt == 0 ) // drawtext failed
			throwError("drawText failed.");
	}
}

//Load texture from file with D3DX. Supported formats: BMP, PNG and DDS
IDirect3DTexture9 *loadTexture(IDirect3DDevice9* pDevice, string sFileName, int iWidth, int iHeight, D3DCOLOR colorkey)
{
	IDirect3DTexture9 *d3dTexture = NULL;
	D3DXIMAGE_INFO SrcInfo;         //Optional

	// Load image from file
	gResult = D3DXCreateTextureFromFileEx(pDevice, sFileName.c_str(), iWidth, iHeight, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		colorkey, &SrcInfo, NULL, &d3dTexture);

	checkError();

	//Return the newly made texture
	return d3dTexture;
}

// Draws a box with a default color of white
void drawBox( IDirect3DDevice9 *pDevice, IDirect3DVertexBuffer9 *pBuffer, int iLeft, int iTop, int iRight, int iBottom, D3DCOLOR dColor )
{
	VOID* pVertices;

	sPrimitive g_Vertices[4] =
	{	// Quad (1-2-3-4)
		{ iLeft, iTop, 1.0f, 1.0f, dColor }, // Top Left
		{ iRight, iTop, 1.0f, 1.0f, dColor }, // Top Right
		{  iLeft, iBottom, 1.0f, 1.0f, dColor }, // Bottom left
		{  iRight, iBottom, 1.0f, 1.0f, dColor } // Bottom right
	};

	gResult = pBuffer->Lock( 0, sizeof(g_Vertices), (void**)&pVertices, 0 );
	checkError();

	memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );

	gResult = pBuffer->Unlock();
	checkError();

	gResult = pDevice->SetStreamSource( 0, pBuffer, 0, sizeof( sPrimitive) );
	checkError();

	gResult = pDevice->SetFVF( PRIM_FVF );
	checkError();

	gResult = pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	checkError();

	// Cleanup
	if(pDevice)
		pDevice->SetStreamSource( 0, NULL, 0, 0 );
}
// See Above ^^
void drawBox( IDirect3DDevice9 *pDevice, IDirect3DVertexBuffer9 *pBuffer, RECT rRect, D3DCOLOR dColor )
{
	VOID* pVertices;

	sPrimitive g_Vertices[4] =
	{	// Quad (1-2-3-4)
		{ rRect.left, rRect.top, 1.0f, 1.0f, dColor }, // Top Left
		{ rRect.right, rRect.top, 1.0f, 1.0f, dColor }, // Top Right
		{  rRect.left, rRect.bottom, 1.0f, 1.0f, dColor }, // Bottom left
		{  rRect.right, rRect.bottom, 1.0f, 1.0f, dColor } // Bottom right
	};

	gResult = pBuffer->Lock( 0, sizeof(g_Vertices), &pVertices, 0 );
	checkError();

	memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );

	gResult = pBuffer->Unlock();
	checkError();

	gResult = pDevice->SetStreamSource( 0, pBuffer, 0, sizeof( sPrimitive) );
	checkError();

	gResult = pDevice->SetFVF( PRIM_FVF );
	checkError();

	gResult = pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	checkError();

	// Cleanup
	if(pDevice)
		pDevice->SetStreamSource( 0, NULL, 0, 0 );
}

// Draws a line with a default color of white
void drawLine( IDirect3DDevice9 *pDevice, IDirect3DVertexBuffer9 *pBuffer, int iX1, int iY1, int iX2, int iY2, D3DCOLOR dColor )
{
	VOID* pVertices;

	sPrimitive g_Vertices[2] =
	{
		{ iX1, iY1, 1.0f, 1.0f, dColor }, // start point
		{ iX2, iY2, 1.0f, 1.0f, dColor, }, // end point
	};

	gResult = pBuffer->Lock( 0, sizeof(g_Vertices), (void**)&pVertices, 0 );
	checkError();

	memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );

	gResult = pBuffer->Unlock();
	checkError();

	gResult = pDevice->SetStreamSource( 0, pBuffer, 0, sizeof( sPrimitive) );
	checkError();

	gResult = pDevice->SetFVF( PRIM_FVF );
	checkError();

	gResult = pDevice->DrawPrimitive( D3DPT_LINELIST, 0, 2 );
	checkError();

	// Cleanup
	if(pDevice)
		pDevice->SetStreamSource( 0, NULL, 0, 0 );
}