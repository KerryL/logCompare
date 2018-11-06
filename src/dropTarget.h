// File:  dropTarget.h
// Date:  11/5/2018
// Auth:  K. Loux
// Desc:  Derives from wxDropTarget and overrides OnDropFiles to load files
//        when the user drags-and-drops them onto the main window.

#ifndef DROP_TARGET_H_
#define DROP_TARGET_H_

// wxWidgets headers
#include <wx/dnd.h>

class LogFileWindow;

/// Class derived from wxDropTarget in order to allow drag-and-drop operations.
class DropTarget : public wxDropTarget
{
public:
	explicit DropTarget(LogFileWindow &logFileWindow);
	~DropTarget() = default;

	wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def) override;

private:
	LogFileWindow &mLogFileWindow;
	bool OnDropFiles(const wxArrayString &fileNames);
};

#endif// DROP_TARGET_H_
