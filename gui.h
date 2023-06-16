#ifndef GUI_H
#define GUI_H

//=============================================================================
enum EPenStyle { ESolid=0, EDash=1, EDot=2, EDashDot=3, EDashDotDot=4 };
enum EWeight { EMedium=0, EBold=1 };
enum EHAlign { EHLeft=0, EHCenter=6, EHRight=2 };
enum EVAlign { EVTop=0, EVCenter=6, EVBottom=8 };
enum EOrient { EOrient0=0, EOrient90=1, EOrient180=2, EOrient270=3 };
enum EBkMode { ETransparent=1, EOpaque=2 };
enum EClipMode { EClipModeOR=0, EClipModeXOR=1 };
enum EBrushType { EBrushTypeFill=0, EBrushTypeHollow=1 };
enum EBrushStyle
{
	EBrushSolid          =-1,
	EBrushHatchHoriz     =0,      /* ----- */
	EBrushHatchVert      =1,      /* ||||| */
	EBrushHatchFDiag     =2,      /* \\\\\ */
	EBrushHatchBDiag     =3,      /* ///// */
	EBrushHatchCross     =4,      /* +++++ */
	EBrushHatchDiagCross =5       /* xxxxx */
};


//*****************************************************************************
struct GPoint
{
	GPoint();
	GPoint(int _x, int _y);
	GPoint(const GPoint &r_p);

	void operator =(const GPoint &r_p);
	bool operator ==(GPoint point);
	bool operator !=(GPoint point);
	void operator +=(GPoint point);
	void operator -=(GPoint point);

	void SetPoint(int _x, int _y);

	int x;
	int y;
};


//*****************************************************************************
struct GRect
{
	GRect();
	GRect(int _l, int _t, int _r, int _b);
	GRect(GPoint lt, GPoint rb);
	GRect(const GRect &r_rect);

	void operator =(const GRect &r_rect);
	void operator |= (const GRect &r_rect);

	int Width() const;
	int Height() const;
	GPoint TopLeft() const;
	GPoint BottomRight() const;
	GPoint CenterPoint() const;

	int IntersectRect(GRect *pSrc1, GRect *pSrc2);
	void NormalizeRect();
	void OffsetRect(GPoint p);
	void InflateRect(int dx, int dy);

	int PtInRect(GPoint p) const;

	bool IsRectNull() const;

	int l;
	int t;
	int r;
	int b;
};


//=============================================================================
// SAVE_COLOR_RGB like COLORREF in WINDOWS.
//
#define RGB_SC(r, g, b)        ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))

#define LOBYTE_SC(w)           ((unsigned char)((unsigned long)(w) & 0xff))

#define GetRValue_SC(rgb)      (LOBYTE_SC(rgb))
#define GetGValue_SC(rgb)      (LOBYTE_SC(((unsigned short)(rgb)) >> 8))
#define GetBValue_SC(rgb)      (LOBYTE_SC((rgb)>>16))


//*****************************************************************************
// For backward compatibility save font as LOGFONT in WINDOWS
//
struct Save_LOGFONT
{
	long lfHeight;                  // <-> GFont::m_iHeight
	long lfWidth;                   // ignored
	long lfEscapement;              // ignored
	long lfOrientation;             // ignored
	long lfWeight;                  // lfWeight <= FW_SEMIBOLD(600) -> EWeight::EMedium
	                                // lfWeight >  FW_SEMIBOLD(600) -> EWeight::EBold
	                                // GFont::m_eWeight == EWeight::EMedium -> FW_NORMAL(400)
	                                // GFont::m_eWeight == EWeight::EBold   -> FW_BOLD(700)
	unsigned char lfItalic;         // <-> GFont::m_iItalic
	unsigned char lfUnderline;      // <-> GFont::m_iUnderline
	unsigned char lfStrikeOut;      // <-> GFont::m_iStrikeOut
	unsigned char lfCharSet;        // <-> GFont::m_iCharSet
	unsigned char lfOutPrecision;   // ignored
	unsigned char lfClipPrecision;  // ignored
	unsigned char lfQuality;        // ignored
	unsigned char lfPitchAndFamily; // ignored
	char lfFaceName[32];            // GFont::m_pchName
};


#endif // GUI_H
