///////////////////////////////////////////////////////////////////////////////
// Filename: input.h
// Purpose: Provide a 'framework' for input via win32
// Created by: Shane Laffond
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		None
// Todo:
//		maybe use something more sophisticated
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "log.h"

static int gMouseX;
static int gMouseY;

static bool gMouseLeftDown;
static bool gMouseRightDown;

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
