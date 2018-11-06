// File:  logFile.h
// Created:  11/6/2018
// Author:  K. Loux
// Description:  Class representing log file.

#ifndef LOG_FILE_H_
#define LOG_FILE_H_

// wxWidgets headers
#include <wx/wx.h>

// Standard C++ headers
#include <chrono>

class LogFile
{
public:
	explicit LogFile(const wxString& fileName);

	wxString GetContentsUpTo(const std::chrono::system_clock::time_point& upToTime);

private:
	const wxString originalFileContents;
};

#endif// LOG_FILE_H_
