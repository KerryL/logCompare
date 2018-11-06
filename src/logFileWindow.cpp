// File:  logFileWindow.cpp
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Core functionality for log display.

// Local headers
#include "logFileWindow.h"
#include "mainFrame.h"

LogFileWindow::LogFileWindow(MainFrame& parent, const ButtonConfig& buttonConfig) : wxPanel(&parent), parent(parent)
{
	CreateControls(buttonConfig);
}

bool LogFileWindow::Load(const wxString& fileName)
{
	// TODO
	return false;
}

void LogFileWindow::CreateControls(const ButtonConfig& buttonConfig)
{
	wxSizer* mainSizer(new wxBoxSizer(wxVERTICAL));
	mainSizer->Add(CreateButtonSizer(buttonConfig), wxSizerFlags().Expand());

	mainTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2 | wxHSCROLL);
	mainSizer->Add(mainTextCtrl, wxSizerFlags().Expand().Proportion(1));

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
	EVT_BUTTON(IdOpen,		OnOpenClick)
	EVT_BUTTON(IdAdd,		OnAddClick)
	EVT_BUTTON(IdRemove,	OnRemoveClick)
END_EVENT_TABLE()

void LogFileWindow::OnOpenClick(wxCommandEvent& WXUNUSED(event))
{
	wxString wildcard("All files (*)|*");

	wxArrayString fileList = LibPlot2D::GuiUtilities::GetFileNameFromUser(this,
		_T("Open Data File"), wxEmptyString, wxEmptyString, wildcard,
		wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

	wxFileDialog dialog(&parent, _T("Open"), wxEmptyString, wxEmptyString,
		wildcard, style);

	dialog.CenterOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		// If this was an open dialog, we want to get all of the selected paths,
		// otherwise, just get the one selected path
		if (style & wxFD_OPEN)
			dialog.GetPaths(pathsAndFileNames);
		else
			pathsAndFileNames.Add(dialog.GetPath());
	}

	return pathsAndFileNames;

	if (fileList.GetCount() == 0)
		return;

	mPlotInterface.LoadFiles(fileList);
}

void LogFileWindow::OnAddClick(wxCommandEvent& WXUNUSED(event))
{
	parent.AddLogWindow();
}

void LogFileWindow::OnRemoveClick(wxCommandEvent& WXUNUSED(event))
{
	parent.RemoveLogWindow(this);
}

void LogFileWindow::OnScrollChange(wxCommandEvent& event)
{
}
