///////////////////////////////////////////////////////////////////////////////
// Filename: SStreams.h
// Purpose: Encapsulate string streams
// Created by: Shane Laffond
// Contact info: slaffond1990@gmail.com
//
// Bugs:
//		None
// Todo:
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <sstream>

using std::istringstream;
using std::ostringstream;
using std::string;

typedef short int sint;
typedef unsigned short int usint;
typedef unsigned int uint;

static int gInt;
static string gString;
static sint gShort;
static bool gBool;

class cStringStream
{
private:
	istringstream mInputStream;
	ostringstream mOutputStream;
public:
	void numberToString(int iIn, string &sOut);
	void shortToString(sint iIn, string &sOut);
	void floatToString(float iIn, string &sOut);

	void stringToNumber(string sIn, int &iOut);
	void stringToShort(string sIn, sint &iOut);
	void stringToFloat(string sIn, float &iOut);
};
