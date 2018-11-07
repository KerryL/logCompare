// File:  logFileWindow.cpp
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Core functionality for log display.

// Local headers
#include "logFileWindow.h"
#include "mainFrame.h"
#include "dropTarget.h"
#include "logFile.h"

LogFileWindow::LogFileWindow(MainFrame& parent, const ButtonConfig& buttonConfig) : wxPanel(&parent), parent(parent)
{
	CreateControls(buttonConfig);
}

bool LogFileWindow::Load(const wxString& fileName)
{
	try
	{
		logFile = std::make_unique<LogFile>(fileName.ToStdString());
	}
	catch (const std::exception&)
	{
		return false;
	}

	parent.UpdateComparison();
	return true;
}

void LogFileWindow::CreateControls(const ButtonConfig& buttonConfig)
{
	wxSizer* mainSizer(new wxBoxSizer(wxVERTICAL));
	mainSizer->Add(CreateButtonSizer(buttonConfig), wxSizerFlags().Expand());

	scrolledWin = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALWAYS_SHOW_SB | wxVSCROLL);
	mainTextCtrl = new wxTextCtrl(scrolledWin, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2 | wxTE_NO_VSCROLL | wxTE_DONTWRAP);
	wxSizer* s(new wxBoxSizer(wxVERTICAL));
	s->Add(mainTextCtrl, wxSizerFlags().Expand().Proportion(1));
	scrolledWin->SetSizer(s);

	mainTextCtrl->SetDropTarget(new DropTarget(*this));
	mainSizer->Add(scrolledWin, wxSizerFlags().Expand().Proportion(1));

	SetSizerAndFit(mainSizer);
}

wxSizer* LogFileWindow::CreateButtonSizer(const ButtonConfig& buttonConfig)
{
	wxSizer* sizer(new wxBoxSizer(wxHORIZONTAL));
	sizer->Add(new wxButton(this, IdOpen, _T("Open")));

	if (buttonConfig == ButtonConfig::SecondWindow)
		return sizer;

	if (buttonConfig == ButtonConfig::FirstWindow)
		sizer->Add(new wxButton(this, IdAdd, _T("Add")));
	else if (buttonConfig == ButtonConfig::OtherWindows)
	{
		sizer->AddStretchSpacer();
		sizer->Add(new wxButton(this, IdRemove, _T("Remove")));
	}

	return sizer;
}

BEGIN_EVENT_TABLE(LogFileWindow, wxPanel)
	EVT_BUTTON(IdOpen,				OnOpenClick)
	EVT_BUTTON(IdAdd,				OnAddClick)
	EVT_BUTTON(IdRemove,			OnRemoveClick)
	EVT_SCROLLWIN(					OnScrollChange)
END_EVENT_TABLE()

void LogFileWindow::OnOpenClick(wxCommandEvent& WXUNUSED(event))
{
	const wxString wildcard("All files (*)|*");
	wxFileDialog dialog(&parent, _T("Open"), wxEmptyString, wxEmptyString,
		wildcard, wxFD_OPEN /*| wxFD_MULTIPLE*/ | wxFD_FILE_MUST_EXIST);

	dialog.CenterOnParent();
	wxArrayString pathsAndFileNames;
	if (dialog.ShowModal() == wxID_OK)
		dialog.GetPaths(pathsAndFileNames);

	for (const auto& fn : pathsAndFileNames)
	{
		if (!Load(fn))
		{
			// TODO:  Do what?
		}
	}
}

void LogFileWindow::OnAddClick(wxCommandEvent& WXUNUSED(event))
{
	parent.AddLogWindow();
}

void LogFileWindow::OnRemoveClick(wxCommandEvent& WXUNUSED(event))
{
	parent.RemoveLogWindow(this);
}

void LogFileWindow::SetText(const wxString& s)
{
	mainTextCtrl->SetValue(s);
}

std::string LogFileWindow::GetOriginalContents() const
{
	if (!logFile)
		return std::string();

	return logFile->GetContents();
}

void LogFileWindow::SetScrollPosition(const unsigned int& position)
{
	scrolledWin->SetScrollPos(wxVERTICAL, position);
}

void LogFileWindow::OnScrollChange(wxScrollWinEvent& event)
{
	if (event.GetOrientation() == wxHORIZONTAL)
		return;

	parent.SetScrollPosition(event.GetPosition());
}
