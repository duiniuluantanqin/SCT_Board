// EllipseUnit.h: interface for the CEllipseUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSEUNIT_H__86F2EC75_146E_4B9E_8C51_F09D4658ACD2__INCLUDED_)
#define AFX_ELLIPSEUNIT_H__86F2EC75_146E_4B9E_8C51_F09D4658ACD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CEllipseUnit : public CBaseUnit  
{
 public:
	CEllipseUnit();
	virtual ~CEllipseUnit();
 public:
	 virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	 virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	 virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	 virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
	 virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_ELLIPSEUNIT_H__86F2EC75_146E_4B9E_8C51_F09D4658ACD2__INCLUDED_)
