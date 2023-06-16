#include "stdafx.h"
#include "gui.h"


//*****************************************************************************
// GPoint
//

//=============================================================================
GPoint::GPoint()
	: x(0)
	, y(0)
{
}


//=============================================================================
GPoint::GPoint(int _x, int _y)
	: x(_x)
	, y(_y)
{
}


//=============================================================================
GPoint::GPoint(const GPoint &r_p)
	: x(r_p.x)
	, y(r_p.y)
{
}


//=============================================================================
void GPoint::operator =(const GPoint &r_p)
{
	x = r_p.x;
	y = r_p.y;
}


//=============================================================================
bool GPoint::operator ==(GPoint point)
{
	return (x == point.x && y == point.y);
}


//=============================================================================
bool GPoint::operator !=(GPoint point)
{
	return (x != point.x || y != point.y);
}


//=============================================================================
void GPoint::operator +=(GPoint point)
{
	x += point.x; y += point.y;
}


//=============================================================================
void GPoint::operator -=(GPoint point)
{
	x -= point.x; y -= point.y;
}


//=============================================================================
void GPoint::SetPoint(int _x, int _y)
{
	x = _x;
	y = _y;
}


//*****************************************************************************
// GRect
//

//=============================================================================
GRect::GRect() : l(0), t(0), r(0), b(0) {}


//=============================================================================
GRect::GRect(int _l, int _t, int _r, int _b)
	: l(_l)
	, t(_t)
	, r(_r)
	, b(_b)
{
}


//=============================================================================
GRect::GRect(GPoint lt, GPoint rb)
	: l(lt.x)
	, t(lt.y)
	, r(rb.x)
	, b(rb.y)
{
}


//=============================================================================
GRect::GRect(const GRect &r_rect)
	: l(r_rect.l)
	, t(r_rect.t)
	, r(r_rect.r)
	, b(r_rect.b)
{
}


//=============================================================================
void GRect::operator =(const GRect &r_rect)
{
	l = r_rect.l;
	t = r_rect.t;
	r = r_rect.r;
	b = r_rect.b;
}


//=============================================================================
void GRect::operator |= (const GRect &r_rect)
{
	if(0 == r_rect.l && 0 == r_rect.t && 0 == r_rect.r && 0 == r_rect.b)
		return;
	if(0 == l && 0 == t && 0 == r && 0 == b)
	{
		*this = r_rect;
		return;
	}
	l = l < r_rect.l ? l : r_rect.l;
	t = t < r_rect.t ? t : r_rect.t;
	r = r > r_rect.r ? r : r_rect.r;
	b = b > r_rect.b ? b : r_rect.b;
}


//=============================================================================
int GRect::Width() const
{
	return r-l;
}


//=============================================================================
int GRect::Height() const
{
	return b-t;
}


//=============================================================================
GPoint GRect::TopLeft() const
{
	return GPoint(l, t);
}


//=============================================================================
GPoint GRect::BottomRight() const
{
	return GPoint(r, b);
}


//=============================================================================
GPoint GRect::CenterPoint() const
{
	return GPoint((l+r)/2, (t+b)/2);
}


//=============================================================================
int GRect::IntersectRect(GRect *pSrc1, GRect *pSrc2)
{
	l = pSrc1->l > pSrc2->l ? pSrc1->l : pSrc2->l;
	t = pSrc1->t > pSrc2->t ? pSrc1->t : pSrc2->t;
	r = pSrc1->r < pSrc2->r ? pSrc1->r : pSrc2->r;
	b = pSrc1->b < pSrc2->b ? pSrc1->b : pSrc2->b;

	if(l >= r || t >= b)
	{
		l = t = r = b = 0;
		return 0;
	}
	return 1;
}


//=============================================================================
void GRect::NormalizeRect()
{
	if(l > r)
	{
		int a = r;
		r = l;
		l = a;
	}
	if(t > b)
	{
		int a = t;
		t = b;
		b = a;
	}
}


//=============================================================================
void GRect::OffsetRect(GPoint p)
{
	l += p.x;
	t += p.y;
	r += p.x;
	b += p.y;
}


//=============================================================================
void GRect::InflateRect(int dx, int dy)
{
	l -= dx;
	t -= dx;
	r += dy;
	b += dy;
}


//=============================================================================
int GRect::PtInRect(GPoint p) const
{
	if(l <= p.x && p.x < r &&
	   t <= p.y && p.y < b)
	{
		return 1;
	}
	return 0;
}


//=============================================================================
bool GRect::IsRectNull() const
{
	return (l == 0 && r == 0 && t == 0 && b == 0);
}
