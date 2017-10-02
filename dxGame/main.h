///////////////////////////////////////////////////////////////////////////////
// Filename: windoze.h
// Purpose: Provide a framework for win32
// Created by: Shane Laffond
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		None
// Todo:
//		None
///////////////////////////////////////////////////////////////////////////////
#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define STRICT

#include <Windows.h>
#include <windowsx.h>
#include "resource.h"
#include "phatDX.h" // for menu
#include "graphics.h"
#include "input.h"
#include "camera.h"
#include "button.h"
#include "light.h"
#include "console.h"
#include "sprite.h"

// Global handles
MSG gMessage;
HWND gWindow;
HINSTANCE gInstance;

HWND hHTP; // How to Play dialog
HWND hScoring; // Scoring dialog

RECT window_rect; // Actual drawing client area used by AdjustClientRect

				  // Timing
LARGE_INTEGER timeStart;
LARGE_INTEGER timeEnd;
LARGE_INTEGER numCounts;
LARGE_INTEGER timerFreq;

cPhatDX gfx;
phatConsole con;

OrthoCamera eye;

phatSprite tex;
phatSprite tex2;

phatButton butt0;

phatLight white_light;

const LPCTSTR WINDOW_TITLE = "Phat DX";
const LPCTSTR WINDOW_CLASS = "DXWINDOW0";

const DWORD WINDOW_STYLE = WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_EX_TOPMOST | WS_VISIBLE;

//function prototypes
void init();
void loop();
void shutdown();
void doInput(WPARAM wparam); // for external kb input

LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);
