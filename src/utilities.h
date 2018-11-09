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

typedef std::chrono::duration<double> Duration;
typedef std::chrono::time_point<std::chrono::system_clock, Duration> TimePoint;

bool LineHasTimestamp(const std::string& line, const std::string& timestampFormat);
TimePoint GetTimeStamp(const std::string& line, const std::string& timestampFormat);
unsigned int GetNextChunk(std::istringstream& ss, std::string& chunk,
	TimePoint& nextTime, const std::string& timestampFormat, const Duration& offset);
std::string ExtractFileName(const std::string& s);
std::string GetFormatBeforeMilliseconds(const std::string& format);

}// namespace Utilities

#endif// UTILITIES_H_
