// File:  dropTarget.cpp
// Date:  11/5/2018
// Auth:  K. Loux
// Desc:  Derives from wxFileDropTarget and overrides OnDropFiles to load files
//        when the user drags-and-drops them onto the main window.

// Local headers
#include "dropTarget.h"
#include "logFileWindow.h"

//=============================================================================
// Class:			DropTarget
// Function:		DropTarget
//
// Description:		Constructor for DropTarget class.
//
// Input Arguments:
//		logFileWindow	= &LogFileWindow, reference to main application window
//
// Output Arguments:
//		None
//
// Return Value:
//		None
//
//=============================================================================
DropTarget::DropTarget(LogFileWindow &logFileWindow) : mLogFileWindow(logFileWindow)
{
	wxDataObjectComposite *dataObject(new wxDataObjectComposite);
	dataObject->Add(new wxFileDataObject);
	SetDataObject(dataObject);
}

//=============================================================================
// Class:			DropTarget
// Function:		OnDropFiles
//
// Description:		Handles dragging and dropping of files.
//
// Input Arguments:
//		fileNames	= const &wxArrayString containing the list of filenames
//					  being dropped
//
// Output Arguments:
//		None
//
// Return Value:
//		true to accept the data, false to veto
//
//=============================================================================
bool DropTarget::OnDropFiles(const wxArrayString &fileNames)
{
	if (fileNames.Count() != 1)
		return false;

	mLogFileWindow.Load(fileNames.front());
	return true;
}

//=============================================================================
// Class:			DropTarget
// Function:		OnData
//
// Description:		Overloaded virtual method from wxTextDropTarget.
//
// Input Arguments:
//		x		= wxCoord (unused)
//		y		= wxCoord (unused)
//		def		= wxDragResult
//
// Output Arguments:
//		None
//
// Return Value:
//		wxDragResult
//
//=============================================================================
wxDragResult DropTarget::OnData(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
	wxDragResult def)
{
	if (!GetData())
		return wxDragNone;

	wxDataObjectComposite *dataObject(
		static_cast<wxDataObjectComposite*>(m_dataObject));
	const size_t bufferSize(
		dataObject->GetDataSize(dataObject->GetReceivedFormat()));

	std::vector<char> buffer(bufferSize);
	if (!dataObject->GetDataHere(
		dataObject->GetReceivedFormat(), buffer.data()))
		return wxDragNone;

	if (dataObject->GetReceivedFormat().GetType() == wxDF_FILENAME)
	{
		wxFileDataObject fileData;
		fileData.SetData(bufferSize, buffer.data());
		return OnDropFiles(fileData.GetFilenames()) ? def : wxDragNone;
	}

	assert(false);
	return wxDragNone;
}
