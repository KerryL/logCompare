// File:  logFile.cpp
// Created:  11/6/2018
// Author:  K. Loux
// Description:  Class representing log file.

// Local headers
#include "logFile.h"

// Standard C++ headers
#include <fstream>
#include <exception>
#include <iomanip>

LogFile::LogFile(const std::string& fileName)
{
	if (!ReadFile(fileName, originalFileContents))
		throw std::exception("Failed to open file");
}

bool LogFile::ReadFile(const std::string& fileName, std::string& contents)
{
	std::ifstream file(fileName.c_str());
	if (!file.good() || !file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line))
		contents += line + '\n';

	return true;
}

void LogFileComparer::AddLog(const std::string& contents, const std::string& timestampFormat, const double& offset)
{
	RefPoint r;
	r.ss.str(contents);
	r.timestampFormat = timestampFormat;
	r.offset = std::chrono::duration<double>(offset);

	r.linesAdded = Utilities::GetNextChunk(r.ss, r.nextChunk, r.nextTime, r.timestampFormat, r.offset);
	refs.push_back(std::move(r));

	if (!contents.empty() && r.nextTime < nextPrintTime)
		nextPrintTime = r.nextTime;
}

void LogFileComparer::AddChunk(RefPoint& r)
{
	r.text.append(r.linesAddedSincePrint, '\n');
	r.text.append(r.nextChunk);
	r.linesAddedSincePrint = 0;
	r.nextChunk.clear();
	r.linesAdded = Utilities::GetNextChunk(r.ss, r.nextChunk, r.nextTime, r.timestampFormat, r.offset);
}

void LogFileComparer::DoComparison()
{
	unsigned int maxLinesAdded(1);
	while (maxLinesAdded > 0)
	{
		maxLinesAdded = 0;
		std::vector<RefPoint*> addedTo;
		for (auto& r : refs)
		{
			if (r.nextTime <= nextPrintTime)
			{
				AddChunk(r);
				addedTo.push_back(&r);

				if (r.linesAdded > maxLinesAdded)
					maxLinesAdded = r.linesAdded;
			}
		}

		nextPrintTime = Utilities::TimePoint::max();
		for (auto& r : refs)
		{
			if (r.ss.str().empty())
				continue;

			AdjustLinesAdded(maxLinesAdded, r, addedTo);
			if (r.nextTime < nextPrintTime)
				nextPrintTime = r.nextTime;
		}
	}
}

void LogFileComparer::AdjustLinesAdded(const unsigned int& maxLinesAdded, RefPoint& r,
	const std::vector<RefPoint*>& addedTo)
{
	bool added(false);
	for (const auto& a : addedTo)
	{
		if (a == &r)
		{
			added = true;
			break;
		}
	}

	if (added)
		r.linesAddedSincePrint = maxLinesAdded - r.linesAdded;
	else
		r.linesAddedSincePrint += maxLinesAdded;
}
