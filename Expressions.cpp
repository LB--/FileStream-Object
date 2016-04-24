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
//	DuringExpression de {*this};
	return Runtime.CopyString(error_msg.c_str());
}

int Extension::SignedByteAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::int8_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::int8_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::SignedShortAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::int16_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::int16_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::UnsignedByteAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::uint8_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::uint8_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::UnsignedShortAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::uint16_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::uint16_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

int Extension::IntAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::int32_t value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<std::int32_t>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

float Extension::FloatAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	float value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<float>, this, std::placeholders::_1, std::ref(value)
	));
	return value;
}

float Extension::DoubleAt(int slot, unsigned position)
{
	DuringExpression de {*this};
	double value {};
	safe_seekg(slot, position, std::bind
	(
		&Extension::read_primitive<double>, this, std::placeholders::_1, std::ref(value)
	));
	return static_cast<float>(value);
}

TCHAR const *Extension::String8At(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::string str;
	safe_seekg(slot, position, [=, &str](Slots_t::iterator it)
	{
		std::getline(it->second, str, '\0');
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading null-terminated UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " bytes were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read null-terminated UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " bytes were read");
		}
	});
	return Runtime.CopyString(str_to16fr8(str).c_str());
}

TCHAR const *Extension::String16At(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::wstring str;
	safe_seekg(slot, position, [=, &str](Slots_t::iterator it)
	{
		for(TCHAR c = _T('\0')
		;   it->second.read(reinterpret_cast<char *>(&c), sizeof(TCHAR)), it->second.good() && c != _T('\0')
		;   str += c)
		{
		}
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading null-terminated UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " code points were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read null-terminated UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " code points were read");
		}
	});
	return Runtime.CopyString(str.c_str());
}

TCHAR const *Extension::SizedString8At(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::string str;
	if(auto size = static_cast<std::uint32_t>(IntAt(slot, position)))
	{
		safe_seekg(slot, position + sizeof(std::uint32_t), [=, &str](Slots_t::iterator it)
		{
			auto buf = std::make_unique<std::string::value_type[]>(size);
			it->second.read(buf.get(), size);
			str.assign(buf.get(), static_cast<std::size_t>(it->second.gcount()));
			if(it->second.eof())
			{
				generate_error("Reached end of file while reading length-terminated UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " bytes were read");
			}
			else if(!it->second.good())
			{
				generate_error("Could not read length-terminated UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " bytes were read");
			}
		});
	}
	return Runtime.CopyString(str_to16fr8(str).c_str());
}

TCHAR const *Extension::SizedString16At(int slot, unsigned position)
{
	DuringExpression de {*this};
	std::wstring str;
	if(auto size = static_cast<std::uint32_t>(IntAt(slot, position)))
	{
		safe_seekg(slot, position + sizeof(std::uint32_t), [=, &str](Slots_t::iterator it)
		{
			for(TCHAR c = _T('\0')
			;   str.size() < size && (it->second.read(reinterpret_cast<char *>(&c), sizeof(TCHAR)), it->second.good())
			;   str += c)
			{
			}
			if(it->second.eof())
			{
				generate_error("Reached end of file while reading length-terminated UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " code points were read");
			}
			else if(!it->second.good())
			{
				generate_error("Could not read length-terminated UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " code points were read");
			}
		});
	}
	return Runtime.CopyString(str.c_str());
}

TCHAR const *Extension::CustomString8At(int slot, unsigned position, unsigned size)
{
	DuringExpression de {*this};
	std::string str;
	safe_seekg(slot, position + sizeof(std::uint32_t), [=, &str](Slots_t::iterator it)
	{
		auto buf = std::make_unique<std::string::value_type[]>(size);
		it->second.read(buf.get(), size);
		str.assign(buf.get(), static_cast<std::size_t>(it->second.gcount()));
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading custom-size UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " bytes were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read custom-size UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " bytes were read");
		}
	});
	return Runtime.CopyString(str_to16fr8(str).c_str());
}

TCHAR const *Extension::CustomString16At(int slot, unsigned position, unsigned size)
{
	DuringExpression de {*this};
	std::wstring str;
	safe_seekg(slot, position + sizeof(std::uint32_t), [=, &str](Slots_t::iterator it)
	{
		std::uint32_t i = 0;
		for(TCHAR c = _T('\0')
		;   it->second.read(reinterpret_cast<char *>(&c), sizeof(TCHAR)), it->second.good() && i < size
		;   str += c, ++i)
		{
		}
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading custom-size UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " code points were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read custom-size UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " code points were read");
		}
	});
	return Runtime.CopyString(str.c_str());
}

unsigned Extension::ReadCursorPos(int slot)
{
	DuringExpression de {*this};
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
	DuringExpression de {*this};
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
	DuringExpression de {*this};
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
