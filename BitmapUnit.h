// BitmapUnit.h: interface for the CBitmapUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPUNIT_H__C9B41484_F55B_46AF_9EE9_884B8976794B__INCLUDED_)
#define AFX_BITMAPUNIT_H__C9B41484_F55B_46AF_9EE9_884B8976794B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CBitmapUnit : public CBaseUnit  
{
public:
	CBitmapUnit();
	virtual ~CBitmapUnit();
public:
	void ErasedDraw(CDC *memDC,DataInfo &pDataInfo, HRGN ErasedRgn);
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	virtual void ReDraw(CDC* memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_BITMAPUNIT_H__C9B41484_F55B_46AF_9EE9_884B8976794B__INCLUDED_)
