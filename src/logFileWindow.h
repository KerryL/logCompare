// File:  logFileWindow.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Core functionality for log display.

#ifndef LOG_FILE_WINDOW_H_
#define LOG_FILE_WINDOW_H_

// wxWidgets headers
#include <wx/wx.h>

// Standard C++ headers
#include <memory>

class MainFrame;
class LogFile;

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
	void SetText(const wxString& s);
	std::string GetOriginalContents() const;
	void SetScrollPosition(const unsigned int& position);

private:
	MainFrame& parent;
	std::unique_ptr<LogFile> logFile;

	void CreateControls(const ButtonConfig& buttonConfig);
	wxSizer* CreateButtonSizer(const ButtonConfig& buttonConfig);

	wxScrolledWindow* scrolledWin;
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

	void OnScrollChange(wxScrollWinEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif// LOG_FILE_WINDOW_H_
