// File:  logFileWindow.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Core functionality for log display.

#ifndef LOG_FILE_WINDOW_H_
#define LOG_FILE_WINDOW_H_

// wxWidgets headers
#include <wx/wx.h>

class MainFrame;

class LogFileWindow : public wxPanel
{
public:
	enum class ButtonConfig
	{
		FirstWindow,
		SecondWindow,
		OtherWindows
	};

	explicit LogFileWindow(MainFrame& parent, const ButtonConfig& buttonConfig);
	~LogFileWindow() = default;

	bool Load(const wxString& fileName);

private:
	MainFrame& parent;

	void CreateControls(const ButtonConfig& buttonConfig);
	wxSizer* CreateButtonSizer(const ButtonConfig& buttonConfig);

	wxTextCtrl* mainTextCtrl;

	enum EventIDs
	{
		IdOpen,
		IdAdd,
		IdRemove
	};

	void OnOpenClick(wxCommandEvent& event);
	void OnAddClick(wxCommandEvent& event);
	void OnRemoveClick(wxCommandEvent& event);
	void OnScrollChange(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif// LOG_FILE_WINDOW_H_
