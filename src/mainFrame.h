// File:  mainFrame.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Main frame for the application.

#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

// wxWidgets headers
#include <wx/wx.h>

// Standard C++ headers
#include <vector>

class LogFileWindow;

// The main frame class
class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame() = default;

	void AddLogWindow();
	void RemoveLogWindow(LogFileWindow* window);
	void UpdateComparison();
	void SetScrollPosition(const unsigned int& position);

private:
	// Functions that do some of the frame initialization and control positioning
	void CreateControls();
	void SetProperties();

	wxBoxSizer *mainSizer;

	std::vector<LogFileWindow*> logWindows;

	//wxWindow* CreateVersionText(wxWindow *parent);
};

#endif// MAIN_FRAME_H_
