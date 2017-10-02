#pragma once


#define WIN32_LEAN_AND_MEAN		    // Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete [] (x); (x) = NULL; }
#define MSG_RETURN(x,s) if (!(x)) { MessageBox( 0, s, 0, 0 ); return FALSE; }
#define RETURN(x) if (!(x)) { return FALSE; }

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectX Header Files
#include <d3d9.h>
#include <d3dx9.h>
