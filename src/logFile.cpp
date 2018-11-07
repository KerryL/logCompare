// File:  logFile.cpp
// Created:  11/6/2018
// Author:  K. Loux
// Description:  Class representing log file.

// Local headers
#include "logFile.h"

// Standard C++ headers
#include <fstream>
#include <exception>

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
