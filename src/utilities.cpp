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

bool LineHasTimestamp(const std::string& line)
{
	if (line.size() < 24)
		return false;
	if (line[2] != '/' ||
		line[5] != '/' ||
		line[10] != ',' ||
		line[13] != ':' ||
		line[16] != ':' ||
		line[19] != ':')
		return false;
	return true;// TODO:  More thorough/more efficient check?  Handle variety of timestamps?
}

std::chrono::system_clock::time_point GetTimeStamp(const std::string& line)
{
	std::istringstream ss(line);
	std::tm tm;
	ss >> std::get_time(&tm, "%m/%d/%Y,%H:%M:%S");
	auto timePoint(std::chrono::system_clock::from_time_t(std::mktime(&tm)));

	ss.ignore();// ignore the next ':'
	int milliSecs;
	if ((ss >> milliSecs).fail())
		return timePoint;

	return timePoint + std::chrono::milliseconds(milliSecs);
}

unsigned int GetNextChunk(std::istringstream& ss, std::string& chunk, std::chrono::system_clock::time_point& nextTime)
{
	assert(chunk.empty());

	unsigned int lineCount(0);
	std::string line;
	while (std::getline(ss, line))
	{
		chunk.append(line + '\n');
		++lineCount;
		if (Utilities::LineHasTimestamp(line))
		{
			nextTime = Utilities::GetTimeStamp(line);
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
