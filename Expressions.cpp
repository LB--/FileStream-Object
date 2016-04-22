/* Expressions.cpp
 * This is where you should define the
 * behavior of your expressions. Make sure
 * the parameters and return types match those
 * in the JSON exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

TCHAR const *Extension::GetError()
{
	return Runtime.CopyString(error_msg.c_str());
}

int Extension::SignedByteAt(int slot, unsigned position)
{
	//
	return 0;
}

int Extension::SignedShortAt(int slot, unsigned position)
{
	//
	return 0;
}

int Extension::UnsignedByteAt(int slot, unsigned position)
{
	//
	return 0;
}

int Extension::UnsignedShortAt(int slot, unsigned position)
{
	//
	return 0;
}

int Extension::IntAt(int slot, unsigned position)
{
	//
	return 0;
}

float Extension::FloatAt(int slot, unsigned position)
{
	//
	return 0.0f;
}

float Extension::DoubleAt(int slot, unsigned position)
{
	//
	return 0.0f;
}

TCHAR const *Extension::String8At(int slot, unsigned position)
{
	//
	return _T("");
}

TCHAR const *Extension::String16At(int slot, unsigned position)
{
	//
	return _T("");
}

unsigned Extension::ReadCursorPos(int slot)
{
	//
	return 0u;
}

unsigned Extension::WriteCursorPos(int slot)
{
	//
	return 0u;
}

unsigned Extension::FileSize(int slot)
{
	//
	return 0u;
}
