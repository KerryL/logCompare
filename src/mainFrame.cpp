// File:  mainFrame.cpp
// Created:   11/5/2018
// Author:  K. Loux
// Description:  Main frame for the application.

// Local headers
#include "mainFrame.h"
#include "logFileWindow.h"
#include "logCompareApp.h"
#include "utilities.h"

// Standard C++ headers
#include <sstream>

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
	struct RefPoint
	{
		LogFileWindow* logWindow;
		std::istringstream ss;
		std::chrono::system_clock::time_point nextTime;
		std::string text;
		std::string nextChunk;
		unsigned int linesAdded;
		unsigned int linesAddedSincePrint = 0;
	};

	std::chrono::system_clock::time_point nextPrintTime = std::chrono::system_clock::time_point::max();
	std::vector<RefPoint> refs;
	for (const auto& w : logWindows)
	{
		if (w->GetOriginalContents().empty())
			continue;

		RefPoint r;
		r.logWindow = w;
		r.ss.str(w->GetOriginalContents());

		r.linesAdded = Utilities::GetNextChunk(r.ss, r.nextChunk, r.nextTime);
		refs.push_back(std::move(r));

		if (r.nextTime < nextPrintTime)
			nextPrintTime = r.nextTime;
	}

	unsigned int maxLinesAdded(1);
	while (maxLinesAdded > 0)
	{
		maxLinesAdded = 0;
		std::vector<RefPoint*> addedTo;
		for (auto& r : refs)
		{
			if (r.nextTime <= nextPrintTime)
			{
				r.text.append(r.linesAddedSincePrint, '\n');
				r.text.append(r.nextChunk);
				r.linesAddedSincePrint = 0;
				r.nextChunk.clear();

				r.linesAdded = Utilities::GetNextChunk(r.ss, r.nextChunk, r.nextTime);
				addedTo.push_back(&r);
				if (r.linesAdded > maxLinesAdded)
					maxLinesAdded = r.linesAdded;
			}
		}

		nextPrintTime = std::chrono::system_clock::time_point::max();
		for (auto& r : refs)
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

			if (r.nextTime < nextPrintTime)
				nextPrintTime = r.nextTime;
		}
	}

	for (const auto& r : refs)
		r.logWindow->SetText(r.text);
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
