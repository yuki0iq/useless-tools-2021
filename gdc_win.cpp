#include "stdafx.h"

#ifdef _WIN32
#   include "base_windows.h"
#  pragma comment(lib, "gdi32") // link with "gdi32.lib"
#  pragma comment(lib, "user32") // link with "user32.lib"
#endif

#include "gdc_win.h"

#include "math.h"

//=============================================================================
#if (defined _WIN32 && defined _DEBUG && !defined DONT_USE_AFX)
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif // (defined _WIN32 && defined _DEBUG && !defined DONT_USE_AFX)


//*****************************************************************************
// GMaster_WIN
//

//=============================================================================
GMaster_WIN* GMaster_WIN::GetGMaster_WIN()
{
	static GMaster_WIN master_WIN;
	return &master_WIN;
}


//=============================================================================
GMaster_WIN::GMaster_WIN()
	: GMaster()
{
}


//=============================================================================
GMaster_WIN::~GMaster_WIN()
{
}


//=============================================================================
GRgn* GMaster_WIN::GetGRgn()
{
	GRgn_WIN *pGRgn = new GRgn_WIN();
	return pGRgn;
}
//=============================================================================
GRgn* GMaster_WIN::GetGRgn(GPoint *pPoints, int iNumPoint)
{
	GRgn_WIN *pGRgn = new GRgn_WIN(pPoints, iNumPoint);
	return pGRgn;
}
//=============================================================================
GRgn* GMaster_WIN::GetGRgn(GRect *pRects, int iNumRect)
{
	GRgn_WIN *pGRgn = new GRgn_WIN(pRects, iNumRect);
	return pGRgn;
}
//=============================================================================
GRgn* GMaster_WIN::GetGRgn(GRect rect)
{
	GRgn_WIN *pGRgn = new GRgn_WIN(rect);
	return pGRgn;
}
//=============================================================================
void GMaster_WIN::ReleaseGRgn(GRgn *pGRgn)
{
	if(NULL != pGRgn)
	{
		delete pGRgn;
	}
}


//=============================================================================
GColor* GMaster_WIN::GetGColor()
{
	GColor_WIN *pGColor = new GColor_WIN();
	return pGColor;
}
//=============================================================================
GColor* GMaster_WIN::GetGColor(const GColor &r_color)
{
	GColor_WIN *pGColor = new GColor_WIN(r_color);
	return pGColor;
}
//=============================================================================
GColor* GMaster_WIN::GetGColor(unsigned char r, unsigned char g, unsigned char b)
{
	GColor_WIN *pGColor = new GColor_WIN(r, g, b);
	return pGColor;
}
//=============================================================================
GColor* GMaster_WIN::GetGColor(unsigned long uColorRGBIndep)
{
	unsigned char r = GetRValue_SC(uColorRGBIndep);
	unsigned char g = GetGValue_SC(uColorRGBIndep);
	unsigned char b = GetBValue_SC(uColorRGBIndep);
	GColor_WIN *pGColor = new GColor_WIN(r, g, b);
	return pGColor;
}
//=============================================================================
void GMaster_WIN::ReleaseGColor(GColor *pGColor)
{
	if(NULL != pGColor)
	{
		delete pGColor;
	}
}


//=============================================================================
GBrush* GMaster_WIN::GetGBrush()
{
	GBrush_WIN *pGBrush = new GBrush_WIN();
	return pGBrush;
}
//=============================================================================
GBrush* GMaster_WIN::GetGBrush(
	unsigned char r, unsigned char g, unsigned char b, EBrushStyle bs/* = EBrushSolid*/)
{
	GBrush_WIN *pGColor = new GBrush_WIN(r, g, b, bs);
	return pGColor;
}
//=============================================================================
GBrush* GMaster_WIN::GetGBrush(const GColor &r_color, EBrushStyle bs/* = EBrushSolid*/)
{
	GBrush_WIN *pGBrush = new GBrush_WIN(r_color, bs);
	return pGBrush;
}
//=============================================================================
void GMaster_WIN::ReleaseGBrush(GBrush *pGBrush)
{
	if(NULL != pGBrush)
	{
		delete pGBrush;
	}
}


//=============================================================================
GPen* GMaster_WIN::GetGPen()
{
	GPen_WIN *pGPen = new GPen_WIN();
	return pGPen;
}
//=============================================================================
GPen* GMaster_WIN::GetGPen(
	int iWidth, const GColor &r_color, EPenStyle ePenStyle/* = ESolid*/)
{
	GPen_WIN *pGPen = new GPen_WIN(iWidth, r_color, ePenStyle);
	return pGPen;
}
//=============================================================================
void GMaster_WIN::ReleaseGPen(GPen *pGPen)
{
	if(NULL != pGPen)
	{
		delete pGPen;
	}
}


//=============================================================================
GFont* GMaster_WIN::GetGFont()
{
	GFont_WIN *pGFont = new GFont_WIN();
	return pGFont;
}
//=============================================================================
GFont* GMaster_WIN::GetDefaultGFont(int iDefaultFontHeight/* = 12*/)
{
	GFont *pDefaultFont = NULL;
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTA lf;
		memset(&lf, 0, sizeof(LOGFONTA));
		::GetObject(hFont, sizeof(LOGFONTA), &lf);
	pDefaultFont = GetGFont(lf.lfFaceName, iDefaultFontHeight);
	return pDefaultFont;
}
//=============================================================================
GFont* GMaster_WIN::GetGFont(
	const char *pchName, int iHeight, EWeight eWeight/* = EMedium*/,
	int iItalic/* = 0*/, int iUnderline/* = 0*/, int iStrikeOut/* = 0*/, unsigned char uCharSet,
	EOrient eOrient/* = EOrient0*/)
{
	GFont_WIN *pGFont = new GFont_WIN(
		pchName, iHeight, eWeight, iItalic, iUnderline, iStrikeOut, uCharSet, eOrient);
	return pGFont;
}
//=============================================================================
void GMaster_WIN::ReleaseGFont(GFont *pGFont)
{
	if(NULL != pGFont)
	{
		delete pGFont;
	}
}


//=============================================================================
GDC* GMaster_WIN::GetGDC(int iWnd)
{
	GDC_WIN *pGDC = new GDC_WIN(iWnd);
	return pGDC;
}
//=============================================================================
GDC* GMaster_WIN::GetMemGDC(int iWnd, int iWidth, int iHeight)
{
	GDC_WIN *pGDC = new GDC_WIN(iWnd, iWidth, iHeight);
	return pGDC;
}
//=============================================================================
GDC* GMaster_WIN::GetMetafileGDC(int iWnd, const char *pchMetafile, int iWidth, int iHeight)
{
	GDC_WIN *pGDC = new GDC_WIN(iWnd, pchMetafile, iWidth, iHeight);
	return pGDC;
}
//=============================================================================
GDC* GMaster_WIN::GetPrinterGDC(char *pchDriverName, char *pchDeviceName, void *pDM)
{
	DEVMODEA *pDevMode = (DEVMODEA*)pDM;
	GDC_WIN *pGDC = new GDC_WIN(pchDriverName, pchDeviceName, pDevMode);
	return pGDC;
}
//=============================================================================
void GMaster_WIN::ReleaseGDC(GDC *pGDC)
{
	if(NULL != pGDC)
	{
		delete pGDC;
	}
}


//=============================================================================
int GMaster_WIN::StartPrintDoc(GDC *pGDC, char *pchDocName)
{
	int iRetCode = -1; // Failed.
	GDC_WIN *pGDC_WIN = (GDC_WIN*)pGDC;
	if(1 == pGDC_WIN->m_iPrintDC)
	{
		// Start print job.
		DOCINFOA docInfo = { sizeof(DOCINFOA), pchDocName, 0, 0, 0 };
		int iRes = ::StartDocA(pGDC_WIN->m_hDC, &docInfo);
		iRetCode = (iRes > 0 ? 0 : -1); // (iRes > 0 ? Success : Failed)
	}
	return iRetCode;
}
//=============================================================================
int GMaster_WIN::EndPrintDoc(GDC *pGDC)
{
	int iRetCode = -1; // Failed.
	GDC_WIN *pGDC_WIN = (GDC_WIN*)pGDC;
	if(1 == pGDC_WIN->m_iPrintDC)
	{
		// End print job.
		int iRes = ::EndDoc(pGDC_WIN->m_hDC);
		iRetCode = (iRes > 0 ? 0 : -1); // (iRes > 0 ? Success : Failed)
	}
	return iRetCode;
}


//=============================================================================
int GMaster_WIN::StartPrintPage(GDC *pGDC)
{
	int iRetCode = -1; // Failed.
	GDC_WIN *pGDC_WIN = (GDC_WIN*)pGDC;
	if(1 == pGDC_WIN->m_iPrintDC)
	{
		// Start page.
		int iRes = ::StartPage(pGDC_WIN->m_hDC);
		iRetCode = (iRes > 0 ? 0 : -1); // (iRes > 0 ? Success : Failed)
	}
	return iRetCode;
}
//=============================================================================
int GMaster_WIN::EndPrintPage(GDC *pGDC)
{
	int iRetCode = -1; // Failed.
	GDC_WIN *pGDC_WIN = (GDC_WIN*)pGDC;
	{
		// End page.
		int iRes = ::EndPage(pGDC_WIN->m_hDC);
		iRetCode = (iRes > 0 ? 0 : -1); // (iRes > 0 ? Success : Failed)
	}
	return iRetCode;
}


//*****************************************************************************
// GRgn_WIN

//=============================================================================
GRgn_WIN::GRgn_WIN()
	: GRgn()
	, m_hRgn(0)
{
	m_hRgn = ::CreateRectRgn(0, 0, 0, 0);
}


//=============================================================================
GRgn_WIN::GRgn_WIN(GPoint *pPoints, int iNumPoint)
	: GRgn()
	, m_hRgn(0)
{
	m_hRgn = ::CreatePolygonRgn((POINT*)pPoints, iNumPoint, WINDING);
}


//=============================================================================
GRgn_WIN::GRgn_WIN(GRect *pRects, int iNumRect)
	: GRgn()
	, m_hRgn(0)
{
	m_hRgn = ::CreateRectRgn(0, 0, 0, 0);

	HRGN hTmpRgn = ::CreateRectRgn(0, 0, 0, 0);
	for(int k = 0; k < iNumRect; k++)
	{
		::SetRectRgn(hTmpRgn, pRects[k].l, pRects[k].t, pRects[k].r, pRects[k].b);
		::CombineRgn(m_hRgn, m_hRgn, hTmpRgn, RGN_OR);
	}
	::DeleteObject(hTmpRgn);
}


//=============================================================================
GRgn_WIN::GRgn_WIN(GRect rect)
	: GRgn()
{
	m_hRgn = ::CreateEllipticRgn(rect.l, rect.t, rect.r, rect.b);
}


//=============================================================================
GRgn_WIN::~GRgn_WIN()
{
	::DeleteObject(m_hRgn);
	m_hRgn = 0;
}


//=============================================================================
void GRgn_WIN::Offset(int dX, int dY)
{
	::OffsetRgn(m_hRgn, dX, dY);
}


//=============================================================================
void GRgn_WIN::Combine(GRgn *pGRgn)
{
	GRgn_WIN *pGRgn_WIN = (GRgn_WIN*)pGRgn;

	if(NULL == pGRgn_WIN)
	{
		return;
	}

	::CombineRgn(m_hRgn, m_hRgn, pGRgn_WIN->m_hRgn, RGN_OR);
}


//=============================================================================
void GRgn_WIN::Combine(int l, int t, int r, int b)
{
	HRGN hTmpRgn = ::CreateRectRgn(l, t, r, b);

	::CombineRgn(m_hRgn, m_hRgn, hTmpRgn, RGN_OR);

	::DeleteObject(hTmpRgn);
}


//=============================================================================
void GRgn_WIN::Intersect(GRgn *pGRgn)
{
	GRgn_WIN *pGRgn_WIN = (GRgn_WIN*)pGRgn;

	if(NULL == pGRgn_WIN)
	{
		::SetRectRgn(m_hRgn, 0, 0, 0, 0);
		return;
	}

	::CombineRgn(m_hRgn, m_hRgn, pGRgn_WIN->m_hRgn, RGN_AND);
}


//=============================================================================
void GRgn_WIN::Subtract(GRgn *pGRgn)
{
	GRgn_WIN *pGRgn_WIN = (GRgn_WIN*)pGRgn;

	if(NULL == pGRgn_WIN)
	{
		return;
	}

	::CombineRgn(m_hRgn, m_hRgn, pGRgn_WIN->m_hRgn, RGN_DIFF);
}


//=============================================================================
void GRgn_WIN::Copy(GRgn *pGRgn)
{
	GRgn_WIN *pGRgn_WIN = (GRgn_WIN*)pGRgn;

	if(NULL == pGRgn_WIN)
	{
		::SetRectRgn(m_hRgn, 0, 0, 0, 0);
		return;
	}

	::CombineRgn(m_hRgn, pGRgn_WIN->m_hRgn, 0, RGN_COPY);
}


//=============================================================================
void GRgn_WIN::CopyDep(const void *rgn)
{
	HRGN hRgn = (HRGN)rgn;

	if(NULL == hRgn)
	{
		::SetRectRgn(m_hRgn, 0, 0, 0, 0);
		return;
	}

	::CombineRgn(m_hRgn, hRgn, 0, RGN_COPY);
}


//=============================================================================
void GRgn_WIN::SetRect(int l, int t, int r, int b)
{
	::SetRectRgn(m_hRgn, l, t, r, b);
}


//=============================================================================
void GRgn_WIN::GetRgnBox(GRect &rect)
{
	RECT r = { 0, 0, 0, 0 };
	::GetRgnBox(m_hRgn,&r);

	rect.l = r.left;
	rect.t = r.top;
	rect.r = r.right;
	rect.b = r.bottom;
}


//=============================================================================
bool GRgn_WIN::RectInRegion(GRect &rect)
{
	RECT r = { rect.l, rect.t, rect.r, rect.b };

	if(::RectInRegion(m_hRgn, &r) == FALSE)
		return false;

	return true;
}


//=============================================================================
int GRgn_WIN::IsEmpty()
{
	int iEmpty = 0; // Empty.

	int iRes = ::CombineRgn(m_hRgn, m_hRgn, m_hRgn, RGN_AND);
	if(ERROR != iRes && NULLREGION != iRes)
	{
		iEmpty = -1; // Not-Empty.
	}

	return iEmpty;
}


//=============================================================================
void* GRgn_WIN::GetRgnDep()
{
	return m_hRgn;
}


//*****************************************************************************
// GColor_WIN

//=============================================================================
GColor_WIN* GColor_WIN::GetColorWhite_WIN()
{
	static GColor_WIN colorWhite((unsigned char)255, (unsigned char)255, (unsigned char)255);
	return &colorWhite;
}
//=============================================================================
GColor_WIN* GColor_WIN::GetColorBlack_WIN()
{
	static GColor_WIN colorBlack(0, 0, 0);
	return &colorBlack;
}
//=============================================================================
GColor_WIN* GColor_WIN::GetColorRed_WIN()
{
	static GColor_WIN colorRed((unsigned char)255, 0, 0);
	return &colorRed;
}
//=============================================================================
GColor_WIN* GColor_WIN::GetColorGray_WIN()
{
	static GColor_WIN colorGray((unsigned char)128, (unsigned char)128, (unsigned char)128);
	return &colorGray;
}
//=============================================================================
GColor_WIN* GColor_WIN::GetColorLGray_WIN()
{
	static GColor_WIN colorLGray((unsigned char)192, (unsigned char)192, (unsigned char)192);
	return &colorLGray;
}


//=============================================================================
GColor_WIN::GColor_WIN()
	: GColor(0, 0, 0)
{
}


//=============================================================================
GColor_WIN::GColor_WIN(const GColor &r_color_WIN)
	: GColor(0, 0, 0)
{
	operator=(r_color_WIN);
}


//=============================================================================
GColor_WIN::GColor_WIN(unsigned char r, unsigned char g, unsigned char b)
	: GColor(r, g, b)
{
}


//=============================================================================
GColor_WIN::~GColor_WIN()
{
}

//=============================================================================
unsigned long GColor_WIN::GetColorRGBDep() const
{
	return RGB(m_red, m_green, m_blue);
}


//=============================================================================
void GColor_WIN::operator =(const GColor_WIN &r_color_WIN)
{
	GColor::operator=(r_color_WIN);
}


//*****************************************************************************
// GBrush_WIN

//=============================================================================
GBrush_WIN* GBrush_WIN::GetBrushWhite_WIN()
{
	static GBrush_WIN brushWhite(*theColorWhite_WIN);
	return &brushWhite;
}
//=============================================================================
GBrush_WIN* GBrush_WIN::GetBrushBlack_WIN()
{
	static GBrush_WIN brushBlack(*theColorBlack_WIN);
	return &brushBlack;
}
//=============================================================================
GBrush_WIN* GBrush_WIN::GetBrushRed_WIN()
{
	static GBrush_WIN brushRed(*theColorRed_WIN);
	return &brushRed;
}
//=============================================================================
GBrush_WIN* GBrush_WIN::GetBrushGray_WIN()
{
	static GBrush_WIN brushGray(*theColorGray_WIN);
	return &brushGray;
}
//=============================================================================
GBrush_WIN* GBrush_WIN::GetBrushLGray_WIN()
{
	static GBrush_WIN brushLGray(*theColorLGray_WIN);
	return &brushLGray;
}


//=============================================================================
GBrush_WIN::GBrush_WIN()
	: GBrush()
	, m_hBrush(0)
	, m_iDeleteBrush(0)
{
	InitBrush(EBrushTypeHollow, EBrushSolid, 0, 0, 0);

	m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
}


//=============================================================================
GBrush_WIN::GBrush_WIN(const GBrush_WIN &r_brush_WIN)
	: GBrush()
{
	GBrush::operator=(r_brush_WIN);
}


//=============================================================================
GBrush_WIN::GBrush_WIN(
	unsigned char r, unsigned char g, unsigned char b, EBrushStyle bs/* = EBrushSolid*/)
	: GBrush()
	, m_hBrush(0)
	, m_iDeleteBrush(0)
{
	InitBrush(EBrushTypeFill, bs, r, g, b);

	CreateHBRUSH(RGB(r, g, b), bs);
}


//=============================================================================
GBrush_WIN::GBrush_WIN(const GColor &r_color, EBrushStyle bs/* = EBrushSolid*/)
	: GBrush()
	, m_hBrush(0)
	, m_iDeleteBrush(0)
{
	InitBrush(EBrushTypeFill, bs, r_color);

	CreateHBRUSH((COLORREF)r_color.GetColorRGBDep(), bs);
}


//=============================================================================
GBrush_WIN::~GBrush_WIN()
{
	if(0 != m_hBrush && 0 != m_iDeleteBrush)
	{
		::DeleteObject(m_hBrush);
	}
	m_hBrush       = 0;
	m_iDeleteBrush = 0;

	theGMaster->ReleaseGColor(m_pBrushColor);
	m_pBrushColor = 0;
}


//=============================================================================
void GBrush_WIN::operator =(const GBrush_WIN &r_brush_WIN)
{
	GBrush::operator=(r_brush_WIN);

	if(0 != m_hBrush && 0 != m_iDeleteBrush)
	{
		::DeleteObject(m_hBrush);
	}
	m_hBrush       = 0;
	m_iDeleteBrush = 0;

	if(EBrushTypeHollow == m_eBrushType)
	{
		m_hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else // (EBrushTypeFill == m_eBrushType)
	{
		CreateHBRUSH((COLORREF)m_pBrushColor->GetColorRGBDep(), m_eBrushStyle);
	}
}


//=============================================================================
void GBrush_WIN::CreateHBRUSH(COLORREF colorRef, EBrushStyle bs)
{
	if(EBrushSolid == bs)
	{
		m_hBrush = ::CreateSolidBrush(colorRef);
	}
	else // (EBrushSolid != bs)
	{
		int fnStyle = 0;
		switch(bs)
		{
			case EBrushHatchHoriz    : fnStyle = HS_HORIZONTAL; break;
			case EBrushHatchVert     : fnStyle = HS_VERTICAL;   break;
			case EBrushHatchFDiag    : fnStyle = HS_FDIAGONAL;  break;
			case EBrushHatchBDiag    : fnStyle = HS_BDIAGONAL;  break;
			case EBrushHatchCross    : fnStyle = HS_CROSS;      break;
			case EBrushHatchDiagCross: fnStyle = HS_DIAGCROSS;  break;
			default: break;
		}
		m_hBrush = ::CreateHatchBrush(fnStyle, colorRef);
	}
	m_iDeleteBrush = 1;
}


//*****************************************************************************
// GPen_WIN

//=============================================================================
GPen_WIN::GPen_WIN()
	: GPen()
	, m_hPen(0)
	, m_iDeletePen(0)
{
}


//=============================================================================
GPen_WIN::GPen_WIN(int iWidth, const GColor &r_color, EPenStyle ePenStyle/* = ESolid*/)
	: GPen()
	, m_hPen(0)
	, m_iDeletePen(1)
{
	int iPenStyleWin = PS_SOLID;
	switch(ePenStyle)
	{
		case EDash      : iPenStyleWin = PS_DASH;       break;
		case EDot       : iPenStyleWin = PS_DOT;        break;
		case EDashDot   : iPenStyleWin = PS_DASHDOT;    break;
		case EDashDotDot: iPenStyleWin = PS_DASHDOTDOT; break;
		default         : // ESolid
		                  iPenStyleWin = PS_SOLID;      break;
	}
	if(/*iWidth > 1 && */iPenStyleWin != PS_SOLID)  //  Для не PS_SOLID типов красивее с PS_GEOMETRIC,
	                                                //  поэтому сделал для любой ширины.
	{
		LOGBRUSH LogBrush = {BS_SOLID, (COLORREF)r_color.GetColorRGBDep(), 0};
		m_hPen = ::ExtCreatePen(iPenStyleWin | PS_GEOMETRIC, iWidth, &LogBrush, 0, 0);
	}
	else
	{
		m_hPen = ::CreatePen(iPenStyleWin, iWidth, (COLORREF)r_color.GetColorRGBDep());
	}
}


//=============================================================================
GPen_WIN::~GPen_WIN()
{
	if(0 != m_hPen && 0 != m_iDeletePen)
	{
		::DeleteObject(m_hPen);
	}
	m_hPen       = 0;
	m_iDeletePen = 0;
}


//*****************************************************************************
// GFont_WIN

//=============================================================================
GFont_WIN::GFont_WIN()
	: GFont()
	, m_hFont(0)
	, m_iDeleteFont(0)
{
}


//=============================================================================
GFont_WIN::GFont_WIN(
	const char *pchName, int iHeight, EWeight eWeight/* = EMedium*/,
	int iItalic/* = 0*/, int iUnderline/* = 0*/, int iStrikeOut/* = 0*/, unsigned char uCharSet/* = 0x00*/,
	EOrient eOrient/* = EOrient0*/)
	: GFont()
	, m_hFont(0)
	, m_iDeleteFont(1)
{
	int iLen = strlen(pchName) + 1;
	m_logFont.m_pchName = new char[iLen];
	strcpy(m_logFont.m_pchName, pchName);

	if(iHeight < 0)
		m_logFont.m_iHeight  = -iHeight + FONT_NEG_HEIGHT_ADD;
	else
		m_logFont.m_iHeight  = iHeight;

	m_logFont.m_eWeight    = eWeight;
	m_logFont.m_iItalic    = iItalic;
	m_logFont.m_iUnderline = iUnderline;
	m_logFont.m_iStrikeOut = iStrikeOut;
	m_logFont.m_uCharSet   = uCharSet;
	m_logFont.m_eOrient    = eOrient;


	LOGFONTA logFontA;
	memset(&logFontA, 0, sizeof(LOGFONTA));
	{
		HGDIOBJ hFont = ::GetStockObject(DEFAULT_GUI_FONT);
		::GetObjectA(hFont, sizeof(LOGFONT), &logFontA);
	}
	strncpy(logFontA.lfFaceName, m_logFont.m_pchName, LF_FACESIZE);
	logFontA.lfHeight     = m_logFont.m_iHeight;
	logFontA.lfWeight     = (EMedium == m_logFont.m_eWeight ? FW_NORMAL : FW_BOLD);
	logFontA.lfItalic     = (1 == m_logFont.m_iItalic ? 0x01 : 0x00);
	logFontA.lfUnderline  = (1 == m_logFont.m_iUnderline ? 0x01 : 0x00);
	logFontA.lfStrikeOut  = (1 == m_logFont.m_iStrikeOut ? 0x01 : 0x00);
	logFontA.lfCharSet    = m_logFont.m_uCharSet;
	//logFontA.lfEscapement  = 0;
	//logFontA.lfOrientation = 0;
	switch(m_logFont.m_eOrient)
	{
		case EOrient90: logFontA.lfEscapement  = 900; logFontA.lfOrientation = 900; break;
		case EOrient180: logFontA.lfEscapement  = 1800; logFontA.lfOrientation = 1800; break;
		case EOrient270: logFontA.lfEscapement  = 2700; logFontA.lfOrientation = 2700; break;
	}
	//
	m_hFont = ::CreateFontIndirectA(&logFontA);


	memset(&logFontA, 0, sizeof(LOGFONTA));
	::GetObjectA(m_hFont, sizeof(LOGFONTA), &logFontA);
	//
	iLen = strlen(logFontA.lfFaceName) + 1;
	m_logFontLoaded.m_pchName = new char[iLen];
	strcpy(m_logFontLoaded.m_pchName, logFontA.lfFaceName);
	m_logFontLoaded.m_iHeight    = logFontA.lfHeight;
	m_logFontLoaded.m_eWeight    = (logFontA.lfWeight <= FW_MEDIUM ? EMedium : EBold);
	m_logFontLoaded.m_iItalic    = (0x00 == logFontA.lfItalic ? 0 : 1);
	m_logFontLoaded.m_iUnderline = (0x00 == logFontA.lfUnderline ? 0 : 1);
	m_logFontLoaded.m_iStrikeOut = (0x00 == logFontA.lfStrikeOut ? 0 : 1);
	m_logFontLoaded.m_uCharSet   = logFontA.lfCharSet;
	m_logFontLoaded.m_eOrient    = EOrient0;
	switch(logFontA.lfEscapement)
	{
		case 900: m_logFontLoaded.m_eOrient = EOrient90; break;
		case 1800: m_logFontLoaded.m_eOrient = EOrient180; break;
		case 2700: m_logFontLoaded.m_eOrient = EOrient270; break;
	}
}


//=============================================================================
GFont_WIN::~GFont_WIN()
{
	if(0 != m_hFont && 0 != m_iDeleteFont)
	{
		::DeleteObject(m_hFont);
	}
	m_hFont       = 0;
	m_iDeleteFont = 0;
}


//*****************************************************************************
// GDC_WIN

//=============================================================================
GDC_WIN::GDC_WIN(int iWnd)
	: m_hWnd((HWND)iWnd)
	, m_hRefDC(0)
	, m_hTmpDC(0)
	, m_hDC(0)
	, m_iNeedDeleteDC(0)
	, m_iPrintDC(0)
	, m_hBitmap(0)
	, m_hOldBitmap(0)
	, m_hPenNull(0)
{
	m_hDC = ::GetDC(m_hWnd);

	m_hPenNull = ::CreatePen(PS_NULL, 0, 0);
}


//=============================================================================
GDC_WIN::GDC_WIN(int iWnd, int iWidth, int iHeight)
	: m_hWnd((HWND)iWnd)
	, m_hRefDC(0)
	, m_hTmpDC(0)
	, m_hDC(0)
	, m_iNeedDeleteDC(1)
	, m_iPrintDC(0)
	, m_hBitmap(0)
	, m_hOldBitmap(0)
	, m_hPenNull(0)
{
	m_hTmpDC     = ::GetDC(m_hWnd);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = iWidth;
	bmi.bmiHeader.biHeight      = -iHeight;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = iWidth * iHeight * 4;

	char *pixels = 0;
	m_hBitmap    = ::CreateDIBSection(m_hTmpDC, &bmi, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
//	m_hBitmap    = ::CreateCompatibleBitmap(m_hTmpDC, iWidth, iHeight);
	if(m_hBitmap != 0)
	{
		m_hDC        = ::CreateCompatibleDC(m_hTmpDC);
		m_hOldBitmap = (HBITMAP)::SelectObject(m_hDC, m_hBitmap);
	}
	else
	{
		m_hDC = m_hTmpDC;
		m_hTmpDC = 0;
	}

	m_hPenNull = ::CreatePen(PS_NULL, 0, 0);
}


//=============================================================================
GDC_WIN::GDC_WIN(int iWnd, const char *pchMetafile, int iWidth, int iHeight)
	: m_hWnd((HWND)iWnd)
	, m_hRefDC(0)
	, m_hTmpDC(0)
	, m_hDC(0)
	, m_iNeedDeleteDC(0)
	, m_iPrintDC(0)
	, m_hBitmap(0)
	, m_hOldBitmap(0)
	, m_hPenNull(0)
{
	m_hRefDC = ::GetDC(m_hWnd);
	//
	RECT rect = { 0, 0, 0, 0 };
	{
		// Determine the picture frame dimensions.
		int iWidthMM    = ::GetDeviceCaps(m_hRefDC, HORZSIZE); // Display width in millimeters.
		int iHeightMM   = ::GetDeviceCaps(m_hRefDC, VERTSIZE); // Display height in millimeters.
		int iWidthPels  = ::GetDeviceCaps(m_hRefDC, HORZRES);  // Display width in pixels.
		int iHeightPels = ::GetDeviceCaps(m_hRefDC, VERTRES);  // Display height in pixels.
		//
		// Convert client coordinates to .01-mm units. 
		// Determine the number of .01-millimeter units per pixel in the x- and y-directions.
		rect.right  = (iWidth  * iWidthMM  * 100) / iWidthPels;
		rect.bottom = (iHeight * iHeightMM * 100) / iHeightPels;
	}
	m_hDC = ::CreateEnhMetaFileA(m_hRefDC, pchMetafile, &rect, NULL);

	m_hPenNull = ::CreatePen(PS_NULL, 0, 0);
}


//=============================================================================
GDC_WIN::GDC_WIN(char *pchDriverName, char *pchDeviceName, DEVMODEA *pDevMode)
	: m_hWnd(0)
	, m_hRefDC(0)
	, m_hTmpDC(0)
	, m_hDC(0)
	, m_iNeedDeleteDC(1)
	, m_iPrintDC(1)
	, m_hBitmap(0)
	, m_hOldBitmap(0)
	, m_hPenNull(0)
{
	m_hDC = ::CreateDCA(pchDriverName, pchDeviceName, NULL, pDevMode);
}


//=============================================================================
GDC_WIN::~GDC_WIN()
{
	if(0 != m_hPenNull)
	{
		::DeleteObject(m_hPenNull);
		m_hPenNull = 0;
	}
	if(0 != m_hRefDC)
	{
		if(0 != m_hDC)
		{
			HENHMETAFILE hEnhMetaFile = ::CloseEnhMetaFile(m_hDC);
			::DeleteEnhMetaFile(hEnhMetaFile);
			m_hDC = 0;
		}

		::ReleaseDC(m_hWnd, m_hRefDC);
		m_hRefDC = 0;
	}
	if(0 != m_hDC)
	{
		if(0 != m_hBitmap)
		{
			::SelectObject(m_hDC, m_hOldBitmap);
		}

		if(1 == m_iNeedDeleteDC)
		{
			::DeleteDC(m_hDC);
		}
		else // (0 == m_iNeedDeleteDC)
		{
			::ReleaseDC(m_hWnd, m_hDC);
		}
		m_hDC = 0;
	}
	if(0 != m_hTmpDC)
	{
		::ReleaseDC(m_hWnd, m_hTmpDC);
		m_hTmpDC = 0;
	}
	if(0 != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = 0;
	}
	m_hOldBitmap = 0;
}


//=============================================================================
void GDC_WIN::MoveTo(int x, int y)
{
	::MoveToEx(m_hDC, x, y, NULL);
}


//=============================================================================
void GDC_WIN::MoveTo(const GPoint &p)
{
	::MoveToEx(m_hDC, p.x, p.y, NULL);
}


//=============================================================================
void GDC_WIN::LineTo(int x, int y)
{
	::LineTo(m_hDC, x, y);
}


//=============================================================================
void GDC_WIN::LineTo(const GPoint &p)
{
	::LineTo(m_hDC, p.x, p.y);
}


//=============================================================================
void GDC_WIN::DrawPixel(int x, int y, unsigned long uColorRGB)
{
////////  SetPixel не работает под Vista, и при R2_XORPEN.
	::MoveToEx(m_hDC, x, y, 0);
	::LineTo(m_hDC, x+1, y);
	//::SetPixelV(m_hDC, x, y, (COLORREF)uColorRGB);
}


//=============================================================================
void GDC_WIN::DrawLine(int x1, int y1, int x2, int y2)
{
	::MoveToEx(m_hDC, x1, y1, NULL);
	::LineTo(m_hDC, x2, y2);
	//DrawPixel(x2, y2, 0);
}


//=============================================================================
void GDC_WIN::DrawRect(int iL, int iT, int iR, int iB)
{
	::Rectangle(m_hDC, iL, iT, iR, iB);
}


//=============================================================================
void GDC_WIN::DrawEllipse(int iL, int iT, int iR, int iB)
{
	::Ellipse(m_hDC, iL, iT, iR, iB);
}


//=============================================================================
void GDC_WIN::DrawArc(const GRect &rc, const GPoint &pS, const GPoint &pE)
{
	::Arc(m_hDC, rc.l, rc.t, rc.r, rc.b, pS.x, pS.y, pE.x, pE.y);
}


//=============================================================================
void GDC_WIN::DrawChord(const GRect &rc, const GPoint &pS, const GPoint &pE)
{
	::Chord(m_hDC, rc.l, rc.t, rc.r, rc.b, pS.x, pS.y, pE.x, pE.y);
}


//=============================================================================
void GDC_WIN::DrawPie(const GRect &rc, const GPoint &pS, const GPoint &pE)
{
	::Pie(m_hDC, rc.l, rc.t, rc.r, rc.b, pS.x, pS.y, pE.x, pE.y);
}


//=============================================================================
void GDC_WIN::DrawRoundRect(const GRect &rc, const GPoint &p)
{
	::RoundRect(m_hDC, rc.l, rc.t, rc.r, rc.b, p.x, p.y);
}


//=============================================================================
void GDC_WIN::DrawText(
	const char *pchText, int iL, int iT, int iR, int iB, EHAlign eHA, EVAlign eVA)
{
	EOrient eOrient = EOrient0;
	{
		GFont *pFont = GetFont();
	eOrient = pFont->GetOrient();
		theGMaster->ReleaseGFont(pFont);
	}

	RECT rc = { iL, iT, iR, iB };
	UINT uFormat = DT_SINGLELINE | DT_NOCLIP;

	if(EOrient0 == eOrient)
	{
		switch(eHA)
		{
			case EHLeft  : uFormat |= DT_LEFT;   break;
			case EHCenter: uFormat |= DT_CENTER; break;
			case EHRight : uFormat |= DT_RIGHT;  break;
			default:
				break;
		}
		switch(eVA)
		{
			case EVTop   : uFormat |= DT_TOP;     break;
			case EVCenter: uFormat |= DT_VCENTER; break;
			case EVBottom: uFormat |= DT_BOTTOM;  break;
			default:
				break;
		}
	}
	else
	{
		uFormat |= DT_LEFT | DT_TOP;
	}

	int iTextHeight = GetTextHeight(pchText);
	int iTextWidth = GetTextWidth(pchText);

	if(EOrient90 == eOrient)
	{
		if(EHCenter == eHA)
		{
			int dX = (rc.right - rc.left)/2 - iTextWidth/2;
			rc.left += dX;
			rc.right += dX;
		}
		if(EHRight == eHA)
		{
			int dX = (rc.right - rc.left) - iTextWidth;
			rc.left += dX;
			rc.right += dX;
		}

		if(EVCenter == eVA)
		{
			int dY = (rc.bottom - rc.top)/2 - iTextHeight/2;
			rc.top += dY;
			rc.bottom += dY;
		}
		if(EVBottom == eVA)
		{
			int dY = (rc.bottom - rc.top) - iTextHeight;
			rc.top += dY;
			rc.bottom += dY;
		}

		rc.top += iTextHeight;
		rc.bottom += iTextHeight;
	}

	if(EOrient180 == eOrient)
	{
		if(EHCenter == eHA)
		{
			int dX = (rc.right - rc.left)/2 - iTextWidth/2;
			rc.left += dX;
			rc.right += dX;
		}
		if(EHRight == eHA)
		{
			int dX = (rc.right - rc.left) - iTextWidth;
			rc.left += dX;
			rc.right += dX;
		}

		if(EVCenter == eVA)
		{
			int dY = (rc.bottom - rc.top)/2 - iTextHeight/2;
			rc.top += dY;
			rc.bottom += dY;
		}
		if(EVBottom == eVA)
		{
			int dY = (rc.bottom - rc.top) - iTextHeight;
			rc.top += dY;
			rc.bottom += dY;
		}

		rc.left += iTextWidth;
		rc.right += iTextWidth;

		rc.top += iTextHeight;
		rc.bottom += iTextHeight;
	}

	if(EOrient270 == eOrient)
	{
		if(EHCenter == eHA)
		{
			int dX = (rc.right - rc.left)/2 - iTextWidth/2;
			rc.left += dX;
			rc.right += dX;
		}
		if(EHRight == eHA)
		{
			int dX = (rc.right - rc.left) - iTextWidth;
			rc.left += dX;
			rc.right += dX;
		}

		if(EVCenter == eVA)
		{
			int dY = (rc.bottom - rc.top)/2 - iTextHeight/2;
			rc.top += dY;
			rc.bottom += dY;
		}
		if(EVBottom == eVA)
		{
			int dY = (rc.bottom - rc.top) - iTextHeight;
			rc.top += dY;
			rc.bottom += dY;
		}

		rc.left += iTextWidth;
		rc.right += iTextWidth;
	}

	UINT uOldTextAlign = ::GetTextAlign(m_hDC);
	::SetTextAlign(m_hDC, TA_LEFT | TA_TOP | TA_NOUPDATECP);
	//
	::DrawTextA(m_hDC, pchText, -1, &rc, uFormat);
	//
	::SetTextAlign(m_hDC, uOldTextAlign);
}


//=============================================================================
void GDC_WIN::DrawTextEllipsis(
	const char *pchText, int iL, int iT, int iR, int iB, EHAlign eHA, EVAlign eVA)
{
	EOrient eOrient = EOrient0;
	{
		GFont *pFont = GetFont();
	eOrient = pFont->GetOrientLoaded();
		theGMaster->ReleaseGFont(pFont);
	}

	int iAllowedTextWidth = iR-iL; // (EOrient0 == eOrient || EOrient180 == eOrient)
	if(EOrient90 == eOrient || EOrient270 == eOrient)
	{
		iAllowedTextWidth = iB-iT;
	}

	int iLen = strlen(pchText) + 1;
	char *pchTextEllipsis = new char[iLen];
	strcpy(pchTextEllipsis, pchText);

	char *pchTextToDraw = pchTextEllipsis;
	//
	int iWhile = 1;
	while(iWhile)
	{
		int iTextWidth = 0;
		int iTextHeight = 0;
		GetTextWH(pchTextToDraw, iTextWidth, iTextHeight);
		if(EOrient90 == eOrient || EOrient270 == eOrient)
		{
			iTextWidth = iTextHeight;
		}
		if(iTextWidth <= iAllowedTextWidth)
		{
			break;
		}

		iLen = strlen(pchTextToDraw);
		if(iLen <= 3)
		{
			pchTextToDraw[0] = '\0';
			break;
		}

		switch(eHA)
		{
			case EHLeft: // "abcdefgh" -> "abcde.."
			{
				pchTextToDraw[iLen-3] = '.';
				pchTextToDraw[iLen-2] = '.';
				pchTextToDraw[iLen-1] = '\0';
			} break;
			case EHCenter: // "abcdefgh" -> ".cdef."
			{
				pchTextToDraw[iLen-1] = '\0';
				pchTextToDraw++;
				iLen = strlen(pchTextToDraw);
				if(iLen < 3)
				{
					pchTextToDraw[0] = '\0';
				}
				else
				{
					pchTextToDraw[0]      = '.';
					pchTextToDraw[iLen-1] = '.';
				}
			} break;
			case EHRight: // "abcdefgh" -> "..defgh"
			{
				pchTextToDraw++;
				pchTextToDraw[0] = '.';
				pchTextToDraw[1] = '.';
			} break;
			default:
				break;
		}
	}
	//
	DrawText(pchTextToDraw, iL, iT, iR, iB, eHA, eVA);

	delete[] pchTextEllipsis;
}


//=============================================================================
void GDC_WIN::OutText(const wchar_t *pchText, int iX, int iY)
{
	::TextOutW(m_hDC, iX, iY, pchText, wcslen(pchText));
}


//=============================================================================
void GDC_WIN::FillRect(int iL, int iT, int iR, int iB, GBrush *pBrush/* = 0*/)
{
	RECT rc = { iL, iT, iR, iB };
	HBRUSH hBrush = 0;
	if(NULL != pBrush)
	{
		GBrush_WIN *pBrush_WIN = (GBrush_WIN*)pBrush;
		hBrush = pBrush_WIN->m_hBrush;
	}
	else // (NULL == hBrush)
	{
		hBrush = (HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH);
	}

	::FillRect(m_hDC, &rc, hBrush);
}


//=============================================================================
void GDC_WIN::FillPolygon(GPoint *pPoints, int iNumPoints, GBrush *pBrush/* = 0*/)
{
	HPEN hOldPen = (HPEN)::SelectObject(m_hDC, m_hPenNull);
	HBRUSH hOldBrush = 0;
	if(NULL != pBrush)
	{
		GBrush_WIN *pBrush_WIN = (GBrush_WIN*)pBrush;
		hOldBrush = (HBRUSH)::SelectObject(m_hDC, pBrush_WIN->m_hBrush);
	}

	::Polygon(m_hDC, (POINT*)pPoints, iNumPoints);

	if(NULL != pBrush)
	{
		::SelectObject(m_hDC, hOldBrush);
	}
	::SelectObject(m_hDC, hOldPen);
}


//=============================================================================
void GDC_WIN::DrawFocusRect(int iL, int iT, int iR, int iB)
{
	RECT r = { iL, iT, iR, iB };
	::DrawFocusRect(m_hDC, &r);
}


//=============================================================================
void GDC_WIN::DrawPushButton(int iL, int iT, int iR, int iB, bool bPushed, bool bFocused)
{
	RECT r = { iL, iT, iR, iB };
	UINT uState = DFCS_BUTTONPUSH;
	if(bPushed)
	{
		uState |= DFCS_PUSHED;
	}
	::DrawFrameControl(m_hDC, &r, DFC_BUTTON, uState);

	if(bFocused)
	{
		unsigned long uOlBkColor = GetBackgroundColor();
		SetBackgroundColor(theColorGray_WIN->GetColorRGBDep());
		DrawFocusRect(iL+4, iT+4, iR-4, iB-4);
		SetBackgroundColor(uOlBkColor);
	}
}


//=============================================================================
void GDC_WIN::DrawCheckButton(int iL, int iT, int iR, int iB, bool bChecked)
{
	RECT r = { iL, iT, iR, iB };
	UINT uState = DFCS_BUTTONCHECK;
	if(bChecked)
	{
		uState |= DFCS_CHECKED;
	}
	::DrawFrameControl(m_hDC, &r, DFC_BUTTON, uState);
}



//=============================================================================
int GDC_WIN::GetPhysicalWidth()
{
	return ::GetDeviceCaps(m_hDC, PHYSICALWIDTH);
}
//=============================================================================
int GDC_WIN::GetPhysicalHeight()
{
	return ::GetDeviceCaps(m_hDC, PHYSICALHEIGHT);
}
//=============================================================================
int GDC_WIN::GetPhysicalOffsetX()
{
	return ::GetDeviceCaps(m_hDC, PHYSICALOFFSETX);
}
//=============================================================================
int GDC_WIN::GetPhysicalOffsetY()
{
	return ::GetDeviceCaps(m_hDC, PHYSICALOFFSETY);
}
//=============================================================================
int GDC_WIN::GetLogicalHorzSzMM()
{
	return ::GetDeviceCaps(m_hDC, HORZSIZE);
}
//=============================================================================
int GDC_WIN::GetLogicalVertSzMM()
{
	return ::GetDeviceCaps(m_hDC, VERTSIZE);
}
//=============================================================================
int GDC_WIN::GetLogicalDpiX()
{
	return ::GetDeviceCaps(m_hDC, LOGPIXELSX);
}
//=============================================================================
int GDC_WIN::GetLogicalDpiY()
{
	return ::GetDeviceCaps(m_hDC, LOGPIXELSY);
}


//=============================================================================
int GDC_WIN::GetMapMode()
{
	int iMapMode = ::GetMapMode(m_hDC);
	return (MM_ANISOTROPIC == iMapMode ? 1 : 0);
}


//=============================================================================
int GDC_WIN::SetMapMode(int iMode)
{
	int iMapMode = (0 == iMode ? MM_TEXT : MM_ANISOTROPIC);
	int iRes = ::SetMapMode(m_hDC, iMapMode);
	return (0 == iRes ? -1 : 0);
}


//=============================================================================
int GDC_WIN::GetWindowOrigin(int& r_iX, int& r_iY)
{
	POINT pp;
	BOOL bRes = ::GetWindowOrgEx(m_hDC, &pp);
	if(FALSE == bRes)
	{
		return -1; // Failed.
	}
	r_iX = pp.x;
	r_iY = pp.y;
	return 0; // Success.
}


//=============================================================================
int GDC_WIN::SetWindowOrigin(int iX, int iY)
{
	BOOL bRes = ::SetWindowOrgEx(m_hDC, iX, iY, NULL);
	return (FALSE == bRes ? -1 : 0); // (FALSE == bRes ? Failed : Success)
}


//=============================================================================
int GDC_WIN::GetViewportOrigin(int& r_iX, int& r_iY)
{
	POINT pp;
	BOOL bRes = ::GetViewportOrgEx(m_hDC, &pp);
	if(FALSE == bRes)
	{
		return -1; // Failed.
	}
	r_iX = pp.x;
	r_iY = pp.y;
	return 0; // Success.
}


//=============================================================================
int GDC_WIN::SetViewportOrigin(int iX, int iY)
{
	BOOL bRes = ::SetViewportOrgEx(m_hDC, iX, iY, NULL);
	return (FALSE == bRes ? -1 : 0); // (FALSE == bRes ? Failed : Success)
}


//=============================================================================
int GDC_WIN::GetWindowExtents(int& r_iX, int& r_iY)
{
	SIZE sz;
	BOOL bRes = ::GetWindowExtEx(m_hDC, &sz);
	if(FALSE == bRes)
	{
		return -1; // Failed.
	}
	r_iX = sz.cx;
	r_iY = sz.cy;
	return 0; // Success.
}


//=============================================================================
int GDC_WIN::SetWindowExtents(int iX, int iY)
{
	BOOL bRes = ::SetWindowExtEx(m_hDC, iX, iY, NULL);
	return (FALSE == bRes ? -1 : 0); // (FALSE == bRes ? Failed : Success)
}


//=============================================================================
int GDC_WIN::GetViewportExtents(int& r_iX, int& r_iY)
{
	SIZE sz;
	BOOL bRes = ::GetViewportExtEx(m_hDC, &sz);
	if(FALSE == bRes)
	{
		return -1; // Failed.
	}
	r_iX = sz.cx;
	r_iY = sz.cy;
	return 0; // Success.
}


//=============================================================================
int GDC_WIN::SetViewportExtents(int iX, int iY)
{
	BOOL bRes = ::SetViewportExtEx(m_hDC, iX, iY, NULL);
	return (FALSE == bRes ? -1 : 0); // (FALSE == bRes ? Failed : Success)
}


//=============================================================================
int GDC_WIN::GetFlushMode()
{
	return 0;
}
//=============================================================================
void GDC_WIN::SetFlushMode(int /*iFlushImmediately*/)
{
}
//=============================================================================
void GDC_WIN::Flush()
{
}


//=============================================================================
GRgn* GDC_WIN::GetClipRegion(bool *pClipExist)
{
	GRgn_WIN *pClipRgn_WIN = (GRgn_WIN*)theGMaster->GetGRgn();

	int GCRet = ::GetClipRgn(m_hDC, pClipRgn_WIN->m_hRgn);

	*pClipExist = 0 == GCRet ? false : true;

	return pClipRgn_WIN;
}


//=============================================================================
void GDC_WIN::SetClipRegion(const GRgn *pClipRgn)
{
	HRGN hClipRgn = 0;
	if(NULL != pClipRgn)
	{
		GRgn_WIN *pClipRgn_WIN = (GRgn_WIN*)pClipRgn;
		hClipRgn = pClipRgn_WIN->m_hRgn;
	}
	::SelectClipRgn(m_hDC, hClipRgn);
}


//=============================================================================
void GDC_WIN::SetClipRegionDep(const void *rgn)
{
	::SelectClipRgn(m_hDC, (HRGN)rgn);
}


//=============================================================================
EBkMode GDC_WIN::GetBackgroundMode()
{
	int iBkMode = ::GetBkMode(m_hDC);
	if(TRANSPARENT == iBkMode)
	{
		return ETransparent;
	}
	return EOpaque;
}


//=============================================================================
void GDC_WIN::SetBackgroundMode(EBkMode eBkMode)
{
	int iBkMode = (ETransparent == eBkMode ? TRANSPARENT : OPAQUE);
	::SetBkMode(m_hDC, iBkMode);
}


//=============================================================================
unsigned long GDC_WIN::GetBackgroundColor()
{
	return ::GetBkColor(m_hDC);
}


//=============================================================================
void GDC_WIN::SetBackgroundColor(unsigned long uColorRGB)
{
	::SetBkColor(m_hDC, (COLORREF)uColorRGB);
}


//=============================================================================
int GDC_WIN::GetROP2()
{
	return ::GetROP2(m_hDC);
}


//=============================================================================
void GDC_WIN::SetROP2copy()
{
	::SetROP2(m_hDC, R2_COPYPEN);
}


//=============================================================================
void GDC_WIN::SetROP2xor()
{
	::SetROP2(m_hDC, R2_XORPEN);
}


//=============================================================================
void GDC_WIN::SetROP2not()
{
	::SetROP2(m_hDC, R2_NOT);
}


//=============================================================================
void GDC_WIN::RestoreROP2(int iRop)
{
	::SetROP2(m_hDC, iRop);
}


//=============================================================================
unsigned long GDC_WIN::GetTextColor()
{
	return ::GetTextColor(m_hDC);
}


//=============================================================================
void GDC_WIN::SetTextColor(unsigned long uColorRGB)
{
	::SetTextColor(m_hDC, (COLORREF)uColorRGB);
}


//=============================================================================
void GDC_WIN::GetTextAlign(EHAlign &r_eHA, EVAlign &r_eVA)
{
	UINT uTA = ::GetTextAlign(m_hDC);
	switch(uTA & (TA_LEFT | TA_CENTER | TA_RIGHT))
	{
		case TA_LEFT  : r_eHA = EHLeft;   break;
		case TA_CENTER: r_eHA = EHCenter; break;
		case TA_RIGHT : r_eHA = EHRight;  break;
		default: break;
	}
	switch(uTA & (TA_TOP | TA_BASELINE | TA_BOTTOM))
	{
		case TA_TOP     : r_eVA = EVTop;    break;
		case TA_BASELINE: r_eVA = EVCenter; break;
		case TA_BOTTOM  : r_eVA = EVBottom; break;
		default: break;
	}
}


//=============================================================================
void GDC_WIN::SetTextAlign(EHAlign eHA, EVAlign eVA)
{
	UINT uTA = 0;
	switch(eHA)
	{
		case EHLeft  : uTA |= TA_LEFT;   break;
		case EHCenter: uTA |= TA_CENTER; break;
		case EHRight : uTA |= TA_RIGHT;  break;
		default: break;
	}
	switch(eVA)
	{
		case EVTop   : uTA |= TA_TOP;      break;
		case EVCenter: uTA |= TA_BASELINE; break;
		case EVBottom: uTA |= TA_BOTTOM;   break;
		default: break;
	}
	::SetTextAlign(m_hDC, uTA);
}


//=============================================================================
GPen* GDC_WIN::GetPen()
{
	GPen_WIN *pCurPen_WIN = (GPen_WIN*)theGMaster->GetGPen();

	pCurPen_WIN->m_hPen = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);

	return pCurPen_WIN;
}


//=============================================================================
void GDC_WIN::SetPen(GPen *pPen)
{
	HPEN hPen = 0;
	if(NULL != pPen)
	{
		GPen_WIN *pPen_WIN = (GPen_WIN*)pPen;
		hPen = pPen_WIN->m_hPen;
	}
	else
	{
		hPen = m_hPenNull;
	}
	::SelectObject(m_hDC, hPen);
}


//=============================================================================
GFont* GDC_WIN::GetFont()
{
	GFont_WIN *pCurFont_WIN = (GFont_WIN*)theGMaster->GetGFont();

	pCurFont_WIN->m_hFont = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
	pCurFont_WIN->m_iDeleteFont = 0;

	LOGFONTA lf;
	memset(&lf, 0, sizeof(LOGFONTA));
	int iRes = ::GetObject(pCurFont_WIN->m_hFont, sizeof(LOGFONTA), &lf);
	if(0 != iRes)
	{
		int iLen = strlen(lf.lfFaceName) + 1;
		pCurFont_WIN->m_logFont.m_pchName = new char[iLen];
		strcpy(pCurFont_WIN->m_logFont.m_pchName, lf.lfFaceName);
		pCurFont_WIN->m_logFont.m_iHeight    = lf.lfHeight;
		pCurFont_WIN->m_logFont.m_eWeight    = (FW_BOLD == lf.lfWidth ? EBold : EMedium);
		pCurFont_WIN->m_logFont.m_iItalic    = (0x00 == lf.lfItalic ? 0 : 1);
		pCurFont_WIN->m_logFont.m_iUnderline = (0x00 == lf.lfUnderline ? 0 : 1);
		pCurFont_WIN->m_logFont.m_iStrikeOut = (0x00 == lf.lfStrikeOut ? 0 : 1);
		pCurFont_WIN->m_logFont.m_uCharSet   = lf.lfCharSet;
		pCurFont_WIN->m_logFont.m_eOrient    = EOrient0;
		switch(lf.lfEscapement)
		{
			case 900: pCurFont_WIN->m_logFont.m_eOrient = EOrient90; break;
			case 1800: pCurFont_WIN->m_logFont.m_eOrient = EOrient180; break;
			case 2700: pCurFont_WIN->m_logFont.m_eOrient = EOrient270; break;
		}

		iLen = strlen(pCurFont_WIN->m_logFont.m_pchName) + 1;
		pCurFont_WIN->m_logFontLoaded.m_pchName = new char[iLen];
		strcpy(pCurFont_WIN->m_logFontLoaded.m_pchName, pCurFont_WIN->m_logFont.m_pchName);
		pCurFont_WIN->m_logFontLoaded.m_iHeight    = pCurFont_WIN->m_logFont.m_iHeight;
		pCurFont_WIN->m_logFontLoaded.m_eWeight    = pCurFont_WIN->m_logFont.m_eWeight;
		pCurFont_WIN->m_logFontLoaded.m_iItalic    = pCurFont_WIN->m_logFont.m_iItalic;
		pCurFont_WIN->m_logFontLoaded.m_iUnderline = pCurFont_WIN->m_logFont.m_iUnderline;
		pCurFont_WIN->m_logFontLoaded.m_iStrikeOut = pCurFont_WIN->m_logFont.m_iStrikeOut;
		pCurFont_WIN->m_logFontLoaded.m_uCharSet   = pCurFont_WIN->m_logFont.m_uCharSet;
		pCurFont_WIN->m_logFontLoaded.m_eOrient    = pCurFont_WIN->m_logFont.m_eOrient;
	}

	return pCurFont_WIN;
}


//=============================================================================
void GDC_WIN::SetFont(GFont *pFont)
{
	GFont_WIN *pFont_WIN = (GFont_WIN*)pFont;
	::SelectObject(m_hDC, pFont_WIN->m_hFont);
}


//=============================================================================
GBrush* GDC_WIN::GetBrush()
{
	GBrush_WIN *pCurBrush_WIN = (GBrush_WIN*)theGMaster->GetGBrush();

	pCurBrush_WIN->m_hBrush = (HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH);

	return pCurBrush_WIN;
}


//=============================================================================
void GDC_WIN::SetBrush(GBrush *pBrush)
{
	HBRUSH hBrush = 0;
	if(NULL != pBrush)
	{
		GBrush_WIN *pBrush_WIN = (GBrush_WIN*)pBrush;
		hBrush = pBrush_WIN->m_hBrush;
	}
	else
	{
		hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	::SelectObject(m_hDC, hBrush);
}


//=============================================================================
int GDC_WIN::GetTextHeight(const char *pchText)
{
	int iTextWidth = 0;
	int iTextHeight = 0;
	GetTextWH(pchText, iTextWidth, iTextHeight);
	return iTextHeight;
}


//=============================================================================
int GDC_WIN::GetTextWidth(const char *pchText)
{
	int iTextWidth = 0;
	int iTextHeight = 0;
	GetTextWH(pchText, iTextWidth, iTextHeight);
	return iTextWidth;
}


//=============================================================================
GSize GDC_WIN::GetTextExtent(const char *pchText)
{
	SIZE szTxt;
	memset(&szTxt, 0, sizeof(SIZE));
	::GetTextExtentPoint32A(m_hDC, pchText, strlen(pchText), &szTxt);
	return GSize(szTxt.cx, szTxt.cy);
}


//=============================================================================
void GDC_WIN::GetTextWH(const char *pchText, int& r_iCx, int& r_iCy)
{
	SIZE szTxt;
	memset(&szTxt, 0, sizeof(SIZE));
	::GetTextExtentPoint32A(m_hDC, pchText, strlen(pchText), &szTxt);

	EOrient eOrient = EOrient0;
	{
		GFont *pFont = GetFont();
	eOrient = pFont->GetOrient();
		theGMaster->ReleaseGFont(pFont);
	}

	if(EOrient0 == eOrient || EOrient180 == eOrient)
	{
		r_iCx = szTxt.cx;
		r_iCy = szTxt.cy;
	}
	else // (EOrient90 == eOrient || EOrient270 == eOrient)
	{
		r_iCx = szTxt.cy;
		r_iCy = szTxt.cx;
	}
}


//=============================================================================
int GDC_WIN::CopyArea(
	int iDestX, int iDestY, GDC *pSrcGDC, int iSrcX, int iSrcY, int iWidth, int iHeight)
{
	GDC_WIN *pSrcGDC_WIN = (GDC_WIN*)pSrcGDC;

	BOOL bRes =
		::BitBlt(m_hDC, iDestX, iDestY, iWidth, iHeight, pSrcGDC_WIN->m_hDC, iSrcX, iSrcY, SRCCOPY);

	return (FALSE == bRes ? -1 : 0);
}


//=============================================================================
void* GDC_WIN::GetBITMAP()
{
	return (const HBITMAP)m_hBitmap;
}


//=============================================================================
int GDC_WIN::GetDimension(int *piWidth, int *piHeight)
{
	int iRetCode = -1; // Failed.
	if(NULL != m_hBitmap)
	{
		BITMAP bitmapBuffer;
		memset(&bitmapBuffer, 0, sizeof(BITMAP));
		int iSize = ::GetObject(m_hBitmap, sizeof(BITMAP), &bitmapBuffer);
		if(sizeof(BITMAP) == iSize)
		{
			*piWidth  = bitmapBuffer.bmWidth;
			*piHeight = bitmapBuffer.bmHeight;

			iRetCode = 0; // Success.
		}
	}
	return iRetCode;
}


//=============================================================================
int GDC_WIN::CopyToBITMAP(void *pDestBitmap, int iWidth, int iHeight)
{
	int iRetCode = -1; // Failed.
	if(0 != m_hDC)
	{
		HBITMAP hBitmap = (HBITMAP)pDestBitmap;

		HDC hTmpDC = ::CreateCompatibleDC(m_hDC);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hTmpDC, hBitmap);
		//
		BOOL bRes = ::BitBlt(hTmpDC, 0, 0, iWidth, iHeight, m_hDC, 0, 0, SRCCOPY);
		if(TRUE == bRes)
		{
			iRetCode = 0; // Success.
		}
		//
		::SelectObject(hTmpDC, hOldBitmap);
		::DeleteDC(hTmpDC);
	}
	return iRetCode;
}
