// UTGUI.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "UTGUI.h"
#include "TreeCalc.h"
#include "gdc_win.h"
#include "winTimedMsgBox.h"
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <utility>
#include <cmath>
using namespace std;

#define MAX_LOADSTRING 100
typedef double(*F_x_y_0)(double, double);
typedef pair<pair<double, double>, bool>(*F_x_y_t)(double);

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
TreeCalc tc;
GDC *GMemDC = 0, *CalcDC = 0;
GFont *pFont = 0;
int iWidth = 640, iHeight = 400;
const int PLOT_UNIT = 20;
const int PLOT_ZASECKA = 5;
int iMaxWidth, iMaxHeight;
double m_Xs = 1, m_Ys = 1, m_Xm = 0, m_Ym = 0,
	m_Tmin = -5, m_Tmax = 5, m_Tstep = .1;
HWND hMainWnd = 0;
TreeCalc tx, ty;
map<tstring, double> var_map, var_map_tc;
tstring ct_expr;
F_x_y_0 ct_func;
tstring ex = L"X(t) = ", ey = L"Y(t) = ";
F_x_y_t calc_func;

// wParam and lParam are 0
HANDLE hCTEvent = 0, hCTControl = 0;
// you send CONTROL and receive EVENT
// you send CONTROL if you want to set state to StartCalc
// or if you are not in plotting state to Kill
// otherwise you MUST set ONLY the ct_cmd flag
// because the thread SHOULD check that flag
// and use csCTCmd critical section for accessing that flag
HANDLE hCalcThread = 0;
// thread <-
//   HWND
enum __CalcThr_Command { None, StartCalc, StopCalc, Kill } ct_cmd = None;
enum __MainWnd_State { Idle, Plotting } state = Idle;
CRITICAL_SECTION csMemDC;
CRITICAL_SECTION csCTCmd;
CRITICAL_SECTION csMWState;
CRITICAL_SECTION csPlotted;
double dPlotted = 100;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
void InitPrecalc(HWND hWnd);
void ClosePrecalc();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
bool OnPaint(HWND hWnd);
bool OnSize(HWND hWnd, LPARAM lParam);
bool OnClose(HWND hWnd);
bool OnFileExit(HWND hWnd);
bool OnAlLinear(HWND hWnd);
bool OnAlSquare(HWND hWnd);
bool OnAlCubic(HWND hWnd);
bool OnAlTreecalc(HWND hWnd);
bool OnAlStopPlotting(HWND hWnd);
bool OnStArrayprop(HWND hWnd);
bool OnHlpAbout(HWND hWnd);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AlTreecalc(HWND, UINT, WPARAM, LPARAM);
INT_PTR OnAlTreecalcCalc(HWND hDlg);
INT_PTR OnAlTreecalcPlot(HWND hDlg);
INT_PTR CALLBACK TcPlot(HWND, UINT, WPARAM, LPARAM);
INT_PTR OnTcPlot(HWND hDlg);
INT_PTR CALLBACK StatArrProp(HWND, UINT, WPARAM, LPARAM);
INT_PTR OnStatArrPropCalc(HWND hDlg);
int REALtoPX_X(double x);
int REALtoPX_Y(double y);
double PXtoREAL_X(int x);
double PXtoREAL_Y(int y);
void DrawGrid(GDC *pDC);
void DrawFunc_F_x_y_0(HWND hWnd, F_x_y_0 func); // Single-threaded
void DrawFunc_F_x_y_0__Treecalc(HWND hWnd); // Single-threaded
double round_g(double a);
double __f_Treecalc(double x, double y);
pair<pair<double, double>, bool> __f_TcPlot_T(double t);
DWORD WINAPI CalcThreadProc(void *arg);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_UTGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UTGUI));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ClosePrecalc();

	return (int) msg.wParam;
}


//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UTGUI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_UTGUI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 640, 400, NULL, NULL, hInstance, NULL);

   if (!hMainWnd)
   {
      return FALSE;
   }

   InitPrecalc(hMainWnd);
   DrawGrid(CalcDC);
   DrawGrid(GMemDC);

   ShowWindow(hMainWnd, nCmdShow);
   UpdateWindow(hMainWnd);

   return TRUE;
}

void InitPrecalc(HWND hWnd)
{
	iMaxWidth = 2 * ::GetSystemMetrics(SM_CXFULLSCREEN);
	iMaxHeight = 2 * ::GetSystemMetrics(SM_CYFULLSCREEN);
	GMemDC = theGMaster->GetMemGDC((int)hWnd, iMaxWidth, iMaxHeight);
	CalcDC = theGMaster->GetMemGDC((int)hWnd, iMaxWidth, iMaxHeight);
	::InitializeCriticalSection(&csMemDC);
	::InitializeCriticalSection(&csCTCmd);
	::InitializeCriticalSection(&csMWState);
	::InitializeCriticalSection(&csPlotted);

	pFont = theGMaster->GetGFont("Sarasa Fixed J", 16);
	//pFont = theGMaster->GetGFont("Unifont-JP", 16);

	// create events
	hCTEvent = ::CreateEvent(NULL, FALSE, FALSE, nullptr);
	hCTControl = ::CreateEvent(NULL, FALSE, FALSE, nullptr);
	// create thread
	hCalcThread = ::CreateThread(NULL, 0, CalcThreadProc, nullptr, 0, nullptr);
	// wait until thread ready
	::WaitForSingleObject(hCTEvent, -1);
}

void ClosePrecalc()
{
	// SEND QUIT MESSAGE
	::EnterCriticalSection(&csCTCmd);
	ct_cmd = Kill;
	::LeaveCriticalSection(&csCTCmd);
	::SetEvent(hCTControl);
	// wait until thread kills
	::WaitForSingleObject(hCalcThread, -1);
	// delete thread
	::CloseHandle(hCalcThread);
	// delete events
	::CloseHandle(hCTControl);
	::CloseHandle(hCTEvent);

	theGMaster->ReleaseGFont(pFont);
	
	::DeleteCriticalSection(&csPlotted);
	::DeleteCriticalSection(&csMWState);
	::DeleteCriticalSection(&csCTCmd);
	::DeleteCriticalSection(&csMemDC);
	theGMaster->ReleaseGDC(CalcDC);
	CalcDC = 0;
	theGMaster->ReleaseGDC(GMemDC);
	GMemDC = 0;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bHandled = false;

	switch (message)
	{
	case WM_COMMAND:
		bHandled = OnCommand(hWnd, wParam, lParam);
		break;
	/*case WM_ERASEBKGND:
		return 0;*/
	case WM_PAINT:
		bHandled = OnPaint(hWnd);
		break;
	case WM_SIZE:
		bHandled = OnSize(hWnd, lParam);
		break;
	case WM_DESTROY:
		bHandled = true;
		::PostQuitMessage(0);
		break;
	case WM_CLOSE:
		bHandled = OnClose(hWnd);
		break;
	default:
		break;
	}
	
	if(bHandled)
		return 0;
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	bool bHandled = false;
	WORD wmId    = LOWORD(wParam);
	WORD wmEvent = HIWORD(wParam);
	// Разобрать выбор в меню:
	switch (wmId)
	{
	case IDM_FI_EXIT:
		bHandled = OnFileExit(hWnd);
		break;
	case IDM_AL_LINEAR:
		bHandled = OnAlLinear(hWnd);
		break;
	case IDM_AL_SQUARE:
		bHandled = OnAlSquare(hWnd);
		break;
	case IDM_AL_CUBIC:
		bHandled = OnAlCubic(hWnd);
		break;
	case IDM_AL_TREECALC:
		bHandled = OnAlTreecalc(hWnd);
		break;
	case IDM_AL_SP:
		bHandled = OnAlStopPlotting(hWnd);
		break;
	case IDM_ST_ARRAYPROP:
		bHandled = OnStArrayprop(hWnd);
		break;
	case IDM_HLP_ABOUT:
		bHandled = OnHlpAbout(hWnd);
		break;
	case IDM_TC_PLOT:
		bHandled = OnAlTreecalcPlot(hWnd);
	default:
		break;
	}

	return bHandled;
}

bool OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	::BeginPaint(hWnd, &ps);
	GDC *pGDC = theGMaster->GetGDC((int)hWnd);
	pGDC->SetFont(pFont);

	GRect gPaintRect(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
	GRgn *gPaintRgn = theGMaster->GetGRgn(&gPaintRect, 1);
	pGDC->SetClipRegion(gPaintRgn);

	::EnterCriticalSection(&csMemDC);
	pGDC->CopyArea(0, 0, GMemDC, iMaxWidth/2 - iWidth/2, iMaxHeight/2 - iHeight/2, iWidth, iHeight);
	::LeaveCriticalSection(&csMemDC);

	// GRAPH INFO
	GPen *pen = theGMaster->GetGPen(2, *GColor::GetColorBlack());
	pGDC->SetBackgroundMode(EBkMode::ETransparent);
	pGDC->SetPen(pen);

	pGDC->OutText(_T("Ｙ"), iWidth/2 + 4, 4);
	pGDC->OutText(_T("Ｘ"), iWidth - 15, iHeight/2 + 4);
	
	tstring sCoord = _T("Ｏ(") + to_tstring((long double)m_Xm)
		+ _T(";") + to_tstring((long double)m_Ym)
		+ _T("), Xs=") + to_tstring((long double)m_Xs)
		+ _T(", Ys=") + to_tstring((long double)m_Ys);
	tstring sTmode = _T("Ｔ[") + to_tstring((long double)m_Tmin)
		+ _T(":") + to_tstring((long double)m_Tmax)
		+ _T("], step=") + to_tstring((long double)m_Tstep);

	::EnterCriticalSection(&csMWState);
	__MainWnd_State __state = state;
	::LeaveCriticalSection(&csMWState);

	tstring sState;
	::EnterCriticalSection(&csPlotted);
	double __dPlotted = dPlotted;
	::LeaveCriticalSection(&csPlotted);

	if(__state == Plotting) sState = _T("Plotting: ") + to_tstring((long double)__dPlotted) + _T("%");
	else if(__state == Idle) sState = _T("Plotted");

	pGDC->OutText(sCoord.c_str(), 10, 10);
	pGDC->OutText(sTmode.c_str(), 10, 26);
	pGDC->OutText(ex.c_str(), 10, 42);
	pGDC->OutText(ey.c_str(), 10, 58);
	pGDC->OutText(sState.c_str(), 10, 74);
	
	theGMaster->ReleaseGRgn(gPaintRgn);
	theGMaster->ReleaseGPen(pen);
	theGMaster->ReleaseGDC(pGDC);
	::EndPaint(hWnd, &ps);

	return true;
}

bool OnSize(HWND hWnd, LPARAM lParam)
{
	iWidth = LOWORD(lParam);
	iHeight = HIWORD(lParam);
	::InvalidateRect(hWnd, nullptr, FALSE);
	return true;
}

bool OnClose(HWND hWnd)
{
	// todo confirmation
	bool bRes = true;
	if (bRes)
	{
		::DestroyWindow(hWnd);
	}
	return true;
}

bool OnFileExit(HWND hWnd)
{
	::SendMessageA(hWnd, WM_CLOSE, 0, 0);
	return true;
}

bool OnAlLinear(HWND hWnd)
{
	::MessageBoxA(hWnd,
		"Algebra:Linear is under construction",
		"UT-GUI",
		MB_ICONINFORMATION | MB_OK);
	return true;
}

bool OnAlSquare(HWND hWnd)
{
	::MessageBoxA(hWnd,
		"Algebra:Square is under construction",
		"UT-GUI",
		MB_ICONINFORMATION | MB_OK);
	return true;
}

bool OnAlCubic(HWND hWnd)
{
	::MessageBoxA(hWnd,
		"Algebra:Cubic is under construction",
		"UT-GUI",
		MB_ICONINFORMATION | MB_OK);
	return true;
}

bool OnAlTreecalc(HWND hWnd)
{
	::DialogBox(hInst, MAKEINTRESOURCE(IDD_AL_TREECALC), hWnd, AlTreecalc);
	::PostMessage(hWnd, WM_PAINT, 0, 0);
	return true;
}

bool OnAlStopPlotting(HWND hWnd)
{
	::EnterCriticalSection(&csMWState);
	__MainWnd_State __state = state;
	::LeaveCriticalSection(&csMWState);
	if (__state == Plotting)
	{
		::EnterCriticalSection(&csCTCmd);
		ct_cmd = StopCalc;
		::LeaveCriticalSection(&csCTCmd);
		::SetEvent(hCTControl);
		::WaitForSingleObject(hCTEvent, -1);
	}
	::PostMessage(hWnd, WM_PAINT, 0, 0);
	return true;
}

bool OnStArrayprop(HWND hWnd)
{
	::DialogBox(hInst, MAKEINTRESOURCE(IDD_ST_ARRAYPROP), hWnd, StatArrProp);
	return true;
}

bool OnHlpAbout(HWND hWnd)
{
	::DialogBox(hInst, MAKEINTRESOURCEW(IDD_ABOUTBOX), hWnd, About);
	return true;
}


// Обработчик сообщений для окна "О программе".
//========== AboutDlg
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//========== AlTreecalcDlg
INT_PTR CALLBACK AlTreecalc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		::SetFocus(::GetDlgItem(hDlg, IDC_EXPR));
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_CALC)
		{
			return OnAlTreecalcCalc(hDlg);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR OnAlTreecalcCalc(HWND hDlg)
{
	HWND hEdExpr = GetDlgItem(hDlg, IDC_EXPR);
	int iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	TCHAR *achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring sExpr(achExpr, iLenExpr - 1);
	delete achExpr;

	tc.ProcessTreeString(sExpr);

	HWND hEdVar = GetDlgItem(hDlg, IDC_VAR);
	int iLenVar = GetWindowTextLength(hEdVar);
	++iLenVar;
	TCHAR *achVar = new TCHAR[iLenVar];
	GetWindowText(hEdVar, achVar, iLenVar);
	tstring sVar(achVar, iLenVar);
	delete achVar;

	map<tstring, double> _var_map;
	tstring x;
	double y;
	tistringstream ss(sVar);
	while(ss >> x >> y)
	{
		_var_map[x] = y;
	}

	pair<double, CalcError> res = tc.substitute(_var_map);
	tstring sRes = to_tstring((long double)res.first);
	tstring sErr = CEtoSTR(res.second);
	::SetWindowText(::GetDlgItem(hDlg, IDC_RES), sRes.c_str());
	::SetWindowText(::GetDlgItem(hDlg, IDC_ERR), sErr.c_str());
	::SetWindowText(::GetDlgItem(hDlg, IDC_HRF), tc.ToInfix().c_str());
	
	return TRUE;
}

//========== TcPlotDlg
INT_PTR OnAlTreecalcPlot(HWND hWnd)
{
	::EnterCriticalSection(&csMWState);
	__MainWnd_State __state = state;
	::LeaveCriticalSection(&csMWState);
	if (__state == Plotting)
	{
		::MessageBox(hWnd, _T("You can't start plotting while other plot is being plotted"),
			_T("UselessTools"), MB_ICONHAND | MB_OK);
		return TRUE;
	}

	// TODO вызов диалога IDD_AL_PLOT
	::DialogBox(hInst, MAKEINTRESOURCE(IDD_AL_PLOT), hWnd, TcPlot);
	::PostMessage(hWnd, WM_PAINT, 0, 0);
	return TRUE;
}

INT_PTR CALLBACK TcPlot(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		::SetFocus(::GetDlgItem(hDlg, IDC_XT));
		::SetWindowText(::GetDlgItem(hDlg, IDC_XT), ex.c_str() + 7);
		::SetWindowText(::GetDlgItem(hDlg, IDC_YT), ey.c_str() + 7);
		::SetWindowText(::GetDlgItem(hDlg, IDC_XM), to_tstring((long double)m_Xm).c_str());
		::SetWindowText(::GetDlgItem(hDlg, IDC_YM), to_tstring((long double)m_Ym).c_str());
		::SetWindowText(::GetDlgItem(hDlg, IDC_XS), to_tstring((long double)m_Xs).c_str());
		::SetWindowText(::GetDlgItem(hDlg, IDC_YS), to_tstring((long double)m_Ys).c_str());
		::SetWindowText(::GetDlgItem(hDlg, IDC_T1), to_tstring((long double)m_Tmin).c_str());
		::SetWindowText(::GetDlgItem(hDlg, IDC_T2), to_tstring((long double)m_Tmax).c_str());
		::SetWindowText(::GetDlgItem(hDlg, IDC_T3), (_T("S") + to_tstring((long double)m_Tstep)).c_str());
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_PLOT)
		{
			return OnTcPlot(hDlg);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR OnTcPlot(HWND hDlg)
{
	HWND hEdExpr = GetDlgItem(hDlg, IDC_XT);
	int iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	TCHAR *achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring sxt(achExpr, iLenExpr - 1);
	delete achExpr;

	hEdExpr = GetDlgItem(hDlg, IDC_YT);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring syt(achExpr, iLenExpr - 1);
	delete achExpr;

	tx.ProcessTreeString(sxt);
	ty.ProcessTreeString(syt);
	ex = _T("X(t) = ") + sxt;
	ey = _T("Y(t) = ") + syt;

	// OriginX
	hEdExpr = GetDlgItem(hDlg, IDC_XM);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring sxm(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream issxm(sxm);
	issxm >> m_Xm;

	// OriginY
	hEdExpr = GetDlgItem(hDlg, IDC_YM);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring sym(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream issym(sym);
	issym >> m_Ym;

	// ScaleX
	hEdExpr = GetDlgItem(hDlg, IDC_XS);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring sxs(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream issxs(sxs);
	issxs >> m_Xs;

	// ScaleY
	hEdExpr = GetDlgItem(hDlg, IDC_YS);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring sys(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream issys(sys);
	issys >> m_Ys;

	// TMIN
	hEdExpr = GetDlgItem(hDlg, IDC_T1);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring st1(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream isst1(st1);
	isst1 >> m_Tmin;

	// TMAX
	hEdExpr = GetDlgItem(hDlg, IDC_T2);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring st2(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream isst2(st2);
	isst2 >> m_Tmax;

	if (m_Tmin > m_Tmax) swap(m_Tmin, m_Tmax);

	// T-MODE
	hEdExpr = GetDlgItem(hDlg, IDC_T3);
	iLenExpr = GetWindowTextLength(hEdExpr);
	++iLenExpr;
	achExpr = new TCHAR[iLenExpr];
	GetWindowText(hEdExpr, achExpr, iLenExpr);
	tstring st3(achExpr, iLenExpr - 1);
	delete achExpr;
	tistringstream isst3(st3);

	if (st3.size() > 1)
	{
		TCHAR ch;
		isst3 >> ch;

		if (ch == _T('S'))
		{
			isst3 >> m_Tstep;
		}
		else if (ch == _T('C'))
		{
			long long cnt;
			isst3 >> cnt;
			m_Tstep = (m_Tmax - m_Tmin) / (cnt - 1.);
		}
	}
	else
	{
		::MessageBox(hDlg,
			_T("You have entered invalid data to [T mode]"),
			_T("Plot Setting"),
			MB_OK | MB_ICONHAND);
		return TRUE;
	}

	// return VARiables...
	/*HWND hEdVar = GetDlgItem(hDlg, IDC_VAR);
	int iLenVar = GetWindowTextLength(hEdVar);
	++iLenVar;
	char *achVar = new char[iLenVar];
	GetWindowText(hEdVar, achVar, iLenVar);
	string sVar(achVar, iLenVar);
	delete achVar;

	string x;
	double y;
	istringstream ss(sVar);
	while(ss >> x >> y)
	{
		var_map_tc[x] = y;
	}*/
	var_map_tc.clear();

	//ct_func = __f_Treecalc;
	calc_func = __f_TcPlot_T;
	//tc_calc = tc;
	::EnterCriticalSection(&csCTCmd);
	ct_cmd = StartCalc;
	::LeaveCriticalSection(&csCTCmd);
	::SetEvent(hCTControl);
	::WaitForSingleObject(hCTEvent, -1);

	::PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);

	return TRUE;
}


//========== StatDlg
INT_PTR CALLBACK StatArrProp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		::SetFocus(::GetDlgItem(hDlg, IDC_ARRAY));
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_CALC)
		{
			return OnStatArrPropCalc(hDlg);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR OnStatArrPropCalc(HWND hDlg)
{
	HWND hEditArray = GetDlgItem(hDlg, IDC_ARRAY);
	int iLenArray = GetWindowTextLength(hEditArray);
	++iLenArray;
	TCHAR *achArray = new TCHAR[iLenArray];
	GetWindowText(hEditArray, achArray, iLenArray);
	tstring sArray(achArray);
	delete achArray;

	tistringstream ss(sArray);
	double temp;
	vector<double> arr, sh, sh2;
	while(ss >> temp)
		arr.push_back(temp);

	if(arr.size() == 0)
	{
		::MessageBox(hDlg,
			_T("You didn't enter anything to array field, so what you've supposed to calc?"),
			_T("Stat:ArrayProp"),
			MB_ICONHAND | MB_OK);
		return (INT_PTR)FALSE;
	}
			
	double sum = 0; for (int i = 0; i < arr.size(); ++i) sum += arr[i];
	//cout << "Sum:\t" << sum << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_SUM), to_tstring((long double)sum).c_str());
	
	double mean = sum / (double)arr.size();
	//cout << "Mean:\t" << mean << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_MEAN), to_tstring((long double)mean).c_str());

	double median = 0;
	sort(arr.begin(), arr.end());
	//cout << "Sorted:\t";
	//for (int i = 0; i < arr.size(); ++i) cout << arr[i] << " ";
	//cout << endl;
	tstring str;
	std::for_each(arr.begin(), arr.end(), [&str](double q) {str += _T(" ") + to_tstring((long double)q); });
	::SetWindowText(::GetDlgItem(hDlg, IDC_SORTED), 1 + str.c_str());

	if (arr.size() % 2 == 0) median = (arr[arr.size() / 2] + arr[arr.size() / 2 - 1]) / 2;
	else median = arr[arr.size() / 2];
	//cout << "Median:\t" << median << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_MEDIAN), to_tstring((long double)median).c_str());

	//cout << "Typical:";
	/*multiset<double> m;
	for (auto k : arr) m.insert(k);
	int max_cnt = 0;
	for (auto k : m) if ((int)m.count(k) > max_cnt) max_cnt = m.count(k);
	set<double> m2;
	for (auto k : m) if ((int)m.count(k) == max_cnt) m2.insert(k);
	//for (auto k : m2) cout << k << " ";
	//cout << endl;
	s = "";
	for (auto k : m2) s += " " + to_string(k);
	::SetWindowTextA(::GetDlgItem(IDC_MODA), 1 + s.c_str());*/

	double min = DBL_MAX, max = DBL_MIN; for (int i = 0; i < arr.size(); ++i) { if (arr[i] < min) min = arr[i]; if (arr[i] > max) max = arr[i]; }
	//cout << "Min:\t" << min << endl;
	//cout << "Max:\t" << max << endl;
	//cout << "Range:\t" << max-min << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_MIN), to_tstring((long double)min).c_str());
	::SetWindowText(::GetDlgItem(hDlg, IDC_MAX), to_tstring((long double)max).c_str());
	::SetWindowText(::GetDlgItem(hDlg, IDC_RANGE), to_tstring((long double)max - min).c_str());

	for (int i = 0; i < arr.size(); ++i) sh.push_back(arr[i] - mean);
	//cout << "Shift:\t";
	//for (int i = 0; i < sh.size(); ++i) cout << sh[i] << " ";
	//cout << endl;
	
	for (int i = 0; i < sh.size(); ++i) sh2.push_back(sh[i]*sh[i]);
	//cout << "Shift2:\t";
	//for (int i = 0; i < sh2.size(); ++i) cout << sh2[i] << " ";
	//cout << endl;

	double sum2 = 0; for (int i = 0; i < sh2.size(); ++i) sum2 += sh2[i];
	//cout << "SumSH2:\t" << sum2 << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_SUMSH2), to_tstring((long double)sum2).c_str());

	double s2 = sum2 / sh2.size();
	//cout << "S^2:\t" << s2 << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_S_2), to_tstring((long double)s2).c_str());

	double s = sqrt(s2);
	//cout << "S:\t" << s << endl;
	::SetWindowText(::GetDlgItem(hDlg, IDC_S), to_tstring((long double)s).c_str());

	return (INT_PTR)TRUE;
}


//========== Painter
int REALtoPX_X(double x)
{
	int pos = (int)round_g(iMaxWidth * (x - m_Xm) / (m_Xs * iMaxWidth / (double)PLOT_UNIT)) + iMaxWidth/2;
	return pos;
}

int REALtoPX_Y(double y)
{
	double dpixel = (double)iMaxHeight * (y - m_Ym) / (m_Ys * iMaxHeight / (double)PLOT_UNIT);
	int pos = iMaxHeight / 2 - (int)round_g(dpixel);
	return pos;
}

double PXtoREAL_X(int x)
{
	double dxu = ((double)x - iMaxWidth/2.) * m_Xs / (double)PLOT_UNIT + m_Xm;
	return dxu;
}

double PXtoREAL_Y(int y)
{
	double dyu = (iMaxHeight/2. - (double)y) * m_Ys / (double)PLOT_UNIT + m_Ym;
	return dyu;
}

void DrawGrid(GDC *pDC)
{
	GBrush *brush = theGMaster->GetGBrush(*GColor::GetColorWhite());
	pDC->FillRect(0, 0, iMaxWidth, iMaxHeight, brush);
	theGMaster->ReleaseGBrush(brush);
	
	int h_mid = iMaxHeight / 2;
	int w_mid = iMaxWidth / 2;

	GPen *oldPen = pDC->GetPen();
	GPen *pen = 0;

	// grid
	pen = theGMaster->GetGPen(1, *GColor::GetColorLGray());
	pDC->SetPen(pen);
	{
		int hor = 0;
		while (hor < h_mid)
		{
			hor += PLOT_UNIT;
			pDC->DrawLine(0, h_mid + hor, iMaxWidth, h_mid + hor);
			pDC->DrawLine(0, h_mid - hor, iMaxWidth, h_mid - hor);
		}
		int ver = 0;
		while (ver < w_mid)
		{
			ver += PLOT_UNIT;
			pDC->DrawLine(w_mid + ver, 0, w_mid + ver, iMaxHeight);
			pDC->DrawLine(w_mid - ver, 0, w_mid - ver, iMaxHeight);
		}
	}
	theGMaster->ReleaseGPen(pen);

	// axes
	pen = theGMaster->GetGPen(2, *GColor::GetColorBlack());
	pDC->SetPen(pen);
	{
		pDC->DrawLine(0, h_mid, iMaxWidth, h_mid);
		pDC->DrawLine(w_mid, 0, w_mid, iMaxHeight);
		pDC->DrawLine(w_mid + PLOT_UNIT, h_mid + PLOT_ZASECKA, w_mid + PLOT_UNIT, h_mid - PLOT_ZASECKA);
		pDC->DrawLine(w_mid + PLOT_ZASECKA, h_mid - PLOT_UNIT, w_mid - PLOT_ZASECKA, h_mid - PLOT_UNIT);
	}
	theGMaster->ReleaseGPen(pen);

	// origin
	GColor *color = theGMaster->GetGColor(0, 0, 255);
	pen = theGMaster->GetGPen(2, *color);
	pDC->SetPen(pen);
	{
		int x = REALtoPX_X(0);
		int y = REALtoPX_Y(0);
		pDC->DrawLine(x - 10, y, x + 10, y);
		pDC->DrawLine(x, y - 10, x, y + 10);
	}
	theGMaster->ReleaseGPen(pen);
	theGMaster->ReleaseGColor(color);

	pDC->SetPen(oldPen);
}

void DrawFunc_F_x_y_0(HWND hWnd, F_x_y_0 func)
{
	GPen *pen = theGMaster->GetGPen(2, *GColor::GetColorRed());
	GMemDC->SetPen(pen);
	for(int X = 0; X < iMaxWidth; ++X)
	{
		for(int Y = 0; Y < iMaxHeight; ++Y)
		{
			if(abs(func(PXtoREAL_X(X), PXtoREAL_Y(Y))) <= max(m_Xs, m_Ys) / (double)PLOT_UNIT)
			{
				GMemDC->DrawLine(X, Y, X, Y);
			}
		}
		if(X % 100 == 0)
		{
			/*CWinTimedMsgBox tmb;
			int iRes = tmb.Run(hWnd, (to_string(X * 100 / (long double)iMaxWidth) + string("% done")).c_str(),
				"Continue?", MB_ICONQUESTION | MB_YESNO, 5000);*/
			int iRes = ::MessageBox(hWnd,
				(to_tstring(X * 100 / (long double)iMaxWidth) + tstring(_T("% done"))).c_str(),
				_T("Continue?"), MB_ICONQUESTION | MB_YESNO);
			if (IDNO == iRes)
			{
				break;
			}
		}
	}
	theGMaster->ReleaseGPen(pen);

	var_map.clear();
}

void DrawFunc_F_x_y_0__Treecalc(HWND hWnd)
{
	GPen *pen = theGMaster->GetGPen(2, *GColor::GetColorRed());
	GMemDC->SetPen(pen);
	double dXMin = PXtoREAL_X(0);
	double dXMax = PXtoREAL_X(iMaxWidth);
	double dYMax = PXtoREAL_Y(0);
	double dYMin = PXtoREAL_Y(iMaxHeight);
	for(int X = 0; X < iMaxWidth; ++X)
	{
		var_map[_T("x")] = PXtoREAL_X(X);
		vector<double> solutions = tc.OptiSubstitute(var_map).Solve(_T("y"), dYMin, dYMax, iMaxHeight);
		for(int i = 0; i < solutions.size(); ++i)
		{
			int Y = REALtoPX_Y(solutions[i]);
			GMemDC->DrawLine(X, Y, X, Y);
		}
		if(X % 100 == 0)
		{
			int iRes = ::MessageBox(hWnd,
				(to_tstring(X * 50 / (long double)iMaxWidth) + tstring(_T("% done"))).c_str(),
				_T("Continue?"), MB_ICONQUESTION | MB_YESNO);
			if (IDNO == iRes)
			{
				break;
			}
		}
	}
	var_map.erase(_T("x"));
	for(int Y = 0; Y < iMaxHeight; ++Y)
	{
		var_map[_T("y")] = PXtoREAL_Y(Y);
		vector<double> solutions = tc.OptiSubstitute(var_map).Solve(_T("x"), dXMin, dXMax, iMaxWidth);
		for(int i = 0; i < solutions.size(); ++i)
		{
			int X = REALtoPX_X(solutions[i]);
			GMemDC->DrawLine(X, Y, X, Y);
		}
		if(Y % 100 == 0)
		{
			int iRes = ::MessageBox(hWnd,
				(to_tstring(50 + Y * 50 / (long double)iMaxHeight) + tstring(_T("% done"))).c_str(),
				_T("Continue?"), MB_ICONQUESTION | MB_YESNO);
			if (IDNO == iRes)
			{
				break;
			}
		}
	}
	var_map.erase(_T("y"));
	theGMaster->ReleaseGPen(pen);

	var_map.clear();
}

double round_g(double a)
{
	return floor(a + 0.5);
}

double __f_Treecalc(double x, double y)
{
	var_map_tc[_T("x")] = x;
	var_map_tc[_T("y")] = y;
	return tc.substitute(var_map_tc).first;
}

pair<pair<double, double>, bool> __f_TcPlot_T(double t)
{
	var_map_tc[_T("t")] = t;
	pair<double, CalcError> r1 = tx.substitute(var_map_tc);
	pair<double, CalcError> r2 = ty.substitute(var_map_tc);
	return make_pair(make_pair(r1.first, r2.first),
		r1.second == CalcError::NoError && r2.second == CalcError::NoError);
}


//====
bool isfinite(double arg)
{
	return arg == arg && 
		arg != std::numeric_limits<double>::infinity() &&
		arg != -std::numeric_limits<double>::infinity();
}
//====

DWORD WINAPI CalcThreadProc(void *arg)
{
	::SetEvent(hCTEvent);
	GPen *pen = theGMaster->GetGPen(2, *GColor::GetColorRed());
	CalcDC->SetPen(pen);
	while(true)
	{
		::WaitForSingleObject(hCTControl, -1);
		// What should we do?
		// 1. kill thread
		// 2. start calculating
		::EnterCriticalSection(&csCTCmd);
		__CalcThr_Command cmd = ct_cmd;
		::LeaveCriticalSection(&csCTCmd);
		::SetEvent(hCTEvent);
		switch(cmd)
		{
		case Kill:
			goto CTP_end;
			break;
		case StartCalc:
			{
				::EnterCriticalSection(&csMWState);
				state = Plotting;
				::LeaveCriticalSection(&csMWState);
				DrawGrid(CalcDC);
				/*for(int X = 0; X < iMaxWidth; ++X)
				{
					for(int Y = 0; Y < iMaxHeight; ++Y)
					{
						if(abs(ct_func(PXtoREAL_X(X), PXtoREAL_Y(Y))) <= max(m_Xs, m_Ys) / (double)PLOT_UNIT)
						{
							CalcDC->DrawLine(X, Y, X, Y);
						}
					}
					if(X % 32 == 0 || X == iMaxWidth - 1)
					{
						::EnterCriticalSection(&csCTCmd);
						__CalcThr_Command __cmd = ct_cmd;
						::LeaveCriticalSection(&csCTCmd);
						if (__cmd != StartCalc)
						{
							goto CTP_SC_end;
						}
						::EnterCriticalSection(&csMemDC);
						GMemDC->CopyArea(0, 0, CalcDC, 0, 0, iMaxWidth, iMaxHeight);
						::LeaveCriticalSection(&csMemDC);
						::EnterCriticalSection(&csPlotted);
						dPlotted = X * 100 / (double) iMaxWidth;
						::LeaveCriticalSection(&csPlotted);
						::InvalidateRect(hMainWnd, 0, FALSE);
					}
				}*/
				double t = m_Tmin;
				int cnt = (m_Tmax - m_Tmin) / m_Tstep + 1;
				bool prev_ok = false;
				int px = 0, py = 0;
				for (int i = 0; i < cnt; ++i, t = m_Tmin + (i * (m_Tmax - m_Tmin)) / (cnt - 1.))
				{
					pair<pair<double, double>, bool> coords = calc_func(t);
					pair<double, double> XY = coords.first;
					bool isOk = coords.second && isfinite(XY.first) && isfinite(XY.second);
					int X = REALtoPX_X(XY.first);
					int Y = REALtoPX_Y(XY.second);
					if (isOk)
					{
						if (prev_ok)
						{
							CalcDC->DrawLine(px, py, X, Y);
						}
						px = X, py = Y;
					}
					prev_ok = isOk;

					if(i % 32 == 0 || i == cnt - 1)
					{
						::EnterCriticalSection(&csCTCmd);
						__CalcThr_Command __cmd = ct_cmd;
						::LeaveCriticalSection(&csCTCmd);
						if (__cmd != StartCalc)
						{
							goto CTP_SC_end;
						}
						::EnterCriticalSection(&csMemDC);
						GMemDC->CopyArea(0, 0, CalcDC, 0, 0, iMaxWidth, iMaxHeight);
						::LeaveCriticalSection(&csMemDC);
						::EnterCriticalSection(&csPlotted);
						dPlotted = i * 100 / (double) (cnt - 1);
						::LeaveCriticalSection(&csPlotted);
						::InvalidateRect(hMainWnd, 0, FALSE);
					}
				}

			CTP_SC_end:
				::EnterCriticalSection(&csMWState);
				state = Idle;
				::LeaveCriticalSection(&csMWState);
				var_map_tc.clear();
			}
			break;
		default:
			// ignore
			break;
		}
	}

CTP_end:
	theGMaster->ReleaseGPen(pen);
	return 0;
}
