///////////////////////////////////////////////////////////////////////////////
// Filename: log.h
// Purpose: creates a class for logging anything
// Created by: Shane Laffond
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		None
// Todo:
//		Add more stuff maybe?
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <dxerr.h>
#include "SStreams.h"

using std::string;

inline void MB( string sText, string sTitle = "Error" ) { MessageBox(0, sText.c_str(), sTitle.c_str(), MB_OK); }

static HRESULT gResult;

#ifdef _DEBUG
#pragma comment( lib, "dxerr.lib" )
inline void checkError( HRESULT hError = NULL)
{
	if( hError != NULL || hError != S_OK )
	{
		string tmp = "Error: ";
		tmp.append( DXGetErrorString( hError ) );
		tmp.append("\n");
		OutputDebugString( tmp.c_str() );
	}
	else if ( gResult != S_OK )
	{
		string tmp = "Error: ";
		tmp.append( DXGetErrorString( gResult ) );
		tmp.append("\n");
		OutputDebugString( tmp.c_str() );
	}
}
inline void throwError(string sError)
{
	sError.append("\n");
	OutputDebugString(sError.c_str());
}
#endif
#ifndef _DEBUG
inline void checkError(HRESULT hError ) { } // Supposed to be empty for release builds
inline void throwError(string sError) { }
#endif