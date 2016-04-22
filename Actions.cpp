/* Actions.cpp
 * This is where you should define the
 * behavior of your actions. Make sure
 * the parameters match those in the JSON
 * exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

void Extension::OpenStream(TCHAR const *filepath, int slot, int flags)
{
	std::ios::openmode open_flags = std::ios::binary;
	if(flags & 1){ open_flags |= std::ios::in; }
	if(flags & 2){ open_flags |= std::ios::out; }
	if(flags & 4){ open_flags |= std::ios::trunc; }
	if(flags & 8){ open_flags |= std::ios::app; }
	if(std::fstream file {filepath, open_flags})
	{
		auto it = std::as_const(slots).find(slot);
		if(it != std::cend(slots))
		{
			slots.erase(it);
		}
		slots.emplace(SlotId{slot, filepath}, std::move(file));
	}
	else
	{
		generate_error("Unable to open file stream on \"", filepath, "\"");
	}
}

void Extension::CloseStream(int slot)
{
	auto it = std::as_const(slots).find(slot);
	if(it != std::cend(slots))
	{
		slots.erase(it);
	}
}

void Extension::SetByte(int slot, unsigned position, int value)
{
	//
}

void Extension::SetShort(int slot, unsigned position, int value)
{
	//
}

void Extension::SetInt(int slot, unsigned position, int value)
{
	//
}

void Extension::SetFloat(int slot, unsigned position, float value)
{
	//
}

void Extension::SetDouble(int slot, unsigned position, float value)
{
	//
}

void Extension::SetString8(int slot, unsigned position, TCHAR const *str, int nullterm)
{
	//
}

void Extension::SetString16(int slot, unsigned position, TCHAR const *str, int nullterm)
{
	//
}

void Extension::ClearError(int slot)
{
	//
}

void Extension::FromMemory(int slot, unsigned position, void const *memory, unsigned size)
{
	//
}

void Extension::ToMemory(int slot, unsigned position, void *memory, unsigned size)
{
	//
}
