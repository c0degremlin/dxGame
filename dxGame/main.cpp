#include "main.h"


void init()
{
	gfx.enableAlphaBlending();
	gfx.enableLighting();
	gfx.setAmbient( D3D_GREEN );


	gfx.enableLight(white_light.getLight());
	gfx.p_mDevice->SetMaterial(&white_light.getMat());
	white_light.createPoint();
	gfx.enableLight( white_light.getLight() );
	gfx.p_mDevice->SetMaterial( &white_light.getMat() );

	tex.loadSheet( gfx.p_mDevice, "res\\one.dds", 32, 32);
	tex.moveTo( 50.0f, 50.0f );
	tex.setScale(2.5f);

	tex2.loadSheet( gfx.p_mDevice, "res\\five.dds", 32, 32);
	tex2.moveTo( 250.0f, 50.0f );

	

	butt0.init( "Hello World", 50, 50, 84, 31 );

	butt0.setColor( D3D_PINK );
	butt0.setTextColor( D3D_WHITE );
}

void loop()
{
	// logic here


	if (!con.getHidden())
	{
		QueryPerformanceCounter(&timeStart); // Start timing

		gfx.clear(0, 25, 255);
		gfx.beginScene();

		// Draw background

		//// Draw player and npcs
		tex.draw(gfx.p_mDevice, gfx.p_mVB3d);
		tex2.draw(gfx.p_mDevice, gfx.p_mVB3d);


		// Draw final layer

		// Draw HUD and Text
		butt0.draw(gfx.p_mDevice, gfx.p_mVB, gfx.p_mFont, true);

		if (!con.getHidden())
			con.draw(gfx.p_mDevice, gfx.p_mVBprim, gfx.p_mFont);



		eye.Update(gfx.p_mDevice);

		gfx.presentScene();
		QueryPerformanceCounter(&timeEnd); // End timing
	}

	//anim_rate = ( (float)timeEnd.QuadPart - (float)timeStart.QuadPart ) / timerFreq.QuadPart;
}

void shutdown()
{
}

void doInput(WPARAM wparam)
{
	if( wparam == VK_GUAVE)
	{
		gBool = con.getHidden();
		gBool = !gBool;
		con.setHidden(gBool);
	}

	if( wparam ==  VK_ESCAPE)
		DestroyWindow(gWindow);

	if( con.getHidden() ) // if console isnt being used
	{
		switch(wparam)
		{
		case 'W':
			tex.moveY(-step);
			tex.incFrame();
			tex2.moveY(-step);
			tex2.incFrame();
			break;
		case 'S':
			tex.moveY();
			tex.incFrame();
			tex2.moveY();
			tex2.incFrame();
			break;
		case 'A':
			tex.moveX(-step);
			tex.incFrame();
			tex2.moveX(-step);
			tex2.incFrame();
			break;
		case 'D':
			tex.moveX();
			tex.incFrame();
			tex2.moveX();
			tex2.incFrame();
			break;
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPreviousInstance,
				   LPSTR lpcmdline,
				   int nCmdShow)
{
	HWND hWnd = NULL;
	WNDCLASSEX winclass;

	gWindow = NULL;

	ZeroMemory(&winclass, sizeof(winclass));

	// first fill in the window class structure
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProcedure;
	winclass.hInstance		= hInstance;
	winclass.hCursor		= LoadCursor(0, IDC_ARROW);
	winclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(ICON_DX));
	winclass.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(ICON_DX));
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName = WINDOW_CLASS;

	gInstance = hInstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
		MB("Couldn't register window");

	window_rect.top = 0; window_rect.left = 0;
	window_rect.right = WINDOW_WIDTH;
	window_rect.bottom = WINDOW_HEIGHT;

	if(winclass.lpszMenuName != NULL)
		AdjustWindowRect(&window_rect, WINDOW_STYLE, true);    // adjust the size
	else
		AdjustWindowRect(&window_rect, WINDOW_STYLE, false);    // adjust the size

	// create the main window
	hWnd = CreateWindowEx(NULL, WINDOW_CLASS, WINDOW_TITLE, WINDOW_STYLE,
		( (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH)/2),( (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) /2),	  // Center the screen
		window_rect.right - window_rect.left ,window_rect.bottom - window_rect.top,  // initial width, height
		NULL, NULL, gInstance, NULL);

	if( hWnd == NULL)
		MB("Couldn't create window");

	gWindow = hWnd;

	ShowWindow( gWindow, SW_SHOW );
	UpdateWindow( gWindow );

	// Load DX here
	gfx.createD3D( gWindow );

	init();

	//main program loop
	while(true)
	{
		QueryPerformanceFrequency(&timerFreq); // update timing freq

		// test if there is a message in queue, if so get it
		if (PeekMessage(&gMessage,NULL,0,0,PM_REMOVE))
		{
			// test if this is a quit
			if (gMessage.message == WM_QUIT)
				break;
			// translate any accelerator keys
			TranslateMessage(&gMessage);

			// send the message to the window proc
			DispatchMessage(&gMessage);
		} // end if

		loop();
	} // end while

	shutdown();
	// return to Windows like this
	return(gMessage.wParam);
}

LRESULT CALLBACK WindowProcedure(HWND hWindow,
								 UINT uMessage,
								 WPARAM wparam,
								 LPARAM lparam)
{

	char tempBuffer[128];
	switch(uMessage)
	{
	case WM_COMMAND:
			// Menu stuff here
			switch( LOWORD(wparam) )
			{
			case ID_FILE_NEWGAME:
				break;
			case ID_FILE_LOADGAME:
				break;
			case ID_FILE_SAVEGAME:
				break;
			case ID_FILE_EXIT:
				DestroyWindow(hWindow);
				break;
			case ID_OPTIONS_DIFFICULTY:
				break;
			case ID_OPTIONS_GRAPHICS:
				break;
			case ID_HELP_HOWTOPLAY:
				break;
			case ID_HELP_SCORING:
				break;
			case ID_HELP_ABOUT:
				break;
			}
		break;

		///////////////////////////////////////////////////////////////////////
		// Keyboard input win32 style
		///////////////////////////////////////////////////////////////////////
	case WM_KEYDOWN:
			doInput(wparam);
			con.processInput(wparam);
		break;

	case WM_KEYUP:
		switch(wparam)
		{
		case VK_SHIFT:
			con.setShift(false);
			break;
		}
		break;

		///////////////////////////////////////////////////////////////////////////
		// Mouse Input win32 style
		///////////////////////////////////////////////////////////////////////////
	case WM_MOUSEMOVE:	// get the position of the mouse
		gMouseX = GET_X_LPARAM(lparam);
		gMouseY = GET_Y_LPARAM(lparam);
		break;
	case WM_LBUTTONDOWN:
		if (butt0.getClicks(gMouseX, gMouseY))
			MB("HAHA");
		break;
	case WM_LBUTTONUP:
		gMouseLeftDown = false;
		break;
	case WM_RBUTTONDOWN:
		gMouseRightDown = true;
		break;
	case WM_RBUTTONUP:
		gMouseRightDown = false;
		break;
		///////////////////////////////////////////////////////////////////////////
		// End mouse input
		///////////////////////////////////////////////////////////////////////////

	case WM_DESTROY:
		shutdown();
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	//handle remaining messages
	return DefWindowProc(hWindow,uMessage,wparam,lparam);
} //WindowProcedure


/* BONEYARD *******************************************************************

*/