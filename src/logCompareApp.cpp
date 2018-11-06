// File:  logCompareApp.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  The application class.
// History:

// wxWidgets headers
#include <wx/wx.h>

// Local headers
#include "logCompareApp.h"
#include "mainFrame.h"

// Implement the application (have wxWidgets set up the appropriate entry points, etc.)
IMPLEMENT_APP(LogCompareApp);

//==========================================================================
// Class:			LogCompareApp
// Function:		Constant Declarations
//
// Description:		Constant declarations for the DataPlotterApp class.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//==========================================================================
const wxString LogCompareApp::appTitle = _T("LogCompare");
const wxString LogCompareApp::appName = _T("LogCompareApplication");
const wxString LogCompareApp::creator = _T("Kerry Loux");
// gitHash and versionString are defined in gitHash.cpp, which is automatically generated during the build

//==========================================================================
// Class:			LogCompareApp
// Function:		OnInit
//
// Description:		Initializes the application window.
//
// Input Arguments:
//		None
//
// Output Arguments:
//		None
//
// Return Value:
//		bool = true for successful window initialization, false for error
//
//==========================================================================
bool LogCompareApp::OnInit()
{
	// Set the application's name and the vendor's name
	SetAppName(appName);
	SetVendorName(creator);

	// Create the MainFrame object - this is the parent for all other objects
	mainFrame = new MainFrame();

	// Make sure the MainFrame was successfully created
	if (!mainFrame)
		return false;

	// Make the window visible
	mainFrame->Show(true);

	return true;
}
