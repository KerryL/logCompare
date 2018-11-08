// File:  mainFrame.cpp
// Created:   11/5/2018
// Author:  K. Loux
// Description:  Main frame for the application.

// Local headers
#include "mainFrame.h"
#include "logFileWindow.h"
#include "logCompareApp.h"
#include "logFile.h"

//==========================================================================
// Class:			MainFrame
// Function:		MainFrame
//
// Description:		Constructor for MainFrame class.  Initializes the form
//					and creates the controls, etc.
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
MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, wxEmptyString,
	wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
	CreateControls();
	SetProperties();
}

//==========================================================================
// Class:			MainFrame
// Function:		CreateControls
//
// Description:		Creates sizers and controls and lays them out in the window.
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
void MainFrame::CreateControls()
{
	wxBoxSizer *topSizer(new wxBoxSizer(wxVERTICAL));
	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(mainSizer, wxSizerFlags().Expand().Proportion(1));

	AddLogWindow();
	AddLogWindow();

	SetMinSize(wxSize(600, 400));// TODO:  Has no effect?

	SetSizerAndFit(topSizer);
}

//==========================================================================
// Class:			MainFrame
// Function:		CreateVersionText
//
// Description:		Builds a static text control containing version
//					information.
//
// Input Arguments:
//		parent	= wxWindow*
//
// Output Arguments:
//		None
//
// Return Value:
//		wxWindow*
//
//==========================================================================
/*wxWindow* MainFrame::CreateVersionText(wxWindow *parent)
{
	wxString versionString(LogCompareApp::versionString + _T(" (") + LogCompareApp::gitHash + _T(")"));
	wxStaticText* versionInfo(new wxStaticText(parent, wxID_ANY, versionString));
	return versionInfo;
}*/

//==========================================================================
// Class:			MainFrame
// Function:		SetProperties
//
// Description:		Sets the window properties for this window.  Includes
//					title, frame size, and default fonts.
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
void MainFrame::SetProperties()
{
	SetTitle(LogCompareApp::appTitle);
	SetName(LogCompareApp::appName);
	Center();
}

void MainFrame::UpdateComparison()
{
	LogFileComparer comparer;
	for (const auto& w : logWindows)
		comparer.AddLog(w->GetOriginalContents());

	comparer.DoComparison();

	for (unsigned int i = 0; i < logWindows.size(); ++i)
		logWindows[i]->SetText(comparer.GetText(i));
}

void MainFrame::AddLogWindow()
{
	LogFileWindow::ButtonConfig buttonConfig;
	if (logWindows.size() == 0)
		buttonConfig = LogFileWindow::ButtonConfig::FirstWindow;
	else if (logWindows.size() == 1)
		buttonConfig = LogFileWindow::ButtonConfig::SecondWindow;
	else
		buttonConfig = LogFileWindow::ButtonConfig::OtherWindows;

	logWindows.push_back(new LogFileWindow(*this, buttonConfig));
	mainSizer->Add(logWindows.back(), wxSizerFlags().Expand().Proportion(1));

	PostSizeEvent();
	// TODO:  Need to update min size?  Why isn't this automatic?
}

void MainFrame::RemoveLogWindow(LogFileWindow* window)
{
	assert(logWindows.size() > 2);
	assert(logWindows.front() != window);
	assert(logWindows[1] != window);

	if (window->Destroy())
	{
		auto it(logWindows.begin() + 2);
		for (; it != logWindows.end(); ++it)
		{
			if (*it == window)
			{
				logWindows.erase(it);
				PostSizeEvent();
				UpdateComparison();
				return;
			}
		}
		assert(false);// should never reach here
	}
}

void MainFrame::SetScrollPosition(const unsigned int& position)
{
	for (auto& w : logWindows)
		w->SetScrollPosition(position);
}
