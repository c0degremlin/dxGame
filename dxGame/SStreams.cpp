#include "SStreams.h"

// Converts an int to string C++ way
void cStringStream::numberToString(int iIn, string &sOut)
{
	mInputStream.clear(); // just to be sure
	mOutputStream.str(""); // empty string

	mOutputStream << iIn;

	sOut = mOutputStream.str();
}

// Converts a short to string C++ way
void cStringStream::shortToString(short iIn, string &sOut)
{
	mInputStream.clear(); // just to be sure
	mOutputStream.str("");// empty string

	mOutputStream << iIn;

	sOut = mOutputStream.str();
}

// Converts a float to string C++ way
void cStringStream::floatToString(float iIn, string &sOut)
{
	mInputStream.clear(); // just to be sure
	mOutputStream.str("");// empty string

	mOutputStream << iIn;

	sOut = mOutputStream.str();
}

// converts a string to an int C++ way
void cStringStream::stringToNumber(string sIn, int &iOut)
{
	mInputStream.clear(); // just to be sure
	mInputStream.str(sIn);

	if ( ! ( mInputStream >> iOut ) ) // if string is invald i.e. not numbers
	{
		iOut = 0;
	}
}

// converts a string to a short c++ way
void cStringStream::stringToShort(string sIn, sint &iOut)
{
	mInputStream.clear(); // just to be sure
	mInputStream.str(sIn);

	if ( ! ( mInputStream >> iOut ) ) // if string is invald i.e. not numbers
	{
		iOut = 0;
	}
}

// converts a string to a float c++ way
void cStringStream::stringToFloat(string sIn, float &iOut)
{
	mInputStream.clear(); // just to be sure
	mInputStream.str(sIn);

	if ( ! ( mInputStream >> iOut ) ) // if string is invald i.e. not numbers
	{
		iOut = 0.0f;
	}
}