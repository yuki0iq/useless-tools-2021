#ifndef GDC_WIN_H__EED29528_5CF8_4819_B059_C157EF5A6DDA__INCLUDED
#define GDC_WIN_H__EED29528_5CF8_4819_B059_C157EF5A6DDA__INCLUDED

//=============================================================================
#ifdef _WIN32
#   include "base_windows.h"
#endif

#include "gdc.h"


//=============================================================================
class GDC_WIN;


//*****************************************************************************
class GMaster_WIN : public GMaster
{
public:
	static GMaster_WIN* GetGMaster_WIN();

private:
	GMaster_WIN();
public:
	virtual ~GMaster_WIN();

public: // GMaster
	// Region.
	virtual GRgn* GetGRgn();
	virtual GRgn* GetGRgn(GPoint *pPoints, int iNumPoint);
	virtual GRgn* GetGRgn(GRect *pRects, int iNumRect);
	virtual GRgn* GetGRgn(GRect rect);
	virtual void ReleaseGRgn(GRgn *pGRgn);

	// Color.
	virtual GColor* GetGColor();
	virtual GColor* GetGColor(const GColor &r_color);
	virtual GColor* GetGColor(unsigned char r, unsigned char g, unsigned char b);
	virtual GColor* GetGColor(unsigned long uColorRGBIndep);
	virtual void ReleaseGColor(GColor *pGColor);

	// Brush.
	virtual GBrush* GetGBrush();
	virtual GBrush* GetGBrush(unsigned char r, unsigned char g, unsigned char b, EBrushStyle bs = EBrushSolid);
	virtual GBrush* GetGBrush(const GColor &r_color, EBrushStyle bs = EBrushSolid);
	virtual void ReleaseGBrush(GBrush *pGBrush);

	// Pen.
	virtual GPen* GetGPen();
	virtual GPen* GetGPen(int iWidth, const GColor &r_color, EPenStyle ePenStyle = ESolid);
	virtual void ReleaseGPen(GPen *pGPen);

	// Font.
	virtual GFont* GetDefaultGFont(int iDefaultFontHeight = 12);
	virtual GFont* GetGFont();
//	virtual GFont* GetGFont(GLogFont *pGLogFont);
	virtual GFont* GetGFont(
		const char *pchName, int iHeight, EWeight eWeight = EMedium,
		int iItalic = 0, int iUnderline = 0, int iStrikeOut = 0, unsigned char uCharSet = 0x00,
		EOrient eOrient = EOrient0);
	virtual void ReleaseGFont(GFont *pGFont);

	// DC.
	virtual GDC* GetGDC(int iWnd);
	virtual GDC* GetMemGDC(int iWnd, int iWidth, int iHeight);
	virtual GDC* GetMetafileGDC(int iWnd, const char *pchMetafile, int iWidth, int iHeight);
	virtual GDC* GetPrinterGDC(char *pchDriverName, char *pchDeviceName, void *pDM);
	virtual void ReleaseGDC(GDC *pGDC);

	virtual int StartPrintDoc(GDC *pGDC, char *pchDocName);
	virtual int EndPrintDoc(GDC *pGDC);
	//
	virtual int StartPrintPage(GDC *pGDC);
	virtual int EndPrintPage(GDC *pGDC);
};
#define theGMaster_WIN     ( GMaster_WIN::GetGMaster_WIN() )


//*****************************************************************************
class GRgn_WIN : public GRgn
{
friend class GMaster_WIN;
friend class GDC_WIN;

private:
	GRgn_WIN(); // Empty (Null) rgn
	GRgn_WIN(GPoint *pPoints, int iNumPoint);
	GRgn_WIN(GRect *pRects, int iNumRect);
	GRgn_WIN(GRect rect); // Elliptic rgn.
public:
	virtual ~GRgn_WIN();

public:
	virtual void Offset(int dX, int dY);
	virtual void Combine(GRgn *pGRgn);
	virtual void Combine(int l, int t, int r, int b);
	virtual void Intersect(GRgn *pGRgn);
	virtual void Subtract(GRgn *pGRgn);
	virtual void Copy(GRgn *pGRgn);
	virtual void CopyDep(const void *rgn);
	virtual void SetRect(int l, int t, int r, int b);
	virtual void GetRgnBox(GRect &rect);
	virtual bool RectInRegion(GRect &rect);
	virtual int IsEmpty();

	virtual void* GetRgnDep();

private:
	HRGN m_hRgn;
};


//*****************************************************************************
class GColor_WIN : public GColor
{
friend class GMaster_WIN;
friend class GDC_WIN;

public:
	static GColor_WIN* GetColorWhite_WIN();
	static GColor_WIN* GetColorBlack_WIN();
	static GColor_WIN* GetColorRed_WIN();
	static GColor_WIN* GetColorGray_WIN();
	static GColor_WIN* GetColorLGray_WIN();

private:
	GColor_WIN();
	GColor_WIN(const GColor &r_color_WIN);
	GColor_WIN(unsigned char r, unsigned char g, unsigned char b);
public:
	virtual ~GColor_WIN();

public:
	virtual unsigned long GetColorRGBDep() const;
	virtual void operator =(const GColor &r_color) { operator=((const GColor_WIN&)r_color); }

public:
	void operator =(const GColor_WIN &r_color_WIN);
};
#define theColorWhite_WIN  ( GColor_WIN::GetColorWhite_WIN() )
#define theColorBlack_WIN  ( GColor_WIN::GetColorBlack_WIN() )
#define theColorRed_WIN    ( GColor_WIN::GetColorRed_WIN()   )
#define theColorGray_WIN   ( GColor_WIN::GetColorGray_WIN()  )
#define theColorLGray_WIN  ( GColor_WIN::GetColorLGray_WIN() )


//*****************************************************************************
class GBrush_WIN : public GBrush
{
friend class GMaster_WIN;
friend class GDC_WIN;

public:
	static GBrush_WIN* GetBrushWhite_WIN();
	static GBrush_WIN* GetBrushBlack_WIN();
	static GBrush_WIN* GetBrushRed_WIN();
	static GBrush_WIN* GetBrushGray_WIN();
	static GBrush_WIN* GetBrushLGray_WIN();

private:
	GBrush_WIN();
	GBrush_WIN(const GBrush_WIN &r_brush_WIN);
	GBrush_WIN(unsigned char r, unsigned char g, unsigned char b, EBrushStyle bs = EBrushSolid);
	GBrush_WIN(const GColor &r_color, EBrushStyle bs = EBrushSolid);
public:
	virtual ~GBrush_WIN();

public:
	virtual void operator =(const GBrush &r_brush) { operator=((const GBrush_WIN&)r_brush); }

public:
	void operator =(const GBrush_WIN &r_brush_WIN);

private:
	void CreateHBRUSH(COLORREF colorRef, EBrushStyle bs);

private:
	HBRUSH m_hBrush;
	int m_iDeleteBrush; // if set - m_hBrush must be deleted in destructor.
};
#define theBrushWhite_WIN  ( GBrush_WIN::GetBrushWhite_WIN() )
#define theBrushBlack_WIN  ( GBrush_WIN::GetBrushBlack_WIN() )
#define theBrushRed_WIN    ( GBrush_WIN::GetBrushRed_WIN()   )
#define theBrushGray_WIN   ( GBrush_WIN::GetBrushGray_WIN()  )
#define theBrushLGray_WIN  ( GBrush_WIN::GetBrushLGray_WIN() )


//*****************************************************************************
class GPen_WIN : public GPen
{
friend class GMaster_WIN;
friend class GDC_WIN;

private:
	GPen_WIN();
	// if iWidth is 0 or 1 ePenStyle is ignored and used ESolid.
	GPen_WIN(int iWidth, const GColor &r_color, EPenStyle ePenStyle = ESolid);
public:
	virtual ~GPen_WIN();

private:
	HPEN m_hPen;
	int m_iDeletePen; // if set - m_hPen must be deleted in destructor.
};


//*****************************************************************************
class GFont_WIN : public GFont
{
friend class GMaster_WIN;
friend class GDC_WIN;

private:
	GFont_WIN();
	GFont_WIN(
		const char *pchName, int iHeight, EWeight eWeight = EMedium,
		int iItalic = 0, int iUnderline = 0, int iStrikeOut = 0, unsigned char uCharSet = 0x00,
		EOrient eOrient = EOrient0);
public:
	virtual ~GFont_WIN();

private:
	HFONT m_hFont;
	int m_iDeleteFont; // if set - m_hFont must be deleted in destructor.
};


//*****************************************************************************
class GDC_WIN : public GDC
{
friend class GMaster_WIN;

private:
	GDC_WIN(int iWnd);
	GDC_WIN(int iWnd, int iWidth, int iHeight); // MemDC.
	GDC_WIN(int iWnd, const char *pchMetafile, int iWidth, int iHeight); // MetafileDC.
	GDC_WIN(char *pchDriverName, char *pchDeviceName, DEVMODEA *pDevMode); // PrinterDC.
public:
	virtual ~GDC_WIN();

public:
	virtual void MoveTo(int x, int y);
	virtual void MoveTo(const GPoint &p);
	virtual void LineTo(int x, int y);
	virtual void LineTo(const GPoint &p);

	virtual void DrawPixel(int x, int y, unsigned long uColorRGB);
	virtual void DrawLine(int x1, int y1, int x2, int y2);
	virtual void DrawRect(int iL, int iT, int iR, int iB);
	virtual void DrawEllipse(int iL, int iT, int iR, int iB);
	virtual void DrawArc(const GRect &rc, const GPoint &pS, const GPoint &pE);
	virtual void DrawChord(const GRect &rc, const GPoint &pS, const GPoint &pE);
	virtual void DrawPie(const GRect &rc, const GPoint &pS, const GPoint &pE);
	virtual void DrawRoundRect(const GRect &rc, const GPoint &p);

	virtual void DrawText(
		const char *pchText, int iL, int iT, int iR, int iB, EHAlign eHA, EVAlign eVA);
	virtual void DrawTextEllipsis(
		const char *pchText, int iL, int iT, int iR, int iB, EHAlign eHA, EVAlign eVA);
	virtual void OutText(const wchar_t *pchText, int iX, int iY);

	virtual void FillRect(int iL, int iT, int iR, int iB, GBrush *pBrush = 0);
	virtual void FillPolygon(GPoint *pPoints, int iNumPoints, GBrush *pBrush = 0);

	virtual void DrawFocusRect(int iL, int iT, int iR, int iB);

	virtual void DrawPushButton(int iL, int iT, int iR, int iB, bool bPushed, bool bFocused);
	virtual void DrawCheckButton(int iL, int iT, int iR, int iB, bool bChecked);

public:
	virtual int GetPhysicalWidth();
	virtual int GetPhysicalHeight();
	//
	virtual int GetPhysicalOffsetX();
	virtual int GetPhysicalOffsetY();
	//
	virtual int GetLogicalHorzSzMM();
	virtual int GetLogicalVertSzMM();
	//
	virtual int GetLogicalDpiX();
	virtual int GetLogicalDpiY();

	virtual int GetMapMode();
	virtual int SetMapMode(int iMode);
	//
	virtual int GetWindowOrigin(int& r_iX, int& r_iY);
	virtual int SetWindowOrigin(int iX, int iY);
	//
	virtual int GetViewportOrigin(int& r_iX, int& r_iY);
	virtual int SetViewportOrigin(int iX, int iY);
	//
	virtual int GetWindowExtents(int& r_iX, int& r_iY);
	virtual int SetWindowExtents(int iX, int iY);
	//
	virtual int GetViewportExtents(int& r_iX, int& r_iY);
	virtual int SetViewportExtents(int iX, int iY);

	virtual int GetFlushMode();
	virtual void SetFlushMode(int iFlushImmediately);
	virtual void Flush();

	virtual GRgn* GetClipRegion(bool *pClipExist);
	virtual void SetClipRegion(const GRgn *pClipRgn);

	virtual void SetClipRegionDep(const void *rgn);

	virtual EBkMode GetBackgroundMode();
	virtual void SetBackgroundMode(EBkMode eBkMode);

	virtual unsigned long GetBackgroundColor();
	virtual void SetBackgroundColor(unsigned long uColorRGB);

	virtual int GetROP2();
	virtual void SetROP2copy();
	virtual void SetROP2xor();
	virtual void SetROP2not();
	virtual void RestoreROP2(int iRop);

	virtual unsigned long GetTextColor();
	virtual void SetTextColor(unsigned long uColorRGB);

	virtual void GetTextAlign(EHAlign &r_eHA, EVAlign &r_eVA);
	virtual void SetTextAlign(EHAlign eHA, EVAlign eVA);

	virtual GPen* GetPen();
	virtual void SetPen(GPen *pPen);

	virtual GFont* GetFont();
	virtual void SetFont(GFont *pFont);

	virtual GBrush* GetBrush();
	virtual void SetBrush(GBrush *pBrush);

public:
	virtual int GetTextHeight(const char *pchText);
	virtual int GetTextWidth(const char *pchText);
	virtual void GetTextWH(const char *pchText, int& r_iCx, int& r_iCy);
	virtual GSize GetTextExtent(const char *pchText);

public:
	virtual int CopyArea(
		int iDestX, int iDestY, GDC *pSrcGDC, int iSrcX, int iSrcY, int iWidth, int iHeight);

public:
	virtual void* GetBITMAP();
	virtual int GetDimension(int *piWidth, int *piHeight);
	virtual int CopyToBITMAP(void *pDestBitmap, int iWidth, int iHeight);

private:
	HWND m_hWnd;
	HDC m_hRefDC; // for metfileDC.
	HDC m_hTmpDC; // for memDC.
	HDC m_hDC;
	int m_iNeedDeleteDC; // 1 - ::DeleteObject(m_hDC), 0- ::ReleaseDC(m_hWnd, m_hDC);
	int m_iPrintDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;

	HPEN m_hPenNull;
};


#endif // ndef GDC_WIN_H__EED29528_5CF8_4819_B059_C157EF5A6DDA__INCLUDED
