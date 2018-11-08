// File:  utilities.cpp
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Utility methods.

// Local headers
#include "utilities.h"

// Standard C++ headers
#include <sstream>
#include <iomanip>
#include <cassert>

namespace Utilities
{

bool LineHasTimestamp(const std::string& line, const std::string& timestampFormat)
{
	const std::string format(GetFormatBeforeMilliseconds(timestampFormat));
	std::istringstream ss(line);
	std::tm tm;
	return !(ss >> std::get_time(&tm, format.c_str())).fail();// TODO:  More thorough/more efficient check?
}

std::string GetFormatBeforeMilliseconds(const std::string& format)
{
	const std::string milliId("%_");
	const auto milliPos(format.find(milliId));
	if (milliPos != std::string::npos)
	{
		assert(format.length() == milliPos + milliId.length());// Currently does not support milliseconds anywhere except at the end
		return format.substr(0, milliPos);
	}

	return format;
}

std::chrono::system_clock::time_point GetTimeStamp(const std::string& line, const std::string& timestampFormat)
{
	const std::string format(GetFormatBeforeMilliseconds(timestampFormat));

	std::istringstream ss(line);
	std::tm tm;
	ss >> std::get_time(&tm, format.c_str());
	auto timePoint(std::chrono::system_clock::from_time_t(std::mktime(&tm)));

	if (timestampFormat.length() != format.length())
	{
		int milliSecs;
		if ((ss >> milliSecs).fail())
			return timePoint;
		 timePoint += std::chrono::milliseconds(milliSecs);
	}

	return timePoint;
}

unsigned int GetNextChunk(std::istringstream& ss, std::string& chunk,
	std::chrono::system_clock::time_point& nextTime, const std::string& timestampFormat)
{
	assert(chunk.empty());

	unsigned int lineCount(0);
	std::string line;
	while (std::getline(ss, line))
	{
		chunk.append(line + '\n');
		++lineCount;
		if (Utilities::LineHasTimestamp(line, timestampFormat))
		{
			nextTime = Utilities::GetTimeStamp(line, timestampFormat);
			break;
		}
	}

	return lineCount;
}

std::string ExtractFileName(const std::string& s)
{
#ifdef _WIN32
	const auto lastSlash(s.find_last_of('\\'));
#else
	const auto lastSlash(s.find_last_of('/'));
#endif

	if (lastSlash == std::string::npos || s.length() == lastSlash)
		return s;

	return s.substr(lastSlash + 1);
}

}// namespace Utilities
