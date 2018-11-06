// File:  utilities.cpp
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Utility methods.

// Local headers
#include "utilities.h"

// Standard C++ headers
#include <sstream>
#include <iomanip>

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

}// namespace Utilities
