#include "stdafx.h"

#include <vector>

#include "winTimedMsgBox.h"


//*****************************************************************************
namespace NS_WIN_TIMED_MSG_BOX
{

class CTimedMsgBoxData;

//*****************************************************************************
//
//=============================================================================
class CTMBDataLock
{
public:
	static CTMBDataLock& GetTMBDataLock();

private:
	CTMBDataLock();
public:
	~CTMBDataLock();

public:
	void AddData(UINT_PTR id, CTimedMsgBoxData *pData);
	void RemoveData(UINT_PTR id);
	CTimedMsgBoxData* GetData(UINT_PTR id);

private:
	CRITICAL_SECTION m_cs;
	std::vector<UINT_PTR> m_vecId;
	std::vector<CTimedMsgBoxData*> m_vecData;
};
#define theTMBDataLock  ( CTMBDataLock::GetTMBDataLock() )


//=============================================================================
CTMBDataLock& CTMBDataLock::GetTMBDataLock()
{
	static CTMBDataLock tmbDataLock;
	return tmbDataLock;
}


//=============================================================================
CTMBDataLock::CTMBDataLock()
{
	::InitializeCriticalSection(&m_cs);
}
//=============================================================================
CTMBDataLock::~CTMBDataLock()
{
	::DeleteCriticalSection(&m_cs);
}


//=============================================================================
void CTMBDataLock::AddData(UINT_PTR id, CTimedMsgBoxData *pData)
{
	::EnterCriticalSection(&m_cs);
		m_vecId.push_back(id);
		m_vecData.push_back(pData);
	::LeaveCriticalSection(&m_cs);
}
//=============================================================================
void CTMBDataLock::RemoveData(UINT_PTR id)
{
	::EnterCriticalSection(&m_cs);
		for(int k = 0; k < (int)m_vecId.size(); k++)
		{
			if(id == m_vecId[k])
			{
				m_vecId.erase(m_vecId.begin()+k);
				m_vecData.erase(m_vecData.begin()+k);
				break;
			}
		}
	::LeaveCriticalSection(&m_cs);
}
//=============================================================================
CTimedMsgBoxData* CTMBDataLock::GetData(UINT_PTR id)
{
	CTimedMsgBoxData *pData = nullptr;
	::EnterCriticalSection(&m_cs);
		for(int k = 0; k < (int)m_vecId.size(); k++)
		{
			if(id == m_vecId[k])
			{
				pData = m_vecData[k];
				break;
			}
		}
	::LeaveCriticalSection(&m_cs);
	return pData;
}


//*****************************************************************************
// CTimedMsgBoxData
//
//=============================================================================
class CTimedMsgBoxData
{
friend class CWinTimedMsgBox;

private:
	static void CALLBACK MessageBoxTimer(HWND hwnd, UINT uiMsg, UINT_PTR idTimer, DWORD dwTime);

private:
	CTimedMsgBoxData() : m_hwndTimedOwner(0), m_bTimedOut(false) {}
public:
	~CTimedMsgBoxData() {}

private:
	HWND m_hwndTimedOwner;
	bool m_bTimedOut;
};


//=============================================================================
void CALLBACK CTimedMsgBoxData::MessageBoxTimer(HWND /*hwnd*/, UINT /*uiMsg*/, UINT_PTR idTimer, DWORD /*dwTime*/)
{
	CTimedMsgBoxData *pData = theTMBDataLock.GetData(idTimer);
	if(nullptr == pData)
	{
		return;
	}
	pData->m_bTimedOut = true;
	if(0 != pData->m_hwndTimedOwner)
	{
		::EnableWindow(pData->m_hwndTimedOwner, TRUE);
	}
	::PostQuitMessage(0);
}
} // namespace NS_WIN_TIMED_MSG_BOX
//*****************************************************************************


using namespace NS_WIN_TIMED_MSG_BOX;

//*****************************************************************************
// CWinTimedMsgBox
//
//=============================================================================
int CWinTimedMsgBox::Run(
	HWND hwndOwner,
	LPCTSTR pszMessage,
	LPCTSTR pszTitle,
	UINT flags,
	DWORD dwTimeout/* = -1*/)
{
	int iResult = 0;

	CTimedMsgBoxData tmbData;
	UINT_PTR idTimer = 0;

	if(hwndOwner && ::IsWindowEnabled(hwndOwner))
	{
		tmbData.m_hwndTimedOwner = hwndOwner;
	}

	// Set a timer to dismiss the message box.
	if(-1 != dwTimeout)
	{
		idTimer = ::SetTimer(NULL, 0, dwTimeout, (TIMERPROC)CTimedMsgBoxData::MessageBoxTimer);
theTMBDataLock.AddData(idTimer, &tmbData);
	}

	iResult = ::MessageBox(hwndOwner, pszMessage, pszTitle, flags);

	// Finished with the timer.
	if(-1 != dwTimeout)
	{
		::KillTimer(NULL, idTimer);
theTMBDataLock.RemoveData(idTimer);
		idTimer = 0;
	}

	// See if there is a WM_QUIT message in the queue if we timed out.
	// Eat the message so we do not quit the whole application.
	if(tmbData.m_bTimedOut)
	{
		MSG msg;
		::PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
		iResult = -1;
	}

	return iResult;
}
