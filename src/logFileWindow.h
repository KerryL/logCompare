// File:  logFileWindow.h
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Core functionality for log display.

#ifndef LOG_FILE_WINDOW_H_
#define LOG_FILE_WINDOW_H_

// wxWidgets headers
#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/fdrepdlg.h>

// Standard C++ headers
#include <memory>

class MainFrame;
class LogFile;
class FindDialog;

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
	std::string GetTimestampFormat() const;
	double GetOffset() const;

	void DoFind(const wxString& s, const int& flags);
	void ClearFind();

private:
	MainFrame& parent;
	std::unique_ptr<LogFile> logFile;
	std::unique_ptr<FindDialog> dialog;

	void CreateControls(const ButtonConfig& buttonConfig);
	wxSizer* CreateButtonSizer(const ButtonConfig& buttonConfig);

	wxStyledTextCtrl* mainTextCtrl;
	wxStaticText* fileNameText;
	wxTextCtrl* timestampTextCtrl;
	wxTextCtrl* offsetTextCtrl;

	enum EventIDs
	{
		IdOpen,
		IdAdd,
		IdRemove,
		IdFind
	};

	void OnOpenClick(wxCommandEvent& event);
	void OnAddClick(wxCommandEvent& event);
	void OnRemoveClick(wxCommandEvent& event);
	void OnFindShortcut(wxCommandEvent& event);
	void OnTextInputChange(wxCommandEvent& event);

	void OnScrollChange(wxStyledTextEvent& event);

	DECLARE_EVENT_TABLE()
};

class FindDialog : public wxFindReplaceDialog
{
public:
	FindDialog(LogFileWindow& parent);

private:
	LogFileWindow& parent;

	wxFindReplaceData data;

	void OnFindNextEvent(wxFindDialogEvent& event);
	void OnCloseEvent(wxFindDialogEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif// LOG_FILE_WINDOW_H_
