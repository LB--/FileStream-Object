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
		auto it = slots.find(slot);
		if(it != std::end(slots))
		{
			slots.erase(it);
		}
		slots.emplace(SlotId{slot, filepath, open_flags}, std::move(file));
	}
	else
	{
		return generate_error("Unable to open file stream on \"", str_to8fr16(filepath), "\" for slot ", slot);
	}
}

void Extension::CloseStream(int slot)
{
	auto it = slots.find(slot);
	if(it != std::end(slots))
	{
		slots.erase(it);
	}
}

void Extension::SetByte(int slot, unsigned position, int value)
{
	return safe_seekp(slot, position, std::bind
	(
		&Extension::write_primitive<std::int8_t>, this, std::placeholders::_1, static_cast<std::int8_t>(value)
	));
}

void Extension::SetShort(int slot, unsigned position, int value)
{
	return safe_seekp(slot, position, std::bind
	(
		&Extension::write_primitive<std::int16_t>, this, std::placeholders::_1, static_cast<std::int16_t>(value)
	));
}

void Extension::SetInt(int slot, unsigned position, int value)
{
	return safe_seekp(slot, position, std::bind
	(
		&Extension::write_primitive<std::int32_t>, this, std::placeholders::_1, value
	));
}

void Extension::SetFloat(int slot, unsigned position, float value)
{
	return safe_seekp(slot, position, std::bind
	(
		&Extension::write_primitive<float>, this, std::placeholders::_1, value
	));
}

void Extension::SetDouble(int slot, unsigned position, float value)
{
	return safe_seekp(slot, position, std::bind
	(
		&Extension::write_primitive<double>, this, std::placeholders::_1, value
	));
}

void Extension::SetString8(int slot, unsigned position, TCHAR const *str, int nullterm)
{
	return safe_seekp(slot, position, [=](Slots_t::iterator it)
	{
		auto s = str_to8fr16(str);
		it->second.write(s.c_str(), s.size()+(nullterm? 1 : 0));
		if(!it->second.good())
		{
			return generate_error("Could not write UTF-8 string to position ", position, " for slot ", slot);
		}
	});
}

void Extension::SetString16(int slot, unsigned position, TCHAR const *str, int nullterm)
{
	return safe_seekp(slot, position, [=](Slots_t::iterator it)
	{
		it->second.write(reinterpret_cast<char const *>(str), (std::wcslen(str)+(nullterm? 1 : 0))*sizeof(TCHAR));
		if(!it->second.good())
		{
			return generate_error("Could not write UTF-16 string to position ", position, " for slot ", slot);
		}
	});
}

void Extension::SetSizedString8(int slot, unsigned position, TCHAR const *str)
{
	return safe_seekp(slot, position, [=](Slots_t::iterator it)
	{
		auto s = str_to8fr16(str);
		std::uint32_t size = s.size();
		it->second.write(reinterpret_cast<char const *>(&size), sizeof(std::uint32_t));
		it->second.write(s.c_str(), s.size());
		if(!it->second.good())
		{
			return generate_error("Could not write UTF-8 string to position ", position, " for slot ", slot);
		}
	});
}

void Extension::SetSizedString16(int slot, unsigned position, TCHAR const *str)
{
	return safe_seekp(slot, position, [=](Slots_t::iterator it)
	{
		std::uint32_t size = std::wcslen(str);
		it->second.write(reinterpret_cast<char const *>(&size), sizeof(std::uint32_t));
		it->second.write(reinterpret_cast<char const *>(str), size*sizeof(TCHAR));
		if(!it->second.good())
		{
			return generate_error("Could not write UTF-16 string to position ", position, " for slot ", slot);
		}
	});
}

void Extension::ClearError(int slot)
{
	return generate_error("The 'clear error state' action was removed in v1.1.0, please don't use it anymore");
}

void Extension::FromMemory(int slot, unsigned position, void const *memory, unsigned size)
{
	return safe_seekp(slot, position, [=](Slots_t::iterator it)
	{
		it->second.write(reinterpret_cast<char const *>(memory), size);
		if(!it->second.good())
		{
			return generate_error("Could not write from memory to position ", position, " for slot ", slot);
		}
	});
}

void Extension::ToMemory(int slot, unsigned position, void *memory, unsigned size)
{
	return safe_seekg(slot, position, [=](Slots_t::iterator it)
	{
		it->second.read(reinterpret_cast<char *>(memory), size);
		if(!it->second.good())
		{
			if(it->second.eof())
			{
				return generate_error("End of file reached in slot ", slot, " while reading to memory from position ", position, "; ", it->second.gcount(), " out of ", size, " bytes read");
			}
			return generate_error("Could not read to memory from position ", position, " for slot ", slot);
		}
	});
}
