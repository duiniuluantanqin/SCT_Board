// RectUnit.h: interface for the CRectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTUNIT_H__647CD8A9_64E1_475A_B7FB_174E19167DA3__INCLUDED_)
#define AFX_RECTUNIT_H__647CD8A9_64E1_475A_B7FB_174E19167DA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CRectUnit : public CBaseUnit  
{
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	virtual void ReDraw(CDC* memDC,DataInfo &pDataInfo);
	virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	CRectUnit();
	virtual ~CRectUnit();
	virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_RECTUNIT_H__647CD8A9_64E1_475A_B7FB_174E19167DA3__INCLUDED_)
