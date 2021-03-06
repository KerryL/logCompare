// File:  logFileWindow.cpp
// Created:  11/5/2018
// Author:  K. Loux
// Description:  Core functionality for log display.

// Local headers
#include "logFileWindow.h"
#include "mainFrame.h"
#include "dropTarget.h"
#include "logFile.h"
#include "utilities.h"

// Standard C++ headers
#include <algorithm>

LogFileWindow::LogFileWindow(MainFrame& parent, const ButtonConfig& buttonConfig) : wxPanel(&parent), parent(parent)
{
	CreateControls(buttonConfig);

	const int entryCount(1);
	wxAcceleratorEntry entries[entryCount];
	entries[0].Set(wxACCEL_CTRL, static_cast<int>('f'), IdFind);
	wxAcceleratorTable accel(entryCount, entries);
	SetAcceleratorTable(accel);
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

	fileNameText->SetLabel(wxString(_T("File name:  ")) + wxString(Utilities::ExtractFileName(fileName.ToStdString())));
	parent.UpdateComparison();
	return true;
}

void LogFileWindow::CreateControls(const ButtonConfig& buttonConfig)
{
	wxSizer* mainSizer(new wxBoxSizer(wxVERTICAL));
	mainSizer->Add(CreateButtonSizer(buttonConfig), wxSizerFlags().Expand());

	wxFlexGridSizer* inputSizer(new wxFlexGridSizer(2, 5, 5));
	inputSizer->AddGrowableCol(1);

	inputSizer->Add(new wxStaticText(this, wxID_ANY, _T("TS Format:")));
	timestampTextCtrl = new wxTextCtrl(this, wxID_ANY, _T("%m/%d/%Y,%H:%M:%S:%_"));
	timestampTextCtrl->SetToolTip(_T("Timestamp formats can be described using standard C language identifiers.  Additionally, %_ is supported to indicate a 3-digit millisecond field."));

	inputSizer->Add(timestampTextCtrl, wxSizerFlags().Expand().Proportion(1));

	inputSizer->Add(new wxStaticText(this, wxID_ANY, _T("Offset:")));
	wxSizer* offsetSizer(new wxBoxSizer(wxHORIZONTAL));
	offsetTextCtrl = new wxTextCtrl(this, wxID_ANY, _T("0"));
	offsetTextCtrl->SetToolTip(_T("Offset is added to timestamp (positive times move timestamps forward in time)."));
	offsetSizer->Add(offsetTextCtrl, wxSizerFlags().Expand().Proportion(1));
	offsetSizer->Add(new wxStaticText(this, wxID_ANY, _T("sec")), wxSizerFlags().Border(wxLEFT, 5));

	inputSizer->Add(offsetSizer, wxSizerFlags().Expand().Proportion(1));
	mainSizer->Add(inputSizer, wxSizerFlags().Expand().Border(wxALL, 5));

	mainTextCtrl = new wxStyledTextCtrl(this);
	mainTextCtrl->SetDropTarget(new DropTarget(*this));
	mainTextCtrl->SetWrapMode(wxSTC_WRAP_NONE);
	mainTextCtrl->SetMinSize(wxSize(400,400));

	/*if (buttonConfig == ButtonConfig::FirstWindow)
	{
		mainTextCtrl->SetMarginWidth(0, 30);
		mainTextCtrl->SetMarginType(0, wxSTC_MARGIN_NUMBER);
		mainTextCtrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(75, 75, 75));
		mainTextCtrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(220, 220, 220));
	}*/

	mainSizer->Add(mainTextCtrl, wxSizerFlags().Expand().Proportion(1));

	fileNameText = new wxStaticText(this, wxID_ANY, wxEmptyString);
	mainSizer->Add(fileNameText, wxSizerFlags().Expand());

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
	EVT_BUTTON(IdOpen,			OnOpenClick)
	EVT_BUTTON(IdAdd,			OnAddClick)
	EVT_BUTTON(IdRemove,		OnRemoveClick)
	EVT_MENU(IdFind,			OnFindShortcut)
	EVT_STC_PAINTED(wxID_ANY,	OnScrollChange)
	EVT_TEXT(wxID_ANY,			OnTextInputChange)
END_EVENT_TABLE()

void LogFileWindow::OnOpenClick(wxCommandEvent& WXUNUSED(event))
{
	const wxString wildcard("All files (*)|*");
	wxFileDialog openDialog(&parent, _T("Open"), wxEmptyString, wxEmptyString,
		wildcard, wxFD_OPEN /*| wxFD_MULTIPLE*/ | wxFD_FILE_MUST_EXIST);

	openDialog.CenterOnParent();
	wxArrayString pathsAndFileNames;
	if (openDialog.ShowModal() == wxID_OK)
		openDialog.GetPaths(pathsAndFileNames);

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
	mainTextCtrl->ScrollToLine(position);
}

void LogFileWindow::OnFindShortcut(wxCommandEvent& WXUNUSED(event))
{
	dialog = std::make_unique<FindDialog>(*this);
	dialog->Show();
}

void LogFileWindow::OnScrollChange(wxStyledTextEvent& WXUNUSED(event))
{
	const auto line(mainTextCtrl->GetFirstVisibleLine());
	parent.SetScrollPosition(line);
}

void LogFileWindow::OnTextInputChange(wxCommandEvent& WXUNUSED(event))
{
	parent.UpdateComparison();
}

void LogFileWindow::DoFind(const wxString& s, const int& flags)
{
	const double styleId(0);
	mainTextCtrl->IndicatorSetStyle(styleId, wxSTC_INDIC_ROUNDBOX);
	mainTextCtrl->IndicatorSetAlpha(styleId, 100);
	mainTextCtrl->IndicatorSetUnder(styleId, true);
	mainTextCtrl->IndicatorSetForeground(styleId, wxColor(0, 255, 0));

	std::vector<wxString::size_type> selections;
	wxString::size_type currentPosition(0);
	while (currentPosition = mainTextCtrl->FindText(currentPosition,
		mainTextCtrl->GetLastPosition(), s, flags), currentPosition != wxString::npos)
	{
		selections.push_back(currentPosition);
		currentPosition += s.length();
	}

	if (!(flags & wxFR_DOWN))
		std::reverse(selections.begin(), selections.end());

	mainTextCtrl->SetIndicatorCurrent(styleId);
	bool moved(false);
	wxString::size_type cursorPosition(mainTextCtrl->GetCurrentPos());
	for (const auto& p : selections)
	{
		mainTextCtrl->IndicatorFillRange(p, s.length());
		if (!moved)
		{
			if (((flags & wxFR_DOWN) && p > cursorPosition) ||
				(!(flags & wxFR_DOWN) && p < cursorPosition))
			{
				cursorPosition = p;
				moved = true;
			}
		}
	}

	if (!moved && selections.size() > 0)
		cursorPosition = selections.front();

	mainTextCtrl->SetCurrentPos(cursorPosition);
	mainTextCtrl->GotoPos(cursorPosition);
}

void LogFileWindow::ClearFind()
{
	mainTextCtrl->IndicatorClearRange(0, mainTextCtrl->GetLastPosition());
}

std::string LogFileWindow::GetTimestampFormat() const
{
	return timestampTextCtrl->GetValue().ToStdString();
}

double LogFileWindow::GetOffset() const
{
	double offset;
	if (!offsetTextCtrl->GetValue().ToDouble(&offset))
		return 0.0;
	return offset;
}

FindDialog::FindDialog(LogFileWindow& parent) : wxFindReplaceDialog(&parent, &data, _T("Find")), parent(parent)
{
	data.SetFlags(wxFR_DOWN);
}

BEGIN_EVENT_TABLE(FindDialog, wxFindReplaceDialog)
	EVT_FIND_NEXT(wxID_ANY,		OnFindNextEvent)// TODO:  Event doesn't fire the first time button is clicked?
	EVT_FIND_CLOSE(wxID_ANY,	OnCloseEvent)
END_EVENT_TABLE()

void FindDialog::OnFindNextEvent(wxFindDialogEvent& event)
{
	parent.ClearFind();
	parent.DoFind(event.GetFindString(), event.GetFlags());
}

void FindDialog::OnCloseEvent(wxFindDialogEvent& WXUNUSED(event))
{
	parent.ClearFind();
}
