// File:  logCompareApp.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  The application class.
// History:

#ifndef LOG_COMPARE_APP_H_
#define LOG_COMPARE_APP_H_

// wxWidgets headers
#include <wx/wx.h>

// Local forward declarations
class MainFrame;

// The application class
class LogCompareApp : public wxApp
{
public:
	// Initialization function
	bool OnInit();

	// The name of the application
	static const wxString appTitle;// As displayed
	static const wxString appName;// Internal
	static const wxString creator;
	/*static const wxString versionString;
	static const wxString gitHash;*/

private:
	// The main class for the application - this object is the parent for all other objects
	MainFrame *mainFrame = nullptr;
};

// Declare the application object (have wxWidgets create the wxGetApp() function)
DECLARE_APP(LogCompareApp);

#endif// LOG_COMPARE_APP_H_