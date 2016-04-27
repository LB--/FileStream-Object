/* Conditions.cpp
 * This is where you should define the
 * behavior of your conditions. Make sure
 * the parameters and return types match those
 * in the JSON exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

bool Extension::OnError()
{
	return true;
}

bool Extension::IsOpen(int slot)
{
	auto const it = std::as_const(slots).find(slot);
	return it != std::cend(slots) && it->second.is_open();
}

bool Extension::IsOk(int slot)
{
	auto const it = std::as_const(slots).find(slot);
	return it != std::cend(slots) && !it->second.bad();
}

bool Extension::IsEof(int slot)
{
	auto const it = slots.find(slot);
	if(it != std::end(slots) && safe_helper(it))
	{
		return it->second.peek() == std::fstream::traits_type::eof();
	}
	return false;
}

bool Extension::FileExists(TCHAR const *filepath)
{
	auto const it = std::as_const(slots).find(filepath);
	if(it != std::cend(slots))
	{
		if(it->second.is_open())
		{
			return true;
		}
	}
	return bool(std::ifstream{filepath});
}
