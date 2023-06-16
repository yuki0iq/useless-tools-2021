#pragma once

#include <Windows.h>
#include <tchar.h>


// Example:
//
//	CWinTimedMsgBox tmb;
//	int iRes = tmb.Run(0, _T("Message.."), _T("Timed MessageBox (5 secs)"), MB_ICONQUESTION | MB_YESNO, 5*1000);
//	if(-1 == iRes)
//	{
//		// timeout.
//	}
//	if(IDYES == iRes)
//	{
//		// <YES> btn.
//	}

//=============================================================================
class CWinTimedMsgBox
{
public:
	CWinTimedMsgBox() {}
	~CWinTimedMsgBox() {}

public:
	// Return -1 if timeout else retVal depends on flags.
	int Run(
		HWND hwndOwner,
		LPCTSTR pszMessage,
		LPCTSTR pszTitle,
		UINT flags,
		DWORD dwTimeout = -1); // in millisecs; -1 : no timeout
};
