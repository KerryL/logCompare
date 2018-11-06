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

bool LineHasTimestamp(const std::string& line);
std::chrono::system_clock::time_point GetTimeStamp(const std::string& line);

}// namespace Utilities

#endif// UTILITIES_H_
