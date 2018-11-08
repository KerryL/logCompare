// File:  utilities.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Utility methods.

#ifndef UTILITIES_H_
#define UTILITIES_H_

// Standard C++ headers
#include <string>
#include <chrono>

namespace Utilities
{

bool LineHasTimestamp(const std::string& line, const std::string& timestampFormat);
std::chrono::system_clock::time_point GetTimeStamp(const std::string& line, const std::string& timestampFormat);
unsigned int GetNextChunk(std::istringstream& ss, std::string& chunk,
	std::chrono::system_clock::time_point& nextTime, const std::string& timestampFormat);
std::string ExtractFileName(const std::string& s);
std::string GetFormatBeforeMilliseconds(const std::string& format);

}// namespace Utilities

#endif// UTILITIES_H_
