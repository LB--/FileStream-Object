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
	std::int8_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::int8_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::SignedShortAt(int slot, unsigned position)
{
	std::int16_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::int16_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::UnsignedByteAt(int slot, unsigned position)
{
	std::uint8_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::uint8_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::UnsignedShortAt(int slot, unsigned position)
{
	std::uint16_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::uint16_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::IntAt(int slot, unsigned position)
{
	std::int32_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::int32_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

float Extension::FloatAt(int slot, unsigned position)
{
	float value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<float>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

float Extension::DoubleAt(int slot, unsigned position)
{
	double value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<double>, this, std::placeholders::_1, std::ref(value)
	));
	return static_cast<float>(value);
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
	auto it = slots.find(slot);
	if(it != std::end(slots))
	{
		if(safe_helper(it))
		{
			return static_cast<unsigned>(it->second.tellg());
		}
		return 0u;
	}
	return generate_error("Slot ", slot, " does not exist"), 0u;
}

unsigned Extension::WriteCursorPos(int slot)
{
	auto it = slots.find(slot);
	if(it != std::end(slots))
	{
		if(safe_helper(it))
		{
			return static_cast<unsigned>(it->second.tellp());
		}
		return 0u;
	}
	return generate_error("Slot ", slot, " does not exist"), 0u;
}

unsigned Extension::FileSize(int slot)
{
	auto it = slots.find(slot);
	if(it != std::end(slots))
	{
		if(safe_helper(it))
		{
			if(it->first.flags & std::ios::in)
			{
				auto oldpos = it->second.tellg();
				it->second.seekg(std::ios::end);
				auto sizepos = it->second.tellg();
				it->second.seekg(oldpos);
				if(it->second.good())
				{
					return static_cast<unsigned>(sizepos);
				}
			}
			if(it->first.flags & std::ios::out)
			{
				auto oldpos = it->second.tellp();
				it->second.seekp(std::ios::end);
				auto sizepos = it->second.tellp();
				it->second.seekp(oldpos);
				if(it->second.good())
				{
					return static_cast<unsigned>(sizepos);
				}
			}
			return generate_error("Unknown error while trying to determine file size of slot ", slot), 0u;
		}
		return 0u;
	}
	return generate_error("Slot ", slot, " does not exist"), 0u;
}
