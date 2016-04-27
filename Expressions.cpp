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

TCHAR const *Extension::String8At(int slot, unsigned position, int bytes)
{
	DuringExpression de {*this};
	std::string str;
	safe_seekg(slot, position, [=, &str](Slots_t::iterator it)
	{
		if(bytes == -1)
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
		}
		else
		{
			std::uint32_t size = static_cast<std::uint32_t>(bytes);
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
		}
	});
	return Runtime.CopyString(str_to16fr8(str).c_str());
}

TCHAR const *Extension::String16At(int slot, unsigned position, int code_points)
{
	DuringExpression de {*this};
	std::wstring str;
	safe_seekg(slot, position, [=, &str](Slots_t::iterator it)
	{
		if(code_points == -1)
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
		}
		else
		{
			std::uint32_t size = static_cast<std::uint32_t>(code_points);
			for(TCHAR c = _T('\0')
			;   str.size() < size && (it->second.read(reinterpret_cast<char *>(&c), sizeof(TCHAR)), it->second.good())
			;   str += c)
			{
			}
			if(it->second.eof())
			{
				generate_error("Reached end of file while reading custom-size UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " bytes were read");
			}
			else if(!it->second.good())
			{
				generate_error("Could not read custom-size UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " of ", size, " bytes were read");
			}
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

TCHAR const *Extension::StringChars8At(int slot, unsigned position, unsigned chars)
{
	if(chars == 0)
	{
		return _T("");
	}
	DuringExpression de {*this};
	std::string str;
	safe_seekg(slot, position, [=, &str](Slots_t::iterator it)
	{
		std::size_t chars_read {};
		do
		{
			auto character = read_char<std::string::value_type>(it->second);
			if(!character.empty())
			{
				++chars_read;
				str += character;
			}
		} while(it->second.good() && chars_read < chars);
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading UTF-8 string from position ", position, " for slot ", slot, "; only ", chars_read, " of ", chars, " characters were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read UTF-8 string from position ", position, " for slot ", slot, "; only ", chars_read, " of ", chars, " characters were read");
		}
	});
	return Runtime.CopyString(str_to16fr8(str).c_str());
}

TCHAR const *Extension::StringChars16At(int slot, unsigned position, unsigned chars)
{
	if(chars == 0)
	{
		return _T("");
	}
	DuringExpression de {*this};
	std::wstring str;
	safe_seekg(slot, position, [=, &str](Slots_t::iterator it)
	{
		std::size_t chars_read {};
		do
		{
			auto character = read_char<std::wstring::value_type>(it->second);
			if(!character.empty())
			{
				++chars_read;
				str += character;
			}
		} while(it->second.good() && chars_read < chars);
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading UTF-16 string from position ", position, " for slot ", slot, "; only ", chars_read, " of ", chars, " characters were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read UTF-16 string from position ", position, " for slot ", slot, "; only ", chars_read, " of ", chars, " characters were read");
		}
	});
	return Runtime.CopyString(str.c_str());
}

TCHAR const *Extension::StringUntil8At(int slot, unsigned position, TCHAR const *sentry)
{
	DuringExpression de {*this};
	std::string const sentry_str = str_to8fr16(sentry);
	if(sentry_str.empty())
	{
		generate_error("Empty sentry when reading UTF-8 string from position ", position, " for slot ", slot);
		return _T("");
	}
	std::string str;
	safe_seekg(slot, position, [=, &sentry_str, &str](Slots_t::iterator it)
	{
		{ //initially read the length of the sentry (if we read less, the stream will be in an error state)
			auto buf = std::make_unique<std::string::value_type[]>(sentry_str.size());
			it->second.read(buf.get(), sentry_str.size());
			str.assign(buf.get(), static_cast<std::size_t>(it->second.gcount()));
		}
		//then loop until the end of the string matches the sentry (or error)
		auto next = std::fstream::traits_type::eof();
		while(it->second.good()
		&&    !std::equal(std::crbegin(sentry_str), std::crend(sentry_str), std::crbegin(str))
		&&    (next = it->second.get()) != std::fstream::traits_type::eof())
		{
			str += static_cast<char>(next);
		}
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " bytes were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read UTF-8 string from position ", position, " for slot ", slot, "; only ", str.size(), " bytes were read");
		}
		else //success, chop off the sentry
		{
			str.resize(str.size() - sentry_str.size());
		}
	});
	return Runtime.CopyString(str_to16fr8(str).c_str());
}

TCHAR const *Extension::StringUntil16At(int slot, unsigned position, TCHAR const *sentry)
{
	DuringExpression de {*this};
	stdtstring sentry_str = sentry;
	if(sentry_str.empty())
	{
		generate_error("Empty sentry when reading UTF-16 string from position ", position, " for slot ", slot);
		return _T("");
	}
	std::wstring str;
	safe_seekg(slot, position, [=, &sentry_str, &str](Slots_t::iterator it)
	{
		//initially read the length of the sentry (if we read less, the stream will be in an error state)
		for(TCHAR c = _T('\0')
		;   str.size() < sentry_str.size() && (it->second.read(reinterpret_cast<char *>(&c), sizeof(TCHAR)), it->second.good())
		;   str += c)
		{
		}
		//then loop until the end of the string matches the sentry (or error)
		for(TCHAR c = _T('\0')
		;   it->second.good()
		&&  !std::equal(std::crbegin(sentry_str), std::crend(sentry_str), std::crbegin(str))
		&&  (it->second.read(reinterpret_cast<char *>(&c), sizeof(TCHAR)), it->second.good())
		;   str += c)
		{
		}
		if(it->second.eof())
		{
			generate_error("Reached end of file while reading UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " code points were read");
		}
		else if(!it->second.good())
		{
			generate_error("Could not read UTF-16 string from position ", position, " for slot ", slot, "; only ", str.size(), " code points were read");
		}
		else //success, chop off the sentry
		{
			str.resize(str.size() - sentry_str.size());
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
				it->second.seekg(0, std::ios::end);
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
				it->second.seekp(0, std::ios::end);
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
