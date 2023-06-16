#ifndef GDC_H__1F953647_7A40_4774_B25E_675CD4C883CD__INCLUDED
#define GDC_H__1F953647_7A40_4774_B25E_675CD4C883CD__INCLUDED
#include "stdafx.h"


#include "gui.h"


//*****************************************************************************
struct GSize
{
	GSize();
	GSize(int _cx, int _cy);

	int cx;
	int cy;
};


//=============================================================================
class GRgn;
class GColor;
class GBrush;
class GPen;
class GFont;
struct GLogFont;
class GDC;


//*****************************************************************************
class GMaster
{
public:
	static GMaster* GetGMaster();

public:
	GMaster() {}
	virtual ~GMaster() {}

public:
	// Region.
	virtual GRgn* GetGRgn() = 0; // Empty (Null) rgn.
	virtual GRgn* GetGRgn(GPoint *pPoints, int iNumPoint) = 0;
	virtual GRgn* GetGRgn(GRect *pRects, int iNumRect) = 0;
	virtual GRgn* GetGRgn(GRect rect) = 0; // Elliptic rgn.
	virtual void ReleaseGRgn(GRgn *pGRgn) = 0;

	// Color.
	virtual GColor* GetGColor() = 0;
	virtual GColor* GetGColor(const GColor &r_color) = 0;
	virtual GColor* GetGColor(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual GColor* GetGColor(unsigned long uColorRGBIndep) = 0;
	virtual void ReleaseGColor(GColor *pGColor) = 0;

	// Brush.
	virtual GBrush* GetGBrush() = 0; // Empty (Null/Hollow) brush.
	virtual GBrush* GetGBrush(unsigned char r, unsigned char g, unsigned char b, EBrushStyle bs = EBrushSolid) = 0;
	virtual GBrush* GetGBrush(const GColor &r_color, EBrushStyle bs = EBrushSolid) = 0;
	virtual void ReleaseGBrush(GBrush *pGBrush) = 0;

	// Pen.
	virtual GPen* GetGPen() = 0;
	// if iWidth is 0 or 1 ePenStyle is ignored and used ESolid.
	virtual GPen* GetGPen(int iWidth, const GColor &r_color, EPenStyle ePenStyle = ESolid) = 0;
	virtual void ReleaseGPen(GPen *pGPen) = 0;

	// Font.
	virtual GFont* GetDefaultGFont(int iDefaultFontHeight = 12) = 0;
	virtual GFont* GetGFont() = 0;
	virtual GFont* GetGFont(GLogFont *pGLogFont);
	virtual GFont* GetGFont(
		const char *pchName, int iHeight, EWeight eWeight = EMedium,
		int iItalic = 0, int iUnderline = 0, int iStrikeOut = 0, unsigned char uCharSet = 0x00,
		EOrient eOrient = EOrient0) = 0;
	virtual void ReleaseGFont(GFont *pGFont) = 0;

	// DC.
	virtual GDC* GetGDC(int iWnd) = 0;
	virtual GDC* GetMemGDC(int iWnd, int iWidth, int iHeight) = 0;
	virtual GDC* GetMetafileGDC(int iWnd, const char *pchMetafile, int iWidth, int iHeight) = 0;
	virtual GDC* GetPrinterGDC(char *pchDriverName, char *pchDeviceName, void *pDM) = 0;
	virtual void ReleaseGDC(GDC *pGDC) = 0;

	// Retrun 0 if success otherwise return -1.
	virtual int StartPrintDoc(GDC *pGDC, char *pchDocName) = 0;
	virtual int EndPrintDoc(GDC *pGDC) = 0;
	//
	// Retrun 0 if success otherwise return -1.
	virtual int StartPrintPage(GDC *pGDC) = 0;
	virtual int EndPrintPage(GDC *pGDC) = 0;
};
#define theGMaster     ( GMaster::GetGMaster() )


//*****************************************************************************
class GRgn
{
protected:
	GRgn() {}
public:
	virtual ~GRgn() {}

public:
	virtual void Offset(int dX, int dY) = 0;
	virtual void Combine(GRgn *pGRgn) = 0;
	virtual void Combine(int l, int t, int r, int b) = 0;
	virtual void Intersect(GRgn *pGRgn) = 0;
	virtual void Subtract(GRgn *pGRgn) = 0;
	virtual void Copy(GRgn *pGRgn) = 0;
	virtual void CopyDep(const void *rgn) = 0;
	virtual void SetRect(int l, int t, int r, int b) = 0;
	virtual void GetRgnBox(GRect &rect) = 0;
	virtual bool RectInRegion(GRect &rect) = 0;
	// Return 0 if rgn empty.
	virtual int IsEmpty() = 0;

	virtual void* GetRgnDep() = 0;
};


//*****************************************************************************
// SAVE_COLOR_RGB like COLORREF in WINDOWS.
//
#define RGB_SC(r, g, b)        ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))

#define LOBYTE_SC(w)           ((unsigned char)((unsigned long)(w) & 0xff))

#define GetRValue_SC(rgb)      (LOBYTE_SC(rgb))
#define GetGValue_SC(rgb)      (LOBYTE_SC(((unsigned short)(rgb)) >> 8))
#define GetBValue_SC(rgb)      (LOBYTE_SC((rgb)>>16))

void Save_COLOR(GColor *pColor, char *pBuff);
// Load_COLOR return 1 if color changed else return 0.
int Load_COLOR(GColor* &r_pColor, char *pBuff);


//*****************************************************************************
class GColor
{
public:
	static GColor* GetColorWhite();
	static GColor* GetColorBlack();
	static GColor* GetColorRed();
	static GColor* GetColorGray();
	static GColor* GetColorLGray();

protected:
	GColor(unsigned char r, unsigned char g, unsigned char b);
public:
	virtual ~GColor();

public:
	virtual unsigned long GetColorRGBDep() const = 0;
	virtual void operator =(const GColor &r_color);

public:
	virtual unsigned long GetColorRGBIndep() const;

public:
	void GetColorRGB(unsigned char &r_r, unsigned char &r_g, unsigned char &r_b) const;

protected:
	unsigned char m_red;
	unsigned char m_green;
	unsigned char m_blue;
};
#define theColorWhite  ( GColor::GetColorWhite() )
#define theColorBlack  ( GColor::GetColorBlack() )
#define theColorRed    ( GColor::GetColorRed()   )
#define theColorGray   ( GColor::GetColorGray()  )
#define theColorLGray  ( GColor::GetColorLGray() )


//*****************************************************************************
class GBrush
{
public:
	static GBrush* GetBrushWhite();
	static GBrush* GetBrushBlack();
	static GBrush* GetBrushRed();
	static GBrush* GetBrushGray();
	static GBrush* GetBrushLGray();

protected:
	GBrush();
public:
	virtual ~GBrush();

public:
	virtual void operator =(const GBrush &r_brush);

public:
	inline EBrushType GetBrushType() const { return m_eBrushType; }
	inline EBrushStyle GetBrushStyle() const { return m_eBrushStyle; }
	inline const GColor* GetBrushColor() const { return (const GColor*)m_pBrushColor; }

protected:
	void InitBrush(
		EBrushType bt, EBrushStyle bs, unsigned char r, unsigned char g, unsigned char b);
	void InitBrush(EBrushType bt, EBrushStyle bs, const GColor &r_color);

protected:
	EBrushType m_eBrushType;
	EBrushStyle m_eBrushStyle;
	GColor *m_pBrushColor;
};
#define theBrushWhite  ( GBrush::GetBrushWhite() )
#define theBrushBlack  ( GBrush::GetBrushBlack() )
#define theBrushRed    ( GBrush::GetBrushRed()   )
#define theBrushGray   ( GBrush::GetBrushGray()  )
#define theBrushLGray  ( GBrush::GetBrushLGray() )


//*****************************************************************************
class GPen
{
protected:
	GPen() {}
public:
	virtual ~GPen() {}
};


//*****************************************************************************

#define FONT_NEG_HEIGHT_ADD    4

struct GLogFont
{
	GLogFont();
	GLogFont(const GLogFont &r_logFont);

	~GLogFont();

	void operator =(const GLogFont &r_logFont);

	char *m_pchName;
	int m_iHeight;
	EWeight m_eWeight;
	int m_iItalic;
	int m_iUnderline;
	int m_iStrikeOut;
	unsigned char m_uCharSet;  //  Not use in linux.
	EOrient m_eOrient;
};


//*****************************************************************************
class GFont
{
protected:
	GFont() {}
public:
	virtual ~GFont() {}

public:
	inline const char* GetName() const { return m_logFont.m_pchName; }
	inline int GetHeight() const { return m_logFont.m_iHeight; }
	inline EWeight GetWeight() const { return m_logFont.m_eWeight; }
	inline int GetItalic() const { return m_logFont.m_iItalic; }
	inline int GetUnderline() const { return m_logFont.m_iUnderline; }
	inline int GetStrikeOut() const { return m_logFont.m_iStrikeOut; }
	inline unsigned char GetCharSet() const { return m_logFont.m_uCharSet; }
	inline EOrient GetOrient() const { return m_logFont.m_eOrient; }

	inline const char* GetNameLoaded() const { return m_logFontLoaded.m_pchName; }
	inline int GetHeightLoaded() const { return m_logFontLoaded.m_iHeight; }
	inline EWeight GetWeightLoaded() const { return m_logFontLoaded.m_eWeight; }
	inline int GetItalicLoaded() const { return m_logFontLoaded.m_iItalic; }
	inline int GetUnderlineLoaded() const { return m_logFontLoaded.m_iUnderline; }
	inline int GetStrikeOutLoaded() const { return m_logFontLoaded.m_iStrikeOut; }
	inline unsigned char GetCharSetLoaded() const { return m_logFontLoaded.m_uCharSet; }
	inline EOrient GetOrientLoaded() const { return m_logFontLoaded.m_eOrient; }

	const GLogFont& GetGLogFont() const { return m_logFont; };

public:
	void InitFont(GFont *pGFont);

protected:
	GLogFont m_logFont;
	GLogFont m_logFontLoaded;
};


//*****************************************************************************
class GDC
{
protected:
	GDC() {}
public:
	virtual ~GDC() {}

public:
	// Set curPos to (x,y).
	virtual void MoveTo(int x, int y) = 0;
	virtual void MoveTo(const GPoint &p) = 0;
	// Draw line from curPos up to, but not including pos(x,y), with current pen.
	// Set curPos to (x,y).
	virtual void LineTo(int x, int y) = 0;
	virtual void LineTo(const GPoint &p) = 0;

	virtual void DrawPixel(int x, int y, unsigned long uColorRGB) = 0;
	// Draw line with current pen.
	virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
	// Draw line with current pen and brush.
	virtual void DrawRect(int iL, int iT, int iR, int iB) = 0;
	// Draw ellipse with current pen and brush.
	virtual void DrawEllipse(int iL, int iT, int iR, int iB) = 0;
	// Draw arc with current pen.
	// pS - first radial ending point, pE - second radial ending point.
	virtual void DrawArc(const GRect &rc, const GPoint &pS, const GPoint &pE) = 0;
	// Draw chord with current pen and brush.
	// pS - first radial ending point, pE - second radial ending point.
	virtual void DrawChord(const GRect &rc, const GPoint &pS, const GPoint &pE) = 0;
	// Draw pie with current pen and brush.
	// pS - first radial ending point, pE - second radial ending point.
	virtual void DrawPie(const GRect &rc, const GPoint &pS, const GPoint &pE) = 0;
	// NOTE:
	//     on LINUX: DrawRoundRect under construction - call DrawRect now.
	virtual void DrawRoundRect(const GRect &rc, const GPoint &p) = 0;

	// Draw text (single line) with current font and textColor.
	virtual void DrawText(
		const char *pchText, int iL, int iT, int iR, int iB, EHAlign eHA, EVAlign eVA) = 0;
	virtual void DrawTextEllipsis(
		const char *pchText, int iL, int iT, int iR, int iB, EHAlign eHA, EVAlign eVA) = 0;
	// Draw text (single line) with current font, textColor and align.
	virtual void OutText(const wchar_t *pchText, int iX, int iY) = 0;

	// Fill rect with pBrush brush. If pBrush is 0 used current brush.
	virtual void FillRect(int iL, int iT, int iR, int iB, GBrush *pBrush = 0) = 0;
	// Fill polygon with pBrush brush. If pBrush is 0 used current brush.
	virtual void FillPolygon(GPoint *pPoints, int iNumPoints, GBrush *pBrush = 0) = 0;

	// Draw a rectangle in the style used to indicate that the rectangle has the focus.
	// Use XOR function and current background color.
	virtual void DrawFocusRect(int iL, int iT, int iR, int iB) = 0;

	virtual void DrawPushButton(int iL, int iT, int iR, int iB, bool bPushed, bool bFocused) = 0;
	virtual void DrawCheckButton(int iL, int iT, int iR, int iB, bool bChecked) = 0;

public:
	// Return the width(height) of the physical page, in device units.
	virtual int GetPhysicalWidth() = 0;
	virtual int GetPhysicalHeight() = 0;
	//
	// Return the distance from the left(top) edge of the physical page
	// to the left(top) edge of the printable area, in device units.
	virtual int GetPhysicalOffsetX() = 0;
	virtual int GetPhysicalOffsetY() = 0;
	//
	// Return the Horz(Vert) size of the logical page, in mm.
	virtual int GetLogicalHorzSzMM() = 0;
	virtual int GetLogicalVertSzMM() = 0;
	//
	// Return the number of pixels per logical inch along the width(height).
	virtual int GetLogicalDpiX() = 0;
	virtual int GetLogicalDpiY() = 0;

	// Set/Get the mapping mode.
	// Return mapping mode.
	virtual int GetMapMode() = 0;
	// Return 0 if success othrewise return -1.
	// iMode:
	//     0 - Each logical unit is mapped to one device pixel. (like MM_TEXT in WINDOWS).
	//     1 - Logical units are mapped to arbitrary units with arbitrarily scaled axes.
	//         Use the SetWindowExtents and SetViewportExtents functions to specify the
	//         units, orientation, and scaling. (like MM_ANISOTROPIC in WINDOWS).
	virtual int SetMapMode(int iMode) = 0;
	//
	// Get/Set the window origin.
	// Return 0 if success othrewise return -1.
	virtual int GetWindowOrigin(int& r_iX, int& r_iY) = 0;
	virtual int SetWindowOrigin(int iX, int iY) = 0;
	//
	// Get/Set the viewport origin.
	// Return 0 if success othrewise return -1.
	virtual int GetViewportOrigin(int& r_iX, int& r_iY) = 0;
	virtual int SetViewportOrigin(int iX, int iY) = 0;
	//
	// Get/Set the x- and y- extents of the window.
	// Return 0 if success othrewise return -1.
	virtual int GetWindowExtents(int& r_iX, int& r_iY) = 0;
	virtual int SetWindowExtents(int iX, int iY) = 0;
	//
	// Get/Set the x- and y- extents of the viewport.
	// Return 0 if success othrewise return -1.
	virtual int GetViewportExtents(int& r_iX, int& r_iY) = 0;
	virtual int SetViewportExtents(int iX, int iY) = 0;

	virtual int GetFlushMode() = 0;
	virtual void SetFlushMode(int iFlushImmediately) = 0;
	virtual void Flush() = 0;

	// Caller must use theGMaster->ReleaseGRgn to release returned value.
	virtual GRgn* GetClipRegion(bool *pClipExist) = 0;
	virtual void SetClipRegion(const GRgn *pClipRgn) = 0; // If pClipRgn is 0 - remove clip region.
	// rgn must be:
	//     on WINDOWS: HRGN
	//     on LINUX: Region
	virtual void SetClipRegionDep(const void *rgn) = 0;

	virtual EBkMode GetBackgroundMode() = 0;
	virtual void SetBackgroundMode(EBkMode eBkMode) = 0;

	virtual unsigned long GetBackgroundColor() = 0;
	virtual void SetBackgroundColor(unsigned long uColorRGB) = 0;

	virtual int GetROP2() = 0;
	virtual void SetROP2copy() = 0;
	virtual void SetROP2xor() = 0;
	virtual void SetROP2not() = 0;
	virtual void RestoreROP2(int iRop) = 0; // iRop is a value returned GetROP2.

	virtual unsigned long GetTextColor() = 0;
	virtual void SetTextColor(unsigned long uColorRGB) = 0;

	virtual void GetTextAlign(EHAlign &r_eHA, EVAlign &r_eVA) = 0;
	// on WINDOWS: EVCenter - the base line of the text.
	virtual void SetTextAlign(EHAlign eHA, EVAlign eVA) = 0;

	// Caller must use theGMaster->ReleaseGPen to release returned value.
	virtual GPen* GetPen() = 0;
	// If pPen is 0 use 0-pen.
	virtual void SetPen(GPen *pPen) = 0;

	// Caller must use theGMaster->ReleaseGFont to release returned value.
	virtual GFont* GetFont() = 0;
	virtual void SetFont(GFont *pFont) = 0;

	// Caller must use theGMaster->ReleaseGBrush to release returned value.
	virtual GBrush* GetBrush() = 0;
	// If pBrush is 0 use 0-brush.
	virtual void SetBrush(GBrush *pBrush) = 0;

public:
	virtual int GetTextHeight(const char *pchText) = 0;
	virtual int GetTextWidth(const char *pchText) = 0;
	virtual void GetTextWH(const char *pchText, int& r_iCx, int& r_iCy) = 0;
	virtual GSize GetTextExtent(const char *pchText) = 0;

public:
	virtual int CopyArea(
		int iDestX, int iDestY, GDC *pSrcGDC, int iSrcX, int iSrcY, int iWidth, int iHeight) = 0;

public:
	// Return:
	//     on WINDOWS: const HBITMAP
	//     on LINUX: const Pixmap
	// Caller must not release returned value.
	virtual void* GetBITMAP() = 0;

	// Retrieve MemGDC's width and height.
	// Return 0 if success otherwise return -1.
	virtual int GetDimension(int *piWidth, int *piHeight) = 0;

	// Copy MemGDC's rectangle to the destination pDestBitmap.
	// pDestBitmap must be the same type GetBITMAP return.
	virtual int CopyToBITMAP(void *pDestBitmap, int iWidth, int iHeight) = 0;
};


//*****************************************************************************
void Save_LOGFONT_Save(GFont *pFont, char *pBuff);
	// Load return 1 if color changed else return 0.
int Save_LOGFONT_Load(GFont* &r_pFont, char *pBuff);
//
void Save_LOGFONT_Save(const GLogFont &r_logFont, Save_LOGFONT &r_font);
void Save_LOGFONT_Load(GLogFont &r_logFont, const Save_LOGFONT &r_font);


#endif // ndef GDC_H__1F953647_7A40_4774_B25E_675CD4C883CD__INCLUDED
