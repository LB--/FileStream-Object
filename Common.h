/* Common.h
 * This is the common header file included
 * by almost all other *.cpp files in
 * the project. Anything that gets put or
 * included in here will be available to you
 * everywhere else. You shouldn't need to
 * change anything in this file except
 * to include headers that you want and
 * to change the edition of Fusion you want
 * to support.
 */

#pragma once

//Only define one of these at once:
//	#define TGFEXT	//TGF2.0, MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
	#define MMFEXT	//        MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
//	#define PROEXT	//                        MMF2.0 Dev, CTF2.5 Dev

#include "Edif.h"
#include "Resource.h"

#include <bitset>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <type_traits>

using string8  = std::string;
using string16 = std::wstring;

inline string8 str_to8fr16(string16 const &s)
{
	if(s.size() == 0) return string8();
	int r1 = WideCharToMultiByte
	(
		CP_UTF8,
		0,
		reinterpret_cast<wchar_t const *>(s.c_str()),
		s.size(),
		NULL,
		0,
		NULL,
		NULL
	);
	if(r1 == 0) return string8();
	std::unique_ptr<string8::value_type[]> buf {new string8::value_type[r1]};
	int r2 = WideCharToMultiByte
	(
		CP_UTF8,
		0,
		reinterpret_cast<wchar_t const *>(s.c_str()),
		s.size(),
		buf.get(),
		r1,
		NULL,
		NULL
	);
	if(r2 == 0) return string8();
	return string8(buf.get(), r2);
}
inline string16 str_to16fr8(string8 const &s)
{
	if(s.size() == 0) return string16();
	int r1 = MultiByteToWideChar
	(
		CP_UTF8,
		0,
		s.c_str(),
		s.size(),
		NULL,
		0
	);
	if(r1 == 0) return string16();
	std::unique_ptr<string16::value_type[]> buf {new string16::value_type[r1]};
	int r2 = MultiByteToWideChar
	(
		CP_UTF8,
		0,
		s.c_str(),
		s.size(),
		reinterpret_cast<wchar_t *>(buf.get()),
		r1
	);
	if(r2 == 0) return string16();
	return string16(buf.get(), r2);
}

/* stdtstring
 * A std::string that knows if you're using
 * unicode or not. (Protip: strings passed
 * to your extension A/C/Es still need to be
 * const TCHAR *, and the same goes for strings
 * returned from expressions and conditions).
 */
typedef std::basic_string<TCHAR> stdtstring;

#undef min
#undef max

#include "EditData.hpp"
#include "Extension.hpp"
