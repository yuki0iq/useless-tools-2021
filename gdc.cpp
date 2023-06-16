#include "stdafx.h"

#ifdef _WIN32
#   include "base_windows.h"
#endif

//#include "PPW/gdc.h"
#ifdef _WIN32
#  include "gdc_win.h"
#else
#  include "gdc_nix.h"
#endif // def _WIN32

#include <string.h>


//=============================================================================
#if (defined _WIN32 && defined _DEBUG && !defined DONT_USE_AFX)
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif // (defined _WIN32 && defined _DEBUG && !defined DONT_USE_AFX)


//*****************************************************************************
// GSize
//

//=============================================================================
GSize::GSize()
	: cx(0),
	cy(0)
{
}


//=============================================================================
GSize::GSize(int _cx, int _cy)
	: cx(_cx)
	, cy(_cy)
{
}


//*****************************************************************************
// GMaster
//

//=============================================================================
GMaster* GMaster::GetGMaster()
{
#ifdef _WIN32
	return theGMaster_WIN;
#else
	return theGMaster_NIX;
#endif // def _WIN32
}


//=============================================================================
GFont* GMaster::GetGFont(GLogFont *pGLogFont)
{
	return GetGFont(
		pGLogFont->m_pchName, pGLogFont->m_iHeight, pGLogFont->m_eWeight,
		pGLogFont->m_iItalic, pGLogFont->m_iUnderline, pGLogFont->m_iStrikeOut, pGLogFont->m_uCharSet,
		pGLogFont->m_eOrient);
}


//*****************************************************************************
// SAVE_COLOR_RGB
//

//=============================================================================
void Save_COLOR(GColor *pColor, char *pBuff)
{
	int iSaveColor = pColor->GetColorRGBIndep();
	memcpy(pBuff, &iSaveColor, sizeof(int));
}


//=============================================================================
int Load_COLOR(GColor* &r_pColor, char *pBuff)
{
	int iSaveColor;
	memcpy(&iSaveColor, pBuff, sizeof(int));

	if(iSaveColor == (int)r_pColor->GetColorRGBIndep())
	{
		return 0;
	}
	theGMaster->ReleaseGColor(r_pColor);
	r_pColor = theGMaster->GetGColor(iSaveColor);
	return 1;
}


//*****************************************************************************
// GColor
//

//=============================================================================
GColor* GColor::GetColorWhite()
{
#ifdef _WIN32
	return theColorWhite_WIN;
#else
	return theColorWhite_NIX;
#endif // def _WIN32
}
//=============================================================================
GColor* GColor::GetColorBlack()
{
#ifdef _WIN32
	return theColorBlack_WIN;
#else
	return theColorBlack_NIX;
#endif // def _WIN32
}
//=============================================================================
GColor* GColor::GetColorRed()
{
#ifdef _WIN32
	return theColorRed_WIN;
#else
	return theColorRed_NIX;
#endif // def _WIN32
}
//=============================================================================
GColor* GColor::GetColorGray()
{
#ifdef _WIN32
	return theColorGray_WIN;
#else
	return theColorGray_NIX;
#endif // def _WIN32
}
//=============================================================================
GColor* GColor::GetColorLGray()
{
#ifdef _WIN32
	return theColorLGray_WIN;
#else
	return theColorLGray_NIX;
#endif // def _WIN32
}


//=============================================================================
GColor::GColor(unsigned char r, unsigned char g, unsigned char b)
	: m_red(r)
	, m_green(g)
	, m_blue(b)
{
}


//=============================================================================
GColor::~GColor()
{
}


//=============================================================================
void GColor::operator =(const GColor &r_color)
{
	m_red   = r_color.m_red;
	m_green = r_color.m_green;
	m_blue  = r_color.m_blue;
}


//=============================================================================
unsigned long GColor::GetColorRGBIndep() const
{
	return RGB_SC(m_red, m_green, m_blue);
}


//=============================================================================
void GColor::GetColorRGB(unsigned char &r_r, unsigned char &r_g, unsigned char &r_b) const
{
	r_r = m_red;
	r_g = m_green;
	r_b = m_blue;
}


//*****************************************************************************
// GBrush
//

//=============================================================================
GBrush* GBrush::GetBrushWhite()
{
#ifdef _WIN32
	return theBrushWhite_WIN;
#else
	return theBrushWhite_NIX;
#endif // def _WIN32
}
//=============================================================================
GBrush* GBrush::GetBrushBlack()
{
#ifdef _WIN32
	return theBrushBlack_WIN;
#else
	return theBrushBlack_NIX;
#endif // def _WIN32
}
//=============================================================================
GBrush* GBrush::GetBrushRed()
{
#ifdef _WIN32
	return theBrushRed_WIN;
#else
	return theBrushRed_NIX;
#endif // def _WIN32
}
//=============================================================================
GBrush* GBrush::GetBrushGray()
{
#ifdef _WIN32
	return theBrushGray_WIN;
#else
	return theBrushGray_NIX;
#endif // def _WIN32
}
//=============================================================================
GBrush* GBrush::GetBrushLGray()
{
#ifdef _WIN32
	return theBrushLGray_WIN;
#else
	return theBrushLGray_NIX;
#endif // def _WIN32
}


//=============================================================================
GBrush::GBrush()
	: m_eBrushType(EBrushTypeHollow)
	, m_eBrushStyle(EBrushSolid)
	, m_pBrushColor(0)
{
}


//=============================================================================
GBrush::~GBrush()
{
	if(0 != m_pBrushColor)
	{
		theGMaster->ReleaseGColor(m_pBrushColor);
		m_pBrushColor = 0;
	}
}


//=============================================================================
void GBrush::operator =(const GBrush &r_brush)
{
	m_eBrushType  = r_brush.m_eBrushType;
	m_eBrushStyle = r_brush.m_eBrushStyle;
	if(0 == r_brush.m_pBrushColor)
	{
		if(0 != m_pBrushColor)
		{
			theGMaster->ReleaseGColor(m_pBrushColor);
			m_pBrushColor = 0;
		}
	}
	else // (0 != r_brush.m_pBrushColor)
	{
		if(0 == m_pBrushColor)
		{
			m_pBrushColor = theGMaster->GetGColor();
		}
		*m_pBrushColor = *r_brush.m_pBrushColor;
	}
}


//=============================================================================
void GBrush::InitBrush(
	EBrushType bt, EBrushStyle bs, unsigned char r, unsigned char g, unsigned char b)
{
	if(0 != m_pBrushColor)
	{
		return;
	}
	m_eBrushType  = bt;
	m_eBrushStyle = bs;
	m_pBrushColor = theGMaster->GetGColor(r, g, b);
}


//=============================================================================
void GBrush::InitBrush(EBrushType bt, EBrushStyle bs, const GColor &r_color)
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	r_color.GetColorRGB(r, g, b);
	InitBrush(bt, bs, r, g, b);
}


//*****************************************************************************
// GLogFont
//

//=============================================================================
GLogFont::GLogFont()
	: m_pchName(0)
	, m_iHeight(0)
	, m_eWeight(EMedium)
	, m_iItalic(0)
	, m_iUnderline(0)
	, m_iStrikeOut(0)
	, m_uCharSet(0x00)
	, m_eOrient(EOrient0)
{
}


//=============================================================================
GLogFont::GLogFont(const GLogFont &r_logFont)
	: m_pchName(0)
	, m_iHeight(0)
	, m_eWeight(EMedium)
	, m_iItalic(0)
	, m_iUnderline(0)
	, m_iStrikeOut(0)
	, m_uCharSet(0x00)
	, m_eOrient(EOrient0)
{
	*this = r_logFont;
}


//=============================================================================
GLogFont::~GLogFont()
{
	if(NULL != m_pchName)
	{
		delete[] m_pchName;
		m_pchName = NULL;
	}
}


//=============================================================================
void GLogFont::operator =(const GLogFont &r_logFont)
{
	if(NULL != m_pchName)
	{
		delete[] m_pchName;
		m_pchName = NULL;
	}
	if(NULL != r_logFont.m_pchName)
	{
		m_pchName = new char[strlen(r_logFont.m_pchName)+1];
		strcpy(m_pchName,r_logFont.m_pchName);
	}

	m_iHeight    = r_logFont.m_iHeight;
	m_eWeight    = r_logFont.m_eWeight;
	m_iItalic    = r_logFont.m_iItalic;
	m_iUnderline = r_logFont.m_iUnderline;
	m_iStrikeOut = r_logFont.m_iStrikeOut;
	m_uCharSet   = r_logFont.m_uCharSet;
	m_eOrient    = r_logFont.m_eOrient;
}


//*****************************************************************************
// GFont
//

//=============================================================================
void GFont::InitFont(GFont *pGFont)
{
	if(NULL != m_logFont.m_pchName)
	{
		delete[] m_logFont.m_pchName;
		m_logFont.m_pchName = NULL;
	}
	if(NULL != pGFont->GetName())
	{
		int iLen = strlen(pGFont->GetName()) + 1;
		m_logFont.m_pchName = new char[iLen];
		strcpy(m_logFont.m_pchName, pGFont->GetName());
	}
	m_logFont.m_iHeight    = pGFont->GetHeight();
	m_logFont.m_eWeight    = pGFont->GetWeight();
	m_logFont.m_iItalic    = pGFont->GetItalic();
	m_logFont.m_iUnderline = pGFont->GetUnderline();
	m_logFont.m_iStrikeOut = pGFont->GetStrikeOut();
	m_logFont.m_uCharSet   = pGFont->GetCharSet();
	m_logFont.m_eOrient    = pGFont->GetOrient();

	if(NULL != m_logFontLoaded.m_pchName)
	{
		delete[] m_logFontLoaded.m_pchName;
		m_logFontLoaded.m_pchName = NULL;
	}
	if(NULL != pGFont->GetNameLoaded())
	{
		int iLen = strlen(pGFont->GetNameLoaded()) + 1;
		m_logFontLoaded.m_pchName = new char[iLen];
		strcpy(m_logFontLoaded.m_pchName, pGFont->GetNameLoaded());
	}
	m_logFontLoaded.m_iHeight    = pGFont->GetHeightLoaded();
	m_logFontLoaded.m_eWeight    = pGFont->GetWeightLoaded();
	m_logFontLoaded.m_iItalic    = pGFont->GetItalicLoaded();
	m_logFontLoaded.m_iUnderline = pGFont->GetUnderlineLoaded();
	m_logFontLoaded.m_iStrikeOut = pGFont->GetStrikeOutLoaded();
	m_logFontLoaded.m_uCharSet   = pGFont->GetCharSet();
	m_logFontLoaded.m_eOrient    = pGFont->GetOrientLoaded();
}


//*****************************************************************************
// Save_LOGFONT_XXX
//

//=============================================================================
void Save_LOGFONT_Save(GFont *pFont, char *pBuff)
{
	Save_LOGFONT font;
	memset(&font, 0, sizeof(Save_LOGFONT));
	font.lfHeight    = pFont->GetHeight();
	font.lfWeight    = (EMedium == pFont->GetWeight() ? 400 : 700); // EMedium ? FW_NORMAL : FW_BOLD
	font.lfItalic    = (1 == pFont->GetItalic()    ? 1 : 0);
	font.lfUnderline = (1 == pFont->GetUnderline() ? 1 : 0);
	font.lfStrikeOut = (1 == pFont->GetStrikeOut() ? 1 : 0);
	font.lfCharSet   = pFont->GetCharSet();
	strncpy(font.lfFaceName, pFont->GetName(), 31);

	memcpy(pBuff, &font, sizeof(Save_LOGFONT));
}


//=============================================================================
int Save_LOGFONT_Load(GFont* &r_pFont, char *pBuff)
{
	Save_LOGFONT font;
	memcpy(&font, pBuff, sizeof(Save_LOGFONT));

	EWeight eSavedWeight = (font.lfWeight <= 600 ? EMedium : EBold);
	unsigned char curItalic    = (1 == r_pFont->GetItalic()    ? 1 : 0);
	unsigned char curUnderline = (1 == r_pFont->GetUnderline() ? 1 : 0);
	unsigned char curStrikeOut = (1 == r_pFont->GetStrikeOut() ? 1 : 0);
	unsigned char curCharSet   = r_pFont->GetCharSet();

#ifdef _WIN32
	if(_stricmp(font.lfFaceName, r_pFont->GetName()) == 0 &&
#else
	if(strcmp(font.lfFaceName, r_pFont->GetName()) == 0 &&
#endif // def _WIN32
	   font.lfHeight    == r_pFont->GetHeight() &&
	   eSavedWeight     == r_pFont->GetWeight() &&
	   font.lfItalic    == curItalic &&
	   font.lfUnderline == curUnderline &&
	   font.lfStrikeOut == curStrikeOut &&
	   font.lfCharSet   == curCharSet)
	{
		return 0;
	}

	theGMaster->ReleaseGFont(r_pFont);
	r_pFont = theGMaster->GetGFont(
		font.lfFaceName, font.lfHeight, eSavedWeight,
		0 == font.lfItalic ? 0 : 1,
		0 == font.lfUnderline ? 0 : 1,
		0 == font.lfUnderline ? 0 : 1,
		font.lfCharSet);

	return 1;
}


//=============================================================================
void Save_LOGFONT_Save(const GLogFont &r_logFont, Save_LOGFONT &r_font)
{
	memset(&r_font, 0, sizeof(Save_LOGFONT));
	r_font.lfHeight    = r_logFont.m_iHeight;
	r_font.lfWeight    = (EMedium == r_logFont.m_eWeight ? 400 : 700); // EMedium ? FW_NORMAL : FW_BOLD
	r_font.lfItalic    = (1 == r_logFont.m_iItalic    ? 1 : 0);
	r_font.lfUnderline = (1 == r_logFont.m_iUnderline ? 1 : 0);
	r_font.lfStrikeOut = (1 == r_logFont.m_iStrikeOut ? 1 : 0);
	r_font.lfCharSet   = r_logFont.m_uCharSet;
	strncpy(r_font.lfFaceName, r_logFont.m_pchName, 31);
}


//=============================================================================
void Save_LOGFONT_Load(GLogFont &r_logFont, const Save_LOGFONT &r_font)
{
	r_logFont.m_iHeight    = r_font.lfHeight;
	r_logFont.m_eWeight    = r_font.lfWeight == 400 ? EMedium : EBold;
	r_logFont.m_iItalic    = (1 == r_font.lfItalic    ? 1 : 0);
	r_logFont.m_iUnderline = (1 == r_font.lfUnderline ? 1 : 0);
	r_logFont.m_iStrikeOut = (1 == r_font.lfStrikeOut ? 1 : 0);
	r_logFont.m_uCharSet   = r_font.lfCharSet;
	if(r_logFont.m_pchName != NULL)
	{
		delete[] r_logFont.m_pchName;
	}
	r_logFont.m_pchName = new char[strlen(r_font.lfFaceName)+1];
	strcpy(r_logFont.m_pchName, r_font.lfFaceName);
}
