// File:  logFile.h
// Created:  11/6/2018
// Author:  K. Loux
// Description:  Class representing log file.

#ifndef LOG_FILE_H_
#define LOG_FILE_H_

// Standard C++ headers
#include <chrono>
#include <string>

class LogFile
{
public:
	explicit LogFile(const std::string& fileName);

	const std::string& GetContents() const { return originalFileContents; }

private:
	std::string originalFileContents;

	static bool ReadFile(const std::string& fileName, std::string& contents);
};

#endif// LOG_FILE_H_
