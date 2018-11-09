// File:  logFile.h
// Created:  11/6/2018
// Author:  K. Loux
// Description:  Class representing log file.

#ifndef LOG_FILE_H_
#define LOG_FILE_H_

// Local headers
#include "utilities.h"

// Standard C++ headers
#include <chrono>
#include <string>
#include <sstream>
#include <vector>

class LogFile
{
public:
	explicit LogFile(const std::string& fileName);

	const std::string& GetContents() const { return originalFileContents; }

private:
	std::string originalFileContents;

	static bool ReadFile(const std::string& fileName, std::string& contents);
};

class LogFileComparer
{
public:
	void AddLog(const std::string& contents, const std::string& timestampFormat, const double& offset);
	void LogFileComparer::DoComparison();
	std::string GetText(const unsigned int& i) const { return refs[i].text; }

private:
	struct RefPoint
	{
		std::istringstream ss;
		Utilities::TimePoint nextTime;
		Utilities::Duration offset;
		std::string text;
		std::string nextChunk;
		std::string timestampFormat;
		unsigned int linesAdded;
		unsigned int linesAddedSincePrint = 0;
	};

	Utilities::TimePoint nextPrintTime = Utilities::TimePoint::max();
	std::vector<RefPoint> refs;

	void AddChunk(RefPoint& r);
	void AdjustLinesAdded(const unsigned int& maxLinesAdded, RefPoint& r,
		const std::vector<RefPoint*>& addedTo);
};

#endif// LOG_FILE_H_
